/*
 *	ExceptionHandlerTS.cp	-	Target-specific C++ Exception Handler support
 *
 *  Copyright © 1997 Metrowerks, Inc.  All rights reserved.
 *
 *
 *	THEORY OF OPERATION
 *
 *	Here we define any functions which implement the target-specific aspects of
 *	the standard C++ Exception Handler.
 *
 *	A MIPS stack frame looks like this:
 *
 *	HIGH ADDRESSES	|						|
 *					|						|
 *					|-----------------------|
 *					|	incoming arguments	|
 *					|						|
 *	Caller's SP   =>|-----------------------|
 *					|		Callee's		|
 *					|	local variables		|
 *					|-----------------------|
 *					|	GPR save area		|	Storage for nonvolatile GPRs used by callee
 *					|-----------------------|
 *					|	FPR save area		|	Storage for nonvolatile FPRs used by callee
 *					|-----------------------|
 *					|	outgoing arguments	|
 *					|						|
 *	Callee's SP   =>|-----------------------|
 *					|						|
 *	LOW ADDRESSES	|						|
 *
 *	Note that for functions which contain catch-blocks or call alloca(), a separate frame
 *	pointer register (r30) will point where "Callee's SP" does in the diagram above; SP
 *	may point anywhere below that.
 *
 *	The UnwindInfo for the MIPS provides the necessary info for decoding the frame:
 *
 *	|<-2 bits->|<-1 bit->|<-1 bit->|<-4 bits->|
 *	+----------+---------+---------+----------+
 *	|  unused  | has FP? | unused  |  # GPRs  |
 *	+----------+---------+---------+----------+
 *	| Frame Size (1/2/3/4 bytes as needed)	  |
 *	+-----------------------------------------+
 *	| Argument Size (1/2/3/4 bytes as needed) |
 *	+-----------------------------------------+
 *  | # FPRs (if needed) |
 *  +--------------------+
 */

#include "NMWException.h"
#include "ExceptionHandler.h"
#include "UnwindInfo.h"
#include "__config.h"
#include <__rts_info_t__.h>

/*	
 *	__PopStackFrame(context, info)
 *
 *	Restore any registers saved in the current frame, as indicated by the
 *	UnwindInfo stored in 'info->exception_record', and pop the frame.
 *
 *	We don't actually modify the runtime stack; we simply restore the values of
 *	all registers saved in the current function to their value in the function's
 *	caller, IN THE LOCAL COPY OF THE REGISTER VALUES STORED IN 'context'. When we
 *	finally transfer control to a catch-block, we'll copy these values into the
 *	actual machine registers.
 *
 *	We return the return address in the caller--the place where the function whose
 *	frame we are popping would have returned to--so that the caller can use it to
 *	find the corresponding exception table.
 *
 *	We cannot correctly set 'context->FP' to the new frame's frame pointer because
 *	we don't yet have the exception table, which tells us whether the frame used
 *	a separate frame pointer or whether 'context->FP' == 'context->SP'.
 *
 */
 
char *__PopStackFrame(ThrowContext *context, ExceptionInfo *info)
{
	char *p;
	int has_fpu;
	unsigned long framesize = context->target.frame_size;
	unsigned long paramsize = context->target.argument_size;
	unsigned long nreg = context->target.n_saved_GPRs;
	_REG_TYPE *GPRs;
	char *retaddr;
	int i;
	#if __fpu__
		unsigned long nfreg;
		_FREG_TYPE *FPRs;
	#endif
	
	p = info->exception_record;

	has_fpu = *p & UNWIND_HAS_FPU;
	#if __fpu__
		if (has_fpu) {
			nfreg = context->target.n_saved_FPRs;
			FPRs = (_FREG_TYPE *)(context->FP + paramsize + nfreg * sizeof(_FREG_TYPE));
			GPRs = (_REG_TYPE *)((unsigned long)FPRs + nreg * sizeof(_REG_TYPE));
		}
		else
			GPRs = (_REG_TYPE *)(context->FP + paramsize + nreg * sizeof(_REG_TYPE));
	#else
		GPRs = (_REG_TYPE *)(context->FP + paramsize + nreg * sizeof(_REG_TYPE));
	#endif

	if (context->target.has_frame_ptr)
		++GPRs;

		/*	get the new return address	*/
	
	if (__cpu64__ &&
	    !__option(little_endian) && 
	    !__ptr64__) {
		/*	
		 *	In this case all the registers are saved on 64 bits
		 *  but the addresses are only 32 bits.  So we have to 
		 *  convert a 64 bit register into a 32 bit quantity.  This
		 *  conversion depends on the endianness.
		 *
		 */
		retaddr = (char *)(*(address *)((address)(GPRs)+4));
	}
	else {
		retaddr = (char *)(*(address *)((address)(GPRs)));
	}


		/*	restore the frame pointer	*/

	if (context->target.has_frame_ptr) {
		context->target.GPR[30] = *--GPRs;
	}
	
		/*	
		 *	Restore S8 if S8 was used as a save over call
		 *	register. We can tell if S8 is used by looking at
		 *	the number nreg of SOC registers used.  The SOC
		 *	registers are S0-S8 or S0-S6,S8 in PC relative 
		 *	mode.
		 *
		 */ 
	else if (nreg == (__pc_relative__ ? 8 : 9)) {
		context->target.GPR[30] = *--GPRs;
		nreg--;
	}
	GPRs -= nreg;

		/*	restore the saved nonvolatile GPRs	*/
	
	for (i = 0; i < nreg; i++)
		context->target.GPR[16+i] = *GPRs++;
		
		/*	remove the frame from the stack	*/
	
	context->SP = context->FP + framesize;
	
	#if __fpu__
		/*	restore the saved nonvolatiles FPRs	*/

		if (has_fpu) {
			FPRs -= nfreg;
			for (i = 0; i < nfreg; i++)
				context->target.FPR[20+i] = *FPRs++;
		}
	#endif		

		/*	return the new return address	*/
	
	return(retaddr);
}


/*
 *	__SetupFrameInfo(context, info)
 *
 *	Decode the UnwindInfo pointed at by 'info->exception_record'. Using this
 *	and the information contained in 'context', setup any information needed to
 *	address local variables in the current frame. This information is only used
 *	by __LocalVariable() defined below.
 *
 *	Typically this involves setting 'context->FP' to 'context->SP' for functions
 *	which do not have a separate frame pointer, or to whichever general-purpose
 *	register is used as a frame pointer for functions which do require a separate
 *	frame pointer.
 *
 *	On most RISC machines, locals can be addressed via SP unless the function
 *	calls alloca() or contains a catch-block or exception-specification, in which
 *	case a separate frame pointer is used because SP may float while the function
 *	is executing.
 *
 *	It is convenient to retain the decoded UnwindInfo in the target-specific portion
 *	of the ThrowContext so it can be reused by __PopStackFrame() and other routines.
 *
 */

extern void __SetupFrameInfo(ThrowContext *context, ExceptionInfo *info)
{
	char *p = info->exception_record;
	int has_fpu = *p & UNWIND_HAS_FPU;

		/*	decode the UnwindInfo for the current function	*/
	
	context->target.has_frame_ptr = *p & UNWIND_HAS_FRAME_PTR;
	context->target.has_flushback = *p & UNWIND_HAS_FLUSHBACK;
	context->target.n_saved_GPRs = *p & UNWIND_USED_GPR_MASK;
	p = __DecodeUnsignedNumber(p + 1, &context->target.frame_size);
	p = __DecodeUnsignedNumber(p, &context->target.argument_size);
	
		/*	setup the frame pointer for addressing locals in the current function	*/
	
	if (context->target.has_frame_ptr) {
		context->FP = (char *) context->target.GPR[30];	//	s8 is used as the frame pointer
	}
	else {
		context->FP = context->SP;						//	SP is used as the frame pointer
	}
	
#if __fpu__
	if (has_fpu)
		p = __DecodeUnsignedNumber(p, &context->target.n_saved_FPRs);
	else 
		context->target.n_saved_FPRs = 0;
#endif
}


/*
 *	__RelocateExceptionTable	--	In PID mode, transform a gp relative offset into
 *	an absolute address.
 *
 */
 
typedef struct ImageExceptionTable {
	ExceptionTableIndex *ets;
	ExceptionTableIndex *ete;
} ImageExceptionTable;

#define MAX_IMAGE 10

ImageExceptionTable imgtable[MAX_IMAGE] = {0};

int _RegisterExceptionTables(void *ets, void *ete)
{
	int i;
	
	for (i = 0; i < MAX_IMAGE; i++) {
		if (imgtable[i].ets == 0) {
			imgtable[i].ets = (ExceptionTableIndex *)ets;
			imgtable[i].ete = (ExceptionTableIndex *)ete;
			imgtable[i].ete--;
			return i;
		}
	}
	return -1;
}


void _UnRegisterExceptionTables(int exix)
{
	imgtable[exix].ets = imgtable[exix].ete = 0;
}


/*
 *	__FindExceptionTable(info, retaddr)
 *
 *	Find the Exception Table Index that (possibly) contains 'retaddr'.
 *	The start and end of the index are stored in 'info->exception_table_start'
 *	and 'info->exception_table_end'.
 *
 *	If there is any target-specific ExceptionInfo it must be initialized here
 *	as well, e.g. base address of code/data sections.
 *
 *	For most targets there is only one exception table, usually accessed via
 *	globals generated by the linker, e.g. __exception_table_start__ and
 *	__exception_table_end__, so we just fill in those addresses and return TRUE.
 *
 *	If the target supports multiple fragments or overlays or for some other reason
 *	has more than one exception table index for a single program, we can use
 *	'retaddr' to find the appropriate index. If 'retaddr' doesn't correspond to
 *	any known exception table index then we must return FALSE.
 *
 *	Note that at this point 'retaddr' is an absolute return address, since we
 *	don't know until this routine finishes what it may be relative to.
 *
 */

#if __MIPS_WinCE__

int __FindExceptionTable(ExceptionInfo *info, char *retaddr)
{
	int i;
	
	for (i = 0; i < MAX_IMAGE; i++) {
		if (imgtable[i].ets) {
			if (imgtable[i].ets->function_address <= retaddr 
			&&  imgtable[i].ete->function_address + FUNCTION_SIZE(imgtable[i].ete) > retaddr) {				
				info->exception_table_start = imgtable[i].ets;
				info->exception_table_end = imgtable[i].ete;
				return 1;
			}
		}
	}
	return 0;
}

#else

int __FindExceptionTable(ExceptionInfo *info, char *) /* cc removed retaddr*/
{
	info->exception_table_start = (ExceptionTableIndex *)__exception_table_start__;
	info->exception_table_end = (ExceptionTableIndex *)__exception_table_end__;

	return(1);
}

#endif

/*
 *	__SkipUnwindInfo(exceptionrecord)
 *
 *	Given a pointer to an Exception Record, skip past the target-specific UnwindInfo
 *	to point at the Range Table. If the UnwindInfo is fixed-length we can implement
 *	this as a simple macro or inline function, but most targets will have variable-length
 *	UnwindInfo.
 *
 */

char *__SkipUnwindInfo(char *p)
{
	unsigned int dummy;
	int has_fpu = *p & UNWIND_HAS_FPU;
	
	p += 1;	/*	skip flag byte	*/
	p = __DecodeUnsignedNumber(p, &dummy);	/*	skip frame size	*/
	p = __DecodeUnsignedNumber(p, &dummy);	/*	skip argument size	*/
	if (has_fpu)
		p = __DecodeUnsignedNumber(p, &dummy);	/*	skip nb of fp register	*/
	return(p);
}


/*
 *	__TransferControl(info, context, address)
 *
 *	Transfer control back to the program at 'address'. This will always be either
 *	a catch-block or a call to __unexpected() supporting an exception specification.
 *
 *	'address' is of the same form as returned by __AdjustReturnAddress(), i.e.
 *	it is absolute if the exception table index contains absolute addresses, and
 *	relative if it contains relative addresses.
 *
 *	Using the information stored in the ThrowContext we restore all registers, and
 *	restore the stack pointer to its value in the function which called __throw()
 *	(discarding the exception-handling frames but leaving the thrower's frame intact)
 *
 *	Then we fixup the stack frame as needed (e.g. filling in any back-links, guaranteeing
 *	enough space for outgoing arguments, etc) and jump to the target address.
 *
 */

asm void __TransferControl(register ThrowContext *context, register ExceptionInfo *info, register char *address)
{
	.set	reorder
	//	restore the nonvolatile GPRs of the catcher
	
	_LOAD	s0, ThrowContext.target.GPR[16](a0)
	_LOAD	s1, ThrowContext.target.GPR[17](a0)
	_LOAD	s2, ThrowContext.target.GPR[18](a0)
	_LOAD	s3, ThrowContext.target.GPR[19](a0)
	_LOAD	s4, ThrowContext.target.GPR[20](a0)
	_LOAD	s5, ThrowContext.target.GPR[21](a0)
	_LOAD	s6, ThrowContext.target.GPR[22](a0)
	_LOAD	s7, ThrowContext.target.GPR[23](a0)
	_LOAD	fp, ThrowContext.target.GPR[30](a0)

#if __fpu__
	//	restore the nonvolatile FPRs of the catcher
	
	_LOADF	f20, ThrowContext.target.FPR[20](a0)
	_LOADF	f21, ThrowContext.target.FPR[21](a0)
	_LOADF	f22, ThrowContext.target.FPR[22](a0)
	_LOADF	f23, ThrowContext.target.FPR[23](a0)
	_LOADF	f24, ThrowContext.target.FPR[24](a0)
	_LOADF	f25, ThrowContext.target.FPR[25](a0)
	_LOADF	f26, ThrowContext.target.FPR[26](a0)
	_LOADF	f27, ThrowContext.target.FPR[27](a0)
	_LOADF	f28, ThrowContext.target.FPR[28](a0)
	_LOADF	f29, ThrowContext.target.FPR[29](a0)
	_LOADF	f30, ThrowContext.target.FPR[30](a0)
	_LOADF	f31, ThrowContext.target.FPR[31](a0)
#endif
	
	//	restore SP to stack top at throw: discards exception-handling frames
	//	but not exception temporaries!
	_LOADPTR	sp, ThrowContext.throwSP(a0)

	//	add enough space to the frame to accomodate any outgoing arguments
	//	used by the catcher; this is needed because the thrower may not
	//	have as large an argument area as the catcher, and we don't want the
	//	catcher to overwrite locals in the thrower (i.e. the exception object
	//	being thrown) if it makes any calls from the catch-block.
	lw		v0, ThrowContext.target.argument_size(a0)
	sub		sp, sp, v0

	//	transfer control to the catch-block
	
	#if __pc_relative__
		// In pc-relative mode, a2 represents a relative address.  We
		// have to transform it to an absolute address by adding s7
		_ADDPTR	a2,a2,s7
	#endif
	
	jr		a2
}


/*
 *	__throw(throwtype, location, dtor)
 *
 *	Throw an exception object at 'location' of type 'throwtype'. After catch-handling
 *	is complete, the object will be destroyed by calling destructor 'dtor'.
 *
 *	We capture the necessary target-independent (e.g. stack pointer in the thrower,
 *	return address into the thrower) and target-specific (e.g. the values of nonvolatile
 *	registers from the thrower) context information, and remember the throw-type,
 *	exception-object and its destructor, and then jump to the target-independent
 *	throw handler.
 *
 */

#define NO_EXCEPTIONS	0

#if NO_EXCEPTIONS
#include <stdio.h>
#include <exception.h>
void __throw(register char *throwtype, register void *location, register void *dtor)
{
	printf("*** Exception type '%s' thrown at 0x%08x ***\n", throwtype, location);
	terminate();
}
#else 

asm void __throw(register char *throwtype, register void *location, register void *dtor)
{
	.set reorder
	//	capture the stack pointer of the thrower, which we will
	//	need when we transfer control back to the catcher
	_MOVE	v0,sp
	//	allocate space for a local ThrowContext
	//	force the stack to be 16 byte aligned
	addiu	sp,-sizeof(ThrowContext)

	//	capture the nonvolatile GPRs of the thrower
	_STORE	s0, ThrowContext.target.GPR[16](sp)
	_STORE	s1, ThrowContext.target.GPR[17](sp)
	_STORE	s2, ThrowContext.target.GPR[18](sp)
	_STORE	s3, ThrowContext.target.GPR[19](sp)
	_STORE	s4, ThrowContext.target.GPR[20](sp)
	_STORE	s5, ThrowContext.target.GPR[21](sp)
	_STORE	s6, ThrowContext.target.GPR[22](sp)
	_STORE	s7, ThrowContext.target.GPR[23](sp)
	_STORE 	fp, ThrowContext.target.GPR[30](sp)
	
#if __fpu__
	//	capture the nonvolatile FPRs of the thrower
	
	_STOREF	f20, ThrowContext.target.FPR[20](sp)
	_STOREF	f21, ThrowContext.target.FPR[21](sp)
	_STOREF	f22, ThrowContext.target.FPR[22](sp)
	_STOREF	f23, ThrowContext.target.FPR[23](sp)
	_STOREF	f24, ThrowContext.target.FPR[24](sp)
	_STOREF	f25, ThrowContext.target.FPR[25](sp)
	_STOREF	f26, ThrowContext.target.FPR[26](sp)
	_STOREF	f27, ThrowContext.target.FPR[27](sp)
	_STOREF	f28, ThrowContext.target.FPR[28](sp)
	_STOREF	f29, ThrowContext.target.FPR[29](sp)
	_STOREF	f30, ThrowContext.target.FPR[30](sp)
	_STOREF	f31, ThrowContext.target.FPR[31](sp)
#endif

	//	throwcontext.SP = throwcontext.throwSP = <stack pointer of caller>;
	_STOREPTR	v0, ThrowContext.SP(sp)
	_STOREPTR	v0, ThrowContext.throwSP(sp)
	
	//	throwcontext.returnaddr = <return address into caller>;
	_STOREPTR	ra, ThrowContext.returnaddr(sp)
	
	//	remember the throw-type, location, and dtor
	_STOREPTR	a0, ThrowContext.throwtype(sp)
	_STOREPTR	a1, ThrowContext.location(sp)
	_STOREPTR	a2, ThrowContext.dtor(sp)
	
	//	call __ThrowHandler(&throwcontext)
	_MOVE	a0,sp
	_CALL	__ThrowHandler
	//	we will never return to here!
}
#endif

/* cc 991129 silenced unused argument warning */
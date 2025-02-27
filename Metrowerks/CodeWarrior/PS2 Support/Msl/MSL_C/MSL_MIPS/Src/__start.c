/*
 *	__start.c.c	- Startup Routine for the MIPS board
 *
 *	Copyright © 1995-1998 metrowerks inc. All Rights Reserved.
 *
 */

#include <__config.h>
#include <__rts_info_t__.h>		/* linker generated symbols	*/	
#include <__mips_eabi_init.h>
#include <CPlusLib.h>
 
extern void main(int argc, char **argv);

#ifdef __cplusplus
extern "C" {
#endif

	void __start(void);

#ifdef __cplusplus
}
#endif


/*
 * Optimize this file as much as we can.  This is safe.
 *
 */
#pragma optimization_level 4


/* 
 * __copy_memory	-- copy an area of memory.  This function is 
 * intended to copy ROM to RAM.  We assume that the memory is 
 * word aligned and source and destination do not overlap.
 * Note: we do not attempt to do double word copy on ISA3 machines, 
 * because some ISA3 machines do not support double word load (ld) 
 * from ROM.
 *
 */

static void __copy_memoryw(int *from, int len, int *to)
{
	register int t0,t1,t2,t3;
	while (len >= 4) {
		t0 = from[0];
		t1 = from[1];
		t2 = from[2];
		t3 = from[3];
		to[0] = t0;
		to[1] = t1;
		to[2] = t2;
		to[3] = t3;
		from += 4;
		to += 4;
		len -= 4;			
	}
	while (len > 0) {
		*to=*from;
		++from;
		++to;
		--len;
	}	
}


/*
 *	__ROMtoRAM	-- copy the ROM to the RAM.  
 *
 */
 
static void __ROMtoRAM(address ROM_ad, int ROM_size, 
                       address RAM_ad, int RAM_size)
{
	#pragma unused(RAM_size)

	/* If the ROM address and the RAM address are the same there is no	*/
	/* no need for a copy												*/

	if (ROM_ad != RAM_ad) {
		/* 
		 *	We assume that the address is 4 byte aligned and the size is
		 *	a multiple of 4 bytes.  Note: we do not attempt to do double
		 *	word copy on ISA3 machines, because some ISA3 machines do not
		 *	support double word load (ld) from ROM.
		 */
		__copy_memoryw(/* from 	=> */ (int *)ROM_ad, 
		               /* len	=> */ ROM_size/sizeof(int), 
		               /* to	=> */ (int *)RAM_ad);
	}
}


/*
 *	__getS7	--	returns the value of the S7 register.  
 *
 *	In PIC mode the S7 register contains the load address of the 
 *  executable section.
 *
 */
 
asm address __getS7(void)
{
	.set	reorder
	move	v0,s7
	jr		ra
}


/*
 *	__getGP	--	returns the value of the GP register.  
 *
 */
 
asm address __getGP(void)
{
	.set	reorder
	move	v0,gp
	jr		ra
}


/*
 *	__getSP	--	returns the value of the SP register.  
 *
 */
 
asm char * __getSP(void) 
{
	.set	reorder
	move	v0,sp
	jr		ra
}


/*
 *	__getFP	--	returns the value of the FP register.  
 *
 */
 
asm char * __getFP(void) 
{
	.set	reorder
	move	v0,fp
	jr		ra
}


/*
 * The purpose of this routine is:
 * 1- to create the first stack frame
 * 2- perform all sort of initializations such as ROM to RAM 
 *    copy/expansion, zero initialization
 * 3- perform checksum on the different sections
 * 4- perform load time fixup
 * 5- call the static initializers for C++
 * 6- transfer control to the main program
 *
 */
#include <__board.h>
static void __start_c(int argc, char **argv)
{
	/* 
	 *	Expand ROM to RAM
	 *
	 */

//	__ROMtoRAM(_fsdata_ROM, _sdata_ROM_size, _fsdata, _sdata_size);
//	__ROMtoRAM(_fdata_ROM,  _data_ROM_size,  _fdata,  _data_size);

#if 0
	{
		extern const char __ROMAddress[];
		extern const char __RAMAddress[];
		extern const char __RAMLength[];
		int destlen = (unsigned int)__RAMLength;
		int err = uncompress(__RAMAddress, &destlen, __ROMAddress+4, *(unsigned int *)__ROMAddress);
	}
#endif
	
	/* 
	 * 	Initialize the .bss and .sbss section to 0.  We  
	 * 	assume that the memory size is a multiple of the
	 *  register size for the given ISA level and that the 
	 *  memory is aligned for the given ISA level.
	 */

	__clear_memory((_REG_TYPE *)_fsbss, _esbss - _fsbss);
	__clear_memory((_REG_TYPE *)_fbss,  _ebss - _fbss);
	
	
	#if __pc_relative__ || __gp_relative__
	/* 
	 * 	Transfrom relative addresses into absolute addresses
	 * 	This call is required only in GP/PC-relative mode.
	 *
	 */

	__fixup_code_reference();
	#endif	
	

	/* 
	 *	Initialize the C++ runtime system.
	 *	Note: the parameters __exception_table_start__, and 
	 *  __exception_table_end__ maybe 0 if exceptions are not used.
	 *
	 *	The call to __initialize_cpp_rts can be removed if C++ 
	 *	runtime support is not used at all in the program (no 
	 *  exceptions and no static initializers).
	 */

	__initialize_cpp_rts(
		/* Static Initializers		=>	*/ 	__static_init,
		/* Static Initializers end	=>	*/	__static_init_end,
		/* Exception table start	=>	*/	__exception_table_start__,
		/* Exception table end		=>	*/	__exception_table_end__	
		);


	/* 
	 *	Transfer control to main
	 *
	 */

	main(argc, argv);

	
 	/* 
 	 *	calls __destroy_global_chain() to call destructors
 	 * 	for the registered objects, in the reverse order
 	 * 	of their construction.
 	 *
	 *	The call to __destroy_global_chain can be removed if the 
	 *	program does not terminate, or does not rely on the execution
	 *	of destructors.
 	 */

	__destroy_global_chain();
}



asm void __start(void) 
{
	move	a0,zero
	move	a1,zero
	
	move	s0,a0	// Save argc and argv if needed
	move	s1,a1
	
	#if __pc_relative__
		/*
		 *	In PC-relative mode we save the load address of 
		 *  the process into S7.  This code is very much target
		 *  dependent.  It is likely that the loader will pass
		 *  the load address as an in parameter of the __start
		 *  routine.
		 *
		 *  In this case __loadaddress is defined in the PIC
		 *  linker command file.
		 *
		 */
		
		la		s7,__loadaddress
	#endif	/*	__pc_relative__	*/

	.set	reorder

	/* Set the global pointer.  _gp is know by the 			*/
	/* linker												*/
	
	#if __gp_relative__
		/* gp must be preinitialized to 0, because 'la' is 		*/
		/* going to use gp to compute the address of _final_gp	*/	
		/* We have to add 32K when setting GP because the 'la'	*/
		/* contains the value of _final_gp biased by 32K.  This	*/
		/* bizarre situation happens only when loading GP		*/
		li		gp,0
		la		gp,_final_gp
		
	#else 	
		la		gp,_gp
	#endif	/*	__gp_relative__	*/


	/*
	 *	Initialize sp and create an initial stack frame.  Force
	 *  the stack to be at least 16 byte aligned.
	 *
	 */
	 
	la		t9,_stack_addr
	addi	t9,-16
	li 		t8,~15
	and		t9,t8
	addi	sp,t9,-4*sizeof(_REG_TYPE)
	

	/*
	 *	board-level initialization
	 */
	 
	_CALL	__init_hardware


	/* 
	 *	Initialize the Status register
	 *
	 */
	 
	mfc0	t8,$12				/* Register 12 of CP0		*/

	#if __fpu__
		/* 
		 * 	Set the CU1 bit to activate the floating point unit	
		 *
		 */
		li		t9,(1<<29)
		or		t8,t9
	
		#if __fpu64__ || (__MIPS_single_fpu__ && !__MIPS_double_fpu__)
			/* 
			 *	Activate the additional registers (FR bit)			
			 *
			 */
			li		t9,(1<<26)
			or		t8,t9
		#else
			/* 
			 * 	Make sure we have only 16 pairs of fpu regs.  Clear	
			 *	the FR bit of CR0(12)
			 *
			 */
			li		t9,~(1<<26)
			and		t8,t9		
		#endif	/*	__fpu64__	*/
	
		#if __MIPS_ISA4__
			/* 
			 *	Activate ISA4 instructions (CU3)						
			 *
			 */
			li		t9,(1<<31)
			or		t8,t9
		#endif	/*	__MIPS_ISA4__	*/	

	#else	/* No FPU	*/
		/* 
		 * 	Set the CU1 bit to deactivate the floating point unit	
		 *
		 */
		li		t9,~(1<<29)
		and		t8,t9
	#endif	/*	__fpu__	*/

	#if __ptr64__
		
		#error
		
	#else
		/*
		 *	Disable 64 bit addressing
		 *
		 */
		li		t9,~(1<<5)
		and		t8,t9		
	#endif	/*	__ptr64__	*/
	
	/*
	 *	Set the BEV bit (exception vector) to normal
	 *
	 */
	li		t9,~(1<<22)
	and		t8,t9 
	 
	mtc0	t8,$12

	#if __fpu__
		/* 
		 *	Initialize the floating point control register	
		 *
		 */
		li		t9,0x01000000	/* set the FS bit	*/
		ctc1	t9,$31		
	#endif	/*	__fpu__	*/


	/* 
	 * Additional user-defined pre-main initialization
	 */

	_CALL	__init_user


	/* 
	 *	Call the C program	
	 *
	 */
	 
	move	a0,s0				/* pass argc and argv		*/
	move	a1,s1				/* if any					*/
	_CALL	__start_c
	
	
	/* 
	 *	Reset the board	
	 *
	 */
	
	_CALL	__MIPS_Exit		/* No return from here	*/
	
}


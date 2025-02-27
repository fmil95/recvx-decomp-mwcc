/***************************************************************************/
/*

FILE
	__mips_eabi_init.c

DESCRIPTION

	Use this file for C.
	
	Interface for board-level initialization and user-level initialization.
	
	If hardware initialization and pre-main user initialization are required,
	copy this file to your project directory and customize it (instead of
	customizing __start.c).
	
	Note that __init_hardware should not write on the stack until the
	memory controller is properly configured.

	
	void __init_hardware(void)
	
		Initialize the hardware, including the memory controller.
	
	void __init_user(void)
	
		Allow the user to perform initialization before calling main().

	void _ExitProcess(void)
 		
 		This function simply stalls the debugger.  You may want to rewrite this
 		function if you are using an OS.
	
 	abort and exit
 
 	In order to correctly implement the required startup/termination sequence for
 	C and C++ programs, we need to have an exit() routine that can be called by
 	the program startup code. The exit() routine is supposed to
 
 		(1)	call any functions registered via atexit()
 		(2) call destructors for any global objects
 		(3)	flush any unwritten buffers, close any open files, etc.
 		(4) terminates the program
 
 	We don't, however, want to require the ANSI C library for every CodeWarrior
 	program, since it drags in lots of code that may not be needed.
 
 	Instead we provide a dummy exit() function which simply calls the destructors
 	and terminates the program. We assume that any program which uses atexit()
 	or <stdio.h> and which requires those cleanup behaviors will have linked with
 	the ANSI C library, whose definition of exit() will override the one here.
 
 	We similarly define a dummy abort() function (which is called by the default
 	terminate() handler).
 
 	Programs which rely on the proper ANSI C/C++ behavior must use the ANSI C
 	library, and order it in the CodeWarrior project or command-line so that
 	its definitions supersede these definitions in the runtime support library.

	checksum
	
		 Verify the checksum of a given section.
	
COPYRIGHT	
	(c) 1997 Metrowerks Corporation
	All rights reserved.

HISTORY
	97 AUG 18 SCM	Adapted from PPC EABI version.
	97 SEP 15 SCM	Moved checksum function from __start.c to
					this file so that it can be easily customized.

*/
/***************************************************************************/

#include <__board.h>
#include <__config.h>
#include <__mips_eabi_init.h>

asm void __init_hardware(void)
{
	.set noreorder
	
	/*
	 *	Initialize IDT 79S381 board unless running with MWDebug.
	 *	Uncomment the initialization below if running standalone.
	 *	You may need to perform other initializations.
	 */
	
	/*
	 *	When customizing, be aware that the memory controller may not be
	 *	configured.
	 */

//	move	$s0, $ra				/* save off return address in NV reg */
//	jal		IDTInit					/* init board hardware */
//	nop

//	move	$ra, $s0				/* get saved return address */

	jr		$ra
	nop
}

void __init_user(void)
{
}

__declspec(weak) void abort(void)
{
	_ExitProcess();
}


__declspec(weak) void __MIPS_Exit(int status)
{
	#pragma unused(status)
	_ExitProcess();
}


/*
 *	_ExitProcess
 *
 *	MIPS EABI Runtime termination
 */
asm void _ExitProcess(void)
{
#if __IDT_SIM__
	/* Reset the board										*/
	_CALL	__reset /* No return from here!					*/
	nop
#else
	breakc	0xfffff	/* get the debugger's attention			*/
					/* -1 code indicates end-of-program		*/
#endif
}



/* 
 * __clear_memory	-- Set an area of memory to 0.  We assume that 
 * the memory is at least word aligned and its size is a multiple 
 * of _REG_TYPE.
 *
 */
 
#define CLEAR1WORD(mem,i)	mem[i]=0;
#define CLEAR2WORDS(mem,i)	CLEAR1WORD(mem,i);  CLEAR1WORD(mem,i+1);
#define CLEAR4WORDS(mem)	CLEAR2WORDS(mem,0); CLEAR2WORDS(mem,2);

void __clear_memory(_REG_TYPE *mem, int len)
{
	char *cmem;
	
	while (len >= (4 * sizeof(_REG_TYPE))) {
		CLEAR4WORDS(mem);
		mem += 4;
		len -= 4 * sizeof(_REG_TYPE);			
	}
	cmem = (char *)mem;
	while (len > 0) {
		*cmem=0;
		++cmem;
		--len;
	}	
}



/*
 * adler32 	-- compute the Adler-32 checksum of a data stream
 * Original version from Mark Adler, modified by MW.
 *
 */

#define BASE 65521L /* largest prime smaller than 65536 */
#define NMAX 5552	/* NMAX is the largest n such that 255n(n+1)/2 + (n+1)(BASE-1) <= 2^32-1 */

#define DO1(buf,i)  {s1 += buf[i]; s2 += s1;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf)    DO4(buf,0); DO4(buf,4);

static unsigned long __adler32(const address buf, unsigned long len)
{
    unsigned long s1 = 0;
    unsigned long s2 = 0;
    int k;
	unsigned char *bufptr = (unsigned char *)buf;
	
    if (buf == 0) return 1L;

    while (len > 0) {
        k = len < NMAX ? len : NMAX;
        len -= k;
        while (k >= 8) {
            DO8(bufptr);
	    	bufptr += 8;
            k -= 8;
        }
        if (k != 0) do {
            s1 += (*bufptr++);
	    	s2 += s1;
        } while (--k);
        s1 %= BASE;
        s2 %= BASE;
    }
    return (s2 << 16) | s1;
}


/*
 * Verify the checksum of a given section.  The last 4 bytes of 
 * each section is the expected checksum.
 *
 */
 
void __checksum(const char *name, const address mem, unsigned long size)
{
	unsigned long ad;
	unsigned long *ck=(unsigned long *)(mem+size-4);
	
	ad=__adler32(mem, size-4);
	if (*ck!=ad) {

		/*
		** The checksum for a section is bad.  Report or handle
		** the error here.
		*/

#if __IDT_SIM__

		__puts("Checksum error for: ");
		__puts(name);
		__putchar('\n');
		
#endif
		
	}
}


#pragma mark -
#pragma mark === PIC/PID Support ===
/*
 *	__fixup_code_reference	-- transform a relative address into an 
 *	absolute one by adding the load address to the relative address.
 *
 *	In PIC or PID mode it is possible to generate data that must be
 *  relocated at load time in order to execute properly.  An example 
 *  would be something like:
 *
 *		int	i;
 *		int	*j = &i;
 *
 *	In this case the compiler will create in the .sdata section an 
 *	entry for j that contains the offset of i in the data segment.  
 *	This memory location has to be relocated at load time with the 
 *	address of the data section.  A similar situation happens with 
 *	code address.
 *
 *	To solve this problem the linker generates a table of addresses
 * 	indicating where to apply the load time relocation.  The format
 *	of the table is as follow:
 *
 *			...		
 *			list of pc-relative relocations
 *			...
 *			0xffffffff	<== end of list marker
 *			...		
 *			list of gp-relative relocations
 *			...
 *			0xffffffff	<== end of list marker
 *
 *	The addresses being relocated at runtime are always word 
 * 	addresses therefore the last two bits of the addresses are 
 *	always 0.  We use this free space to code some information
 *  we need to pass around.  We set the last bit to indicate what
 *  is the type of the address we have to relocate.
 *
 */
#if __pc_relative__ || __gp_relative__
void __fixup_code_reference() 
{
	register address *memloc, *memlocend;
	register address codeaddress, dataaddress;
	register address ptr;
	register int kind;
	
		
		/*	Set the codeaddress to the base address of the 	*/
		/*  code section.  Set it to 0 if we are not in PC	*/
		/* 	relative mode.									*/
		
	codeaddress = __pc_relative__ ? __getS7() : 0;
	
	
		/*	Set the dataaddress to the base address of the 	*/
		/*  data section.  Set it to 0 if we are not in GP	*/
		/* 	relative mode.									*/
		/*  Since gp points 32K past the beginning of the 	*/
		/*  data section, we can conclude that the load 	*/
		/*	address is gp - 32K								*/
		
	dataaddress = __gp_relative__ ? __getGP() - 0x8000 : 0;
	
		
		/*	pc relative (S7) relocations	*/

	memloc = (address *)__dynrelopic;
	memlocend = (address *)__dynrelopic_end;
	
	while (memloc <  memlocend) {
		ptr = *memloc;
		
		/* Adjust the address where the relocation is taking place	*/
		kind = ptr & __ADDRESS_MASK_TYPE;
		ptr &= ~__ADDRESS_MASK_TYPE;

		if 		(kind == __ADDRESS_GP_RELATIVE)	ptr += dataaddress;
		else if (kind == __ADDRESS_PC_RELATIVE)	ptr += codeaddress;
		else if (kind == __ADDRESS_ABSOLUTE)		;
		
		*(address *)ptr += codeaddress;
		++memloc;
	}
	++memloc;
	
	
		/*	gp relative relocations	*/
	
	memloc = (address *)__dynrelopid;
	memlocend = (address *)__dynrelopid_end;

	while (memloc < memlocend) {
		ptr = *memloc;

		/* Adjust the address where the relocation is taking place	*/
		kind = ptr & __ADDRESS_MASK_TYPE;
		ptr &= ~__ADDRESS_MASK_TYPE;

		if 		(kind == __ADDRESS_GP_RELATIVE)	ptr += dataaddress;
		else if (kind == __ADDRESS_PC_RELATIVE)	ptr += codeaddress;
		else if (kind == __ADDRESS_ABSOLUTE)		;
			
		*(address *)ptr += dataaddress;
		++memloc;
	}
}
#endif



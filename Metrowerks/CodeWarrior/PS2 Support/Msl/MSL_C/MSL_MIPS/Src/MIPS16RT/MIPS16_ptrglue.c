#pragma MIPS16 off
#include <MIPS16_ptrglue.h>

extern char _ftext16[];			/* 1st location of .text16	*/
extern char _etext16[];			/* 1st location after .text16	*/
extern char _ftext[];			/* 1st location of .text	*/
extern char _etext[];			/* 1st location after .text	*/

EXTERN asm int __MIPS16_prtglue16();
EXTERN asm int __MIPS16_prtglue32();

/* These functions are equivalent now.  Difference will matter
   when we can have MIPS16 ASM functions */
EXTERN asm int __MIPS16_prtglue16()
{
		.set reorder
	la 		t6,_ftext16
	sltu	t8,v0,t6
	bnez	t8,dest32
	la		t6,_etext16
	sltu	t8,v0,t6
	beqz	t8,dest32
	
dest16:
	// set bit 0 for 16-bit mode
	ori		v0,v0,1
	jr		v0
	
dest32:
	jr		v0
}

EXTERN asm int __MIPS16_prtglue32()
{
	
		.set reorder
	la 		t6,_ftext16
	sltu	t8,v0,t6
	bnez	t8,dest32
	la		t6,_etext16
	sltu	t8,v0,t6
	beqz	t8,dest32
	
dest16:
	// set bit 0 for 16-bit mode
	ori		v0,v0,1
	jr		v0
	
dest32:
	jr		v0
}
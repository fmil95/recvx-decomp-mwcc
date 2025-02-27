#pragma MIPS16 off
#include <MIPS16_divmod.h>

/*#include "__board.h"
#include "newdivmod.h"
EXTERN asm int _l_muls(int left, int right)
{
		.set reorder 
		move	t9,ra					
		xor		a2,a0,a1					
		bltz	a2,neg
		; same signs
		jal		_l_mulu
		jr		t9					
		; different signs
neg:
		jal		_l_mulu
		lui		a0,0
		sub		v0,a0,v0
		jr		t9
}
*/
/*
 * _u_mulu
 *
 * Entry:	a0/	left factor
 *			a1/	right factor
 * Exit:		
 *			a2/ product
 *			v0/ product 
 *
 * a0=Op1, a1=Op2, a2=p, a3=b#, t0=d, t3=m
 * invariant: m = 1<<b#
 * invariant: d = Op1<<b#
 * result: p = Op1*Op2
 */
/*
EXTERN asm int _l_mulu(int left, int right)
{
		.set reorder 
		lui		a2,0		; p=0
		addi	a3,a2,-1	; b#=-1
		addi	t3,a2,1		; m=1
		
body:
		addi	a3,a3,1	; b++
		and		t2,t3,a1		
		bnez	t2,iter
		nop
		sllv	t0,a0,a3
		addu 	a2,a2,t0

iter:
		sll		t3,t3,1	
		bnez	t3,body
    ; done - result is in a2 
		move	v0,a2
		jr		ra
}
											
EXTERN asm int _l_divs(int divisor, int dividend)
{
		.set reorder 
		move	t9,ra					
		xor		a0,a0,a1					
		bltz	a0,neg
		
		jal		_l_mods
		jr		t9					
		move	v0,v1		; delay slot			

		/* different signs */
/*neg:
		jal		_l_mods
		lui		a0,0
		sub		v0,a0,v1			 
		jr		t9
}

EXTERN asm int _l_mods(int divisor, int dividend)
{
		.set reorder 
		move	t8,ra
		bgtz	a0,neg
		lui		a2,0
		sub		a0,a2,a0
neg:
		bltz	a1,finish
		sub		a1,a2,a1
finish:
		jal		_l_modu       
		jr		t8
}

											
EXTERN asm int _l_divu(int divisor, int dividend)
{
		.set reorder 
		move	t9,ra
		jal		_l_modu
		move	v0,v1
		jr		t9					
}

/*
 * _l_modu
 *
 * Entry:	a0/	divisor  (numerator)
 *			a1/	dividend (denominator)
 * Exit:	a0/ remainder
 *			a1/ (unchanged)
 *			a2/ quotient
 *			v0/ remainder
 *			v1/ quotient
 *			
 * R0=Op1/n, a1=Op2, a2=q, a3=b#, t0=d
 * precondition: Op1>=Op2>0
 * invariant: d = Op2<<b#
 * result: Op1 = q*Op2 + n, n < Op2
 */
/*EXTERN asm int _l_modu(int divisor, int dividend)
{

		.set reorder 
		subu	a3,a0,a1		; check precondition
		bltz	a3,cont
		bgtz	a1,cont
		lui		v0,0
		jr		ra
;
// probably don't need to save		
;

		lui		a3,0			; for (d = Op2, b = 0; Op1 - d >= d; d <<= 1, b++)
		addi	a3,a3,-1
		
cont:	
		add		a3,a3,1			; loop computes b and d   
		sllv	t0,a1,a3
		subu	t3,a0,t0
		subu	t3,t3,t0
		bgez	t3,cont

		lui		t3,0
		addi	t3,t3,1			; constant 1 to compute (1<<b)
		lui		a2,0			; for (n = Op1, q = 0; n >= Op2; d >>= 1, b--)
cont1:
		subu	t2,a0,t0		; if (n >= d)
		bltz	t2,iter
		subu	a0,a0,t0		;	  n -= d,
		sllv	t4,t3,a3
		addu	a2,a2,t4 		;     q += (1<<b);

iter:	
		addi	a3,a3,-1		; b--
		srl		t0,t0,1			; d >>= 1
		subu	t1,a0,a1		; n >= Op2?
		bgez	t1,cont1
		move	v0,a0			; remainder
		move	v1,a2			; quotient
		jr		ra
}
*/

EXTERN unsigned int __MIPS16_l_mulu(unsigned int left, unsigned int right) 
{
	unsigned long result = 0;
	int i;
	
	for (i = 1; i <= right; i++) 
		result += left;
	return result;
}

EXTERN int __MIPS16_l_muls(int left, int right) {
	if (left < 0) {
		if (right < 0)
			return __MIPS16_l_mulu(-left, -right);
		else
			return -__MIPS16_l_mulu(-left, right);
	} else { 
		if (right < 0) 
			return -__MIPS16_l_mulu(left, -right);
		else
			return __MIPS16_l_mulu(left, right);
	}		
}

static int __MIPS16_l_divmodu(unsigned long left, unsigned long right, unsigned long *q, unsigned long *rem) 
{
	unsigned long result = 0;
    
 	for (result = 0; left >= right; result++) {
		left -= right;
    }
	*q = result;
	*rem = left;
}

EXTERN int __MIPS16_l_modu(unsigned int left, unsigned int right) {
	unsigned long result, dont_care;
	
	__MIPS16_l_divmodu(left, right, &dont_care, &result);
			
	return (result);
    
}



EXTERN int __MIPS16_l_divu(unsigned int left, unsigned int right) {
	unsigned long result, dont_care; 
	
	__MIPS16_l_divmodu(left, right, &result, &dont_care);
	return (result);
}


EXTERN int __MIPS16_l_divs(int left, int right) {
	unsigned long result; 
	int negate_result = 0;
	
	if (left < 0) {
		if (right < 0) {
			result = __MIPS16_l_divu(-left, -right);
		} else {
			result = __MIPS16_l_divu(-left, right);
			negate_result = 1;
		}
	} else { 
		if (right < 0) {
			result = __MIPS16_l_divu(left, -right);
			negate_result = 1;
		} else {
			result = __MIPS16_l_divu(left, right);
		}
	}		
	return (negate_result ? -result : result);
	
}


EXTERN int __MIPS16_l_mods(int left, int right) {
	unsigned long u_left = left, u_right = right; 
	unsigned long u_result;
	int result_negative  = 0;
	
	if (left < 0 ) { 
		u_left = (unsigned)-left;
		result_negative = 1;
	}
	if (right < 0 ) 
		u_right = (unsigned)-right;
	
	u_result = __MIPS16_l_modu(u_left, u_right);
	return (result_negative ? -u_result : u_result);
}
//	setjmp.c	-	setjmp() and longjmp() routines for Metrowerks C++ for MIPS
//
// Modified for MIPS Support 081597. 


#include <__config.h>
#include <setjmp.h>

/*	__setjmp	-	C setjmp() routine
Upon entry, register a0 contains the address of the __jmp_buf structure
*/



asm int __setjmp(register __jmp_buf* env)
{
	.set reorder
	_STORE	s0, __jmp_buf.GPR[0](a0)
	_STORE	s1, __jmp_buf.GPR[1](a0)
	_STORE	s2, __jmp_buf.GPR[2](a0)
	_STORE	s3, __jmp_buf.GPR[3](a0)
	_STORE	s4, __jmp_buf.GPR[4](a0)
	_STORE	s5, __jmp_buf.GPR[5](a0)
	_STORE	s6, __jmp_buf.GPR[6](a0)
	_STORE	s7, __jmp_buf.GPR[7](a0)
	_STORE	fp, __jmp_buf.GPR[8](a0)
	_STORE	sp, __jmp_buf.GPR[9](a0)
	_STORE 	ra, __jmp_buf.GPR[10](a0)

	#if __fpu__
	_STOREF	f20, __jmp_buf.FPR[0](a0)
	_STOREF	f21, __jmp_buf.FPR[1](a0)
	_STOREF	f22, __jmp_buf.FPR[2](a0)
	_STOREF	f23, __jmp_buf.FPR[3](a0)
	_STOREF	f24, __jmp_buf.FPR[4](a0)
	_STOREF	f25, __jmp_buf.FPR[5](a0)
	_STOREF	f26, __jmp_buf.FPR[6](a0)
	_STOREF	f27, __jmp_buf.FPR[7](a0)
	_STOREF	f28, __jmp_buf.FPR[8](a0)
	_STOREF	f29, __jmp_buf.FPR[9](a0)
	_STOREF	f30, __jmp_buf.FPR[10](a0)
	_STOREF	f31, __jmp_buf.FPR[11](a0)
	#endif
	
	li	v0,0
	jr	ra
}


//	longjmp		-	C longjmp() routine

asm void __longjmp(register __jmp_buf *env,int val)
{
	.set reorder
	_LOAD	s0, __jmp_buf.GPR[0](a0)
	_LOAD	s1, __jmp_buf.GPR[1](a0)
	_LOAD	s2, __jmp_buf.GPR[2](a0)
	_LOAD	s3, __jmp_buf.GPR[3](a0)
	_LOAD	s4, __jmp_buf.GPR[4](a0)
	_LOAD	s5, __jmp_buf.GPR[5](a0)
	_LOAD	s6, __jmp_buf.GPR[6](a0)
	_LOAD	s7, __jmp_buf.GPR[7](a0)
	_LOAD	fp, __jmp_buf.GPR[8](a0)
	_LOAD	sp, __jmp_buf.GPR[9](a0)
	_LOAD 	ra, __jmp_buf.GPR[10](a0)

	#if __fpu__
	_LOADF	f20, __jmp_buf.FPR[0](a0)
	_LOADF	f21, __jmp_buf.FPR[1](a0)
	_LOADF	f22, __jmp_buf.FPR[2](a0)
	_LOADF	f23, __jmp_buf.FPR[3](a0)
	_LOADF	f24, __jmp_buf.FPR[4](a0)
	_LOADF	f25, __jmp_buf.FPR[5](a0)
	_LOADF	f26, __jmp_buf.FPR[6](a0)
	_LOADF	f27, __jmp_buf.FPR[7](a0)
	_LOADF	f28, __jmp_buf.FPR[8](a0)
	_LOADF	f29, __jmp_buf.FPR[9](a0)
	_LOADF	f30, __jmp_buf.FPR[10](a0)
	_LOADF	f31, __jmp_buf.FPR[11](a0)
	#endif
	move	v0,a1
    bne     v0,0,ret_val_ok
    
	li		v0,1
ret_val_ok:	
	jr	ra
}

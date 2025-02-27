//	ptmf.c	-	pointer-to-member-function support for Metrowerks C++ for MIPS
//
//	Copyright © 1995 metrowerks inc.  All Rights Reserved.
//
//
//	THEORY OF OPERATION
//
//	A pointer-to-member-function (PTMF) is represented as a 3-word struct:
//
//		+-----------------------------+			+-----------------------------+
//		|		'this' delta		  |			|		'this' delta		  |
//		|-----------------------------| 		|-----------------------------|
//		|			-1				  |			|		 vtbl index 		  |
//		|-----------------------------| 		|-----------------------------|
//		|		function pointer	  |			|		 vptr offset		  |
//		+-----------------------------+			+-----------------------------+
//

#pragma internal on
#pragma options align=mips

#include <CPlusLib.h>

//
//	Private Types
//

#ifdef __cplusplus
extern "C" {
#endif

//
//	Prototypes
//

long __ptmf_test(PTMF *ptmf);
long __ptmf_cmpr(PTMF *ptmf1, PTMF *ptmf2);
void __ptmf_call(...);
void __ptmf_call4(...);
void __ptmf_scall(...);
void __ptmf_scall4(...);
PTMF *__ptmf_cast(long offset, const PTMF *ptmfrom, PTMF *ptmto);

#ifdef __cplusplus
}
#endif


//	__ptmf_test	-	test pointer-to-member-function for null
//
//	a0 contains the PTMF. If it is null, we return 0; else we return 1.
//

long __ptmf_test(register PTMF *ptmf)
{
	long l;

	l  = ptmf->this_delta;
	l |= ptmf->vtbl_offset;
	l |= ptmf->func_data.ventry_offset;
	
	return (l != 0);
}


//	__ptmf_cmpr	-	compare two pointer-to-member-functions
//
//	a0 and a1 contain the PTMFs. If equal, we return 0; else we return 1.
//

long __ptmf_cmpr(register PTMF *ptmf1, register PTMF *ptmf2)
{
	long l;

	l  = ptmf1->this_delta ^ ptmf2->this_delta;
	l |= ptmf1->vtbl_offset ^ ptmf2->vtbl_offset;
	l |= ptmf1->func_data.ventry_offset ^ ptmf2->func_data.ventry_offset;
	
	return (l != 0);
}


//	__ptmf_call		-	call pointer-to-member-function
//
//	t9 contains the PTMF. a0 contains 'this'.
//

asm void __ptmf_call(...)
{
		lw		at,PTMF.this_delta(t9)
		lw		t8,PTMF.vtbl_offset(t9)
		lw		t9,PTMF.func_data(t9)	//	function pointer if not virtual

		bltz	t8,L1
		addu	a0,a0,at				//	adjust 'this'
		
		addu	t9,a0,t9
		lw		t9,0(t9)				//	get vptr
		nop
		
		addu	t9,t9,t8				//	point to vtbl entry
		lw		at,4(t9)				//	get 'this' delta
		lw		t9,0(t9)				//	get function pointer
		addu	a0,a0,at				//	adjust 'this' again

L1:
		jr		t9
		nop
}


//	__ptmf_call4	-	call pointer-to-member-function
//
//	t9 contains the PTMF. a1 contains 'this'.
//

asm void __ptmf_call4(...)
{
		lw		at,PTMF.this_delta(t9)
		lw		t8,PTMF.vtbl_offset(t9)
		lw		t9,PTMF.func_data(t9)	//	function pointer if not virtual

		bltz	t8,L1
		addu	a1,a1,at				//	adjust 'this'
		
		addu	t9,a1,t9
		lw		t9,0(t9)				//	get vptr
		nop
		
		addu	t9,t9,t8				//	point to vtbl entry
		lw		at,4(t9)				//	get 'this' delta
		lw		t9,0(t9)				//	get function pointer
		addu	a1,a1,at				//	adjust 'this' again

L1:
		jr		t9
		nop
}


//	__ptmf_scall	-	call pointer-to-member-function
//
//	This is used when we know the class tree uses single inheritance only.
//	t9 contains the PTMF. a0 contains 'this'.
//
//

asm void __ptmf_scall(...)
{
		lw		at,PTMF.this_delta(t9)	//	(needed for thunks)
		lw		t8,PTMF.vtbl_offset(t9)
		lw		t9,PTMF.func_data(t9)	//	function pointer if not virtual

		bltz	t8,L1
		addu	a0,a0,at				//	adjust 'this' (needed for thunks)

		addu	t9,a0,t9
		lw		t9,0(t9)				//  get vptr
		nop
		
		addu	t9,t9,t8
		lw		t9,0(t9)				//	get function pointer
		nop

L1:
		jr		t9
		nop
}


//	__ptmf_scall4	-	call pointer-to-member-function
//
//	This is used when we know the class tree uses single inheritance only.
//	t9 contains the PTMF. a1 contains 'this'.
//
//

asm void __ptmf_scall4(...)
{
		lw		at,PTMF.this_delta(t9)		//	(needed for thunks)
		lw		t8,PTMF.vtbl_offset(t9)
		lw		t9,PTMF.func_data(t9)		//	function pointer if not virtual

		bltz	t8,L1
		addu	a1,a1,at					//	adjust 'this' (needed for thunks)

		addu	t9,a1,t9
		lw		t9,0(t9)					//  get vptr
		nop
		
		addu	t9,t9,t8
		lw		t9,0(t9)					//	get function pointer
		nop

L1:
		jr		t9
		nop
}


//
//	__ptmf_cast	-	copy/cast a pointer-to-member-function
//

/************************************************************************/
/* Purpose..: This function will copy/cast a pointer to func member		*/
/* Input....: offset delta to apply	to pointer to function member		*/
/* Input....: pointer to original pointer to function member			*/
/* Input....: pointer to destiniation pointer to function member		*/
/* Return...: pointer to destiniation pointer to function member		*/
/************************************************************************/
extern PTMF *__ptmf_cast(long offset,const PTMF *ptmfrom,PTMF *ptmto)
{
	ptmto->this_delta=ptmfrom->this_delta+offset;
	if(ptmfrom->vtbl_offset>=0L)
	{	//	virtual function pointer
		ptmto->vtbl_offset				= ptmfrom->vtbl_offset;
		ptmto->func_data.ventry_offset	= ptmfrom->func_data.ventry_offset;
	}
	else
	{	//	non-virtual function pointer
		ptmto->vtbl_offset			= -1L;
		ptmto->func_data.func_addr	= ptmfrom->func_data.func_addr;
	}
	return ptmto;
}


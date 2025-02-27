/*
 *	StaticInitializers.c	-	static initializer/destructor support for RISC targets
 *
 *	Copyright © 1995-1997 Metrowerks, Inc. All Rights Reserved.
 *
 *
 *	THEORY OF OPERATION
 *
 *	For any file that contains static/global objects that require runtime initialization
 *	or construction, the compiler generates a static initializer function of the form:
 *
 *		void __sinit_<filename>(void) { ... }
 *
 *	The linker is responsible for gathering these together and generating an array
 *	of pointers to them:
 *
 *		typedef void (*StaticInitializer)(void);
 *		extern StaticInitializer __sinit__[];
 *
 *	When the compiler generates code to call initializers for global variables and
 *	nonlocal static variables, it calls the function __register_global_object() to
 *	register the constructed object for destruction.
 *
 *	When the program exits, it calls __destroy_global_chain() to call destructors
 *	for the registered objects, in the reverse order of their construction.
 *
 */

#include <CPlusLib.h>
#include <NMWException.h>
#include <__config.h>
#include <__rts_info_t__.h>	/* linker generated symbols	*/	
#include <CPlusLib.h>


	/*	public data		*/

DestructorChain *__global_destructor_chain;


/*
 *	__call_static_initializers	-	call all static initializer functions
 *
 *	In PC-relative mode, the address in the table is an offset from the 
 *  beginning of the program.  The offset has to be transformed to an 
 *	absolute address before the call.
 *
 */

static void __call_static_initializers(void (**si)(void), void (**sie)(void))
{
	union {
		void (*sifunc)(void);
		address sifuncad;
	} u;
	const address offset = __pc_relative__ ? __getS7() : 0;
	
	for (; si < sie; si++) {
		u.sifunc = *si; 
		u.sifuncad += offset;
		(u.sifunc)();
	}
}


/*
 *	__initialize_cpp_rts	--	Initialize the C++ runtime 
 *	system at the beginning of the program
 *		si	:	pointer to the static initializer table
 *		sie	:	pointer to the end of the static initializer table
 *		ets	:	pointer to the beginning of the exception table
 *		ete	:	pointer to the end of the exception table
 *	
 */
 

void __initialize_cpp_rts(void (**si)(void), void (**sie)(void), void *, void *) /* cc removed ete & est*/
{
	__call_static_initializers(si, sie);
}


/*
 *	__initialize_cpp_rts_overlay	--	Initialize the C++ runtime 
 *	system at the beginning of an overlay
 *		si	:	pointer to the static initializer table
 *		ets	:	pointer to the beginning of the exception table
 *		ete	:	pointer to the end of the exception table
 *	
 */
 
extern void __initialize_cpp_rts_overlay(void (**si)(void), void (**sie)(void), void *, void *)  /* cc removed ete & est*/
{
	__call_static_initializers(si, sie);
}


/*
 *	__register_global_object	-	register a global object for later destruction
 *
 *	'regmem points to a global data object where the registration info will be stored.
 *	For convenience for the caller we return the object being registered.
 *
 */

void *__register_global_object(void *object, void *destructor, void *regmem)
{
	((DestructorChain *) regmem)->next = __global_destructor_chain;
	((DestructorChain *) regmem)->destructor = destructor;
	((DestructorChain *) regmem)->object = object;
	__global_destructor_chain = (DestructorChain *) regmem;

	return(object);
}


/*
 *	__destroy_global_chain	-	destroy all constructed global objects
 *
 */

void __destroy_global_chain(void)
{
	DestructorChain	*gdc;

	while((gdc = __global_destructor_chain) != 0) {
		__global_destructor_chain = gdc->next;
		DTORCALL_COMPLETE(gdc->destructor, gdc->object);
	}
}


extern "C" PTMF __ptmf_null = { 0, 0, 0 };

#if __MIPS_WinCE__

// Static initializer marker symbols

#pragma align (4)

#pragma data_seg(".CRT$XCA")
	void  (*__static_init[1])(void) = { (void (*)(void))0 };

#pragma data_seg(".CRT$XCZ")
	void  (*__static_end[1])(void) = { (void (*)(void))0 };



// Local Exception info marker symbols

#pragma data_seg(".exix$a")
	void  (*__exception_table_start__[1])(void) = { (void (*)(void))0 };

#pragma data_seg(".exix$z")
	void  (*__exception_table_end__[1])(void) = { (void (*)(void))0 };


#endif

/* cc 991129 silenced unused argument warning */
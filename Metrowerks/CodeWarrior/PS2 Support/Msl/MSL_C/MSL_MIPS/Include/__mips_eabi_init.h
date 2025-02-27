/***************************************************************************/
/*

FILE
	__mips_eabi_init.h

DESCRIPTION

	Interface for board-level initialization and user-level initialization.
	
	__start
		(registers initialized)
		__init_hardware called
		(data initialized, .data/.bss/.sdata/.sbss...)
		__init_cpp
		(exceptions initialized and static constructors called)
		__init_user
		main
	
	Define __init_hardware and __init_user or use the default stub functions.
	
	Note: __init_hardware should be written so as to not reference memory
	i.e., it should not be written in C or allocate a stackframe.
		
COPYRIGHT	
	(c) 1997 Metrowerks Corporation
	All rights reserved.

HISTORY
	97 AUG 18 SCM	Adapted from PPC EABI version.

*/
/***************************************************************************/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

extern void __init_hardware(void);
extern void __init_user(void);
extern void abort(void);
extern void __MIPS_Exit( int );
extern void _ExitProcess(void);													
extern void __checksum(const char *name, const address mem, unsigned long size);
extern void __clear_memory(_REG_TYPE *mem, int len);
extern void __fixup_code_reference();

extern void IDTInit(void);

#ifdef __cplusplus
}
#endif


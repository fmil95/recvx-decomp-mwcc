/*
 *	ansi_prefix.MIPS_bare.h
 *	
 *		Copyright � 1997 Metrowerks, Inc.
 *		All rights reserved.
 */

#ifndef __ansi_prefix__
#define __ansi_prefix__

#include <__config.h>
#include <ansi_parms.h>


#if __MIPS_WinCE__

	#define __dest_os	__wince_os

#elif __MIPS__

	#define __dest_os	__mips_bare

	#define __IDT79S381__	(1)	/* Board type					*/
	#define __IDT79S464__	(0) /* Board type					*/

	#define __IDT_SIM__		(1) /* Running under IDT sim7 		*/
	#define __PMON__		(0)	/* PMOM board					*/

#else

	#error Unknown OS

#endif


	/* Even with floating point emulation, certain chips, such	*/
	/* as the 821, do not have floating point registers.  		*/
	/* Comment out the definition of _No_Floating_Point_Regs	*/
	/* if your chip does have a floating point unit.			*/
/*#define _No_Floating_Point_Regs*/

	/* the following are OS services that aren't available 		*/
#define _No_Time_OS_Support
#define _No_Alloc_OS_Support
#define _No_Disk_File_OS_Support

	/* uncomment _No_Console if you do not want to	*/
	/* write and read to a console window.  		*/
//#define _No_Console
#ifndef _No_Console
	/* the serial 1 and 2 UARTlibs have unbuffered	*/
	/* IO; comment out the following line if  		*/
	/* you are either not using either the serial 1 */
	/* or 2 UARTlibs or if your OS has buffered IO.	*/
#define _Unbuffered_Console
#endif

#define TARGET_IEEE754
#define TARGET_HEAP_ALIGNMENT	8

#define NEWMODE NEWMODE_MALLOC		//	always use malloc for new

#endif

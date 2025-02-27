/*
 *	__board.h
 *	
 *		Copyright © 1998 Metrowerks, Inc.
 *		All rights reserved.
 *	
 */


#ifdef __cplusplus
	#define EXTERN	extern "C"
#else
	#define EXTERN extern
#endif


/* Board configuration										*/

#if __IDT_SIM__

	EXTERN void __reset   (void);
	EXTERN void __putchar (char c);
	EXTERN void __puts    (const char *s);
	EXTERN void __printf  (char *format, ...);
	EXTERN void timer_start();
	EXTERN unsigned int timer_stop();
	
	#include "IDT_config.h"

#elif __PMON__

	EXTERN void __putchar (char c);
	EXTERN void __puts    (const char *s);
	EXTERN void __printf  (char *format, ...);
	
	#include "PMON_config.h"

#else
/*#error	"No board selected" */
#endif

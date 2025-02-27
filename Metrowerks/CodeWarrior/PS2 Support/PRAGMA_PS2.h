/*	===========================================================================
 *	PRAGMA_PS2.h			©1999-2000 Metrowerks Inc. All rights reserved.
 *	===========================================================================
 *
 *	02/15/2000	kashima,	add alignlabel option
 *	10/26/1999	kashima,	add ".indata" section definition
 *	09/30/1999	kashima
 */

#if	__option(ANSI_strict)
#pragma	ANSI_strict	off	/*	force off	*/
#endif

/*	c++ support	*/
#ifdef	__cplusplus
#pragma	wchar_type	on	/*	force on	*/
#endif

/*	gcc compatibility	*/
#pragma gcc_extensions		on	/*	to support initializing data with variable	*/

/*	section	definition	*/
#pragma define_section indata ".indata" far_absolute RW

/*  Metrowerks Standard Library  */

/*  $Date: 1999/11/18 23:40:19 $ 
 *  $Revision: 1.9 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	new.h        hh 971206 Changed file name from new
 */

#ifndef _NEW_H
#define _NEW_H

#include <new>

#ifndef _MSL_NO_CPP_NAMESPACE
	using std::bad_alloc;
	using std::nothrow_t;
	using std::nothrow;
	using std::new_handler;
	using std::set_new_handler;
#endif

#endif

/*     Change record
hh 971206  Changed filename from new to new.h and added namespace support
// hh 990120 changed name of MSIPL flags
// hh 991113 modified using policy
*/

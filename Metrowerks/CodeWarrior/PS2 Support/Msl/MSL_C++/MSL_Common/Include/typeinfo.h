/*  Metrowerks Standard Library  */

/*  $Date: 1999/11/18 23:44:37 $ 
 *  $Revision: 1.9 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	typeinfo.h
 */

#ifndef _TYPEINFO_H
#define _TYPEINFO_H

#include <typeinfo>

#ifndef _MSL_NO_CPP_NAMESPACE
	using std::type_info;
	using std::bad_cast;
	using std::bad_typeid;
#endif

#endif

/*     Change record
hh 971206  Added namespace support
hh 990120 changed name of MSIPL flags
hh 991113 modified using policy
*/

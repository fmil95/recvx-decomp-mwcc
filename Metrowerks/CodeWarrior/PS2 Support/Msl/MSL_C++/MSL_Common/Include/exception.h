/*  Metrowerks Standard Library  */

/*  $Date: 1999/11/18 23:35:13 $ 
 *  $Revision: 1.9 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/****************************************************************************/
/*	Project...:	Standard C++ Library										*/
/*	Name......:	exception.h													*/
/*	Purpose...:	exception handling											*/
/****************************************************************************/

#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <exception>

#ifndef _MSL_NO_CPP_NAMESPACE
	using std::exception;
	using std::bad_exception;
	using std::unexpected_handler;
	using std::set_unexpected;
	using std::unexpected;
//	using std::uncaught_exception;
	using std::terminate_handler;
	using std::set_terminate;
	using std::terminate;
#endif

#endif

/*     Change record
hh 971206  Added namespace support
hh 990120 changed name of MSIPL flags
hh 991112 modified using policy
*/

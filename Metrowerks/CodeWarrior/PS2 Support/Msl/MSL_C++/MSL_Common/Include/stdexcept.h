/*  Metrowerks Standard Library  */

/*  $Date: 1999/11/18 23:42:39 $ 
 *  $Revision: 1.9 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	stdexcept.h
 */

#ifndef _STDEXCEPT_H
#define _STDEXCEPT_H

#include <stdexcept>

#ifndef _MSL_NO_CPP_NAMESPACE
	using std::logic_error;
	using std::domain_error;
	using std::invalid_argument;
	using std::length_error;
	using std::out_of_range;
	using std::runtime_error;
	using std::range_error;
	using std::overflow_error;
	using std::underflow_error;
#endif

#endif

/*     Change record
hh 971206  File created.
hh 990120 changed name of MSIPL flags
hh 991113 modified using policy
*/

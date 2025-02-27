/*  Metrowerks Standard Library  */

/*  $Date: 1999/11/18 22:48:39 $ 
 *  $Revision: 1.1 $ 
 *  $NoKeywords: $ 
 *
 *		Portions Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */
 
/**
 **  stringfwd.h
 **/

#ifndef _STRINGFWD_H
#define _STRINGFWD_H

#include <stringfwd>

#ifndef _MSL_NO_CPP_NAMESPACE

	using std::allocator;
	using std::char_traits;
	using std::basic_string;
	using std::string;
#ifndef _MSL_NO_WCHART
	using std::wstring;
#endif

#endif

#endif // _STRINGFWD_H

// hh 991114 Created

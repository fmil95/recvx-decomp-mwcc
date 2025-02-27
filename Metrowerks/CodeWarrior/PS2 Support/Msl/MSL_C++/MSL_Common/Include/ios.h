/*  Metrowerks Standard Library  */

/*  $Date: 1999/11/18 23:37:38 $ 
 *  $Revision: 1.9 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	ios.h            // hh 971207 Changed filename from ios to ios.h
 *	
 */

#ifndef _IOS_H             // hh 971207 added standard include guards
#define _IOS_H

#include <ios>

#ifndef _MSL_NO_CPP_NAMESPACE       // hh 971207 Added backward compatibility
#ifndef _MSL_NO_IO
	using std::streamoff;
	using std::streamsize;
	using std::fpos;
	using std::ios_base;
	using std::basic_ios;
	using std::ios;
#ifndef _MSL_NO_WCHART
	using std::wios;
#endif
	using std::boolalpha;
	using std::noboolalpha;
	using std::showbase;
	using std::noshowbase;
	using std::showpoint;
	using std::noshowpoint;
	using std::showpos;
	using std::noshowpos;
	using std::skipws;
	using std::noskipws;
	using std::uppercase;
	using std::nouppercase;
	using std::internal;
	using std::left;
	using std::right;
	using std::dec;
	using std::hex;
	using std::oct;
	using std::fixed;
	using std::scientific;
#endif
#endif

#endif
/*     Change record
// hh 971207 Changed filename from ios to ios.h
// hh 971207 added standard include guards
// hh 971207 Added backward compatibility
// hh 990120 changed name of MSIPL flags
// hh 991113 modified using policy
*/

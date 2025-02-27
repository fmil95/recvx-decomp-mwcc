/*  Metrowerks Standard Library  */

/*  $Date: 1999/11/18 23:42:31 $ 
 *  $Revision: 1.9 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	sstream.h             // hh 971207 Changed filename from sstream to sstream.h
 */

#ifndef _SSTREAM_H              // hh 971207 added standard include guards
#define _SSTREAM_H

#include <sstream>

#ifndef _MSL_NO_CPP_NAMESPACE            // hh 971207 Added backward compatibility
#ifndef _MSL_NO_IO
	using std::basic_stringbuf;
	using std::stringbuf;
	using std::basic_istringstream;
	using std::istringstream;
	using std::basic_ostringstream;
	using std::ostringstream;
	using std::basic_stringstream;
	using std::stringstream;
#ifndef _MSL_NO_WCHART
	using std::wstringbuf;
	using std::wistringstream;
	using std::wostringstream;
	using std::wstringstream;
#endif
#endif
#endif

#endif
/*     Change record
// hh 971207 Changed filename from sstream to sstream.h
// hh 971207 added standard include guards
// hh 971207 Added backward compatibility
// hh 990120 changed name of MSIPL flags
// hh 991113 modified using policy
*/

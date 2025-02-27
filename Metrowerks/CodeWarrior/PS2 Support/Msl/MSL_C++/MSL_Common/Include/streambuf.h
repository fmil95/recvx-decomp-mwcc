/*  Metrowerks Standard Library  */

/*  $Date: 1999/11/18 23:42:42 $ 
 *  $Revision: 1.9 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	streambuf.h           // hh 971207 Changed filename from streambuf to streambuf.h
 */

#ifndef _STREAMBUF_H              // hh 971207 added standard include guards
#define _STREAMBUF_H

#include <streambuf>

#ifndef _MSL_NO_CPP_NAMESPACE      // hh 971207 Added backward compatibility
#ifndef _MSL_NO_IO
	using std::basic_streambuf;
	using std::streambuf;
#ifndef _MSL_NO_WCHART
	using std::wstreambuf;
#endif
#endif
#endif

#endif
/*     Change record
// hh 971207 Changed filename from streambuf to streambuf.h
// hh 971207 added standard include guards
// hh 971207 Added backward compatibility
// hh 990120 changed name of MSIPL flags
// hh 991113 modified using policy
*/

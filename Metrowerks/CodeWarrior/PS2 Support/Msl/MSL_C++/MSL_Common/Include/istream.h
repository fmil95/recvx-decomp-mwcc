/*  Metrowerks Standard Library  */

/*  $Date: 1999/11/18 23:39:03 $ 
 *  $Revision: 1.9 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	istream.h               // hh 971207 Changed filename from istream to istream.h
 */

#ifndef _ISTREAM_H             // hh 971207 added standard include guards
#define _ISTREAM_H

#include <istream>

#ifndef _MSL_NO_CPP_NAMESPACE        // hh 971207 Added backward compatibility
#ifndef _MSL_NO_IO
	using std::basic_istream;
	using std::istream;
	using std::basic_iostream;
	using std::iostream;
#ifndef _MSL_NO_WCHART
	using std::wistream;
	using std::wiostream;
	using std::ws;
#endif
#endif
#endif

#endif
/*     Change record
// hh 971207 Changed filename from istream to istream.h
// hh 971207 added standard include guards
// hh 971207 Added backward compatibility
// hh 990120 changed name of MSIPL flags
// hh 991113 modified using policy
*/

/*  Metrowerks Standard Library  */

/*  $Date: 1999/11/18 23:41:42 $ 
 *  $Revision: 1.9 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright � 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	ostream.h          // hh 971207 Changed filename from ostream to ostream.h
 */

#ifndef _OSTREAM_H           // hh 971207 added standard include guards
#define _OSTREAM_H

#include <ostream>

#ifndef _MSL_NO_CPP_NAMESPACE          // hh 971207 Added backward compatibility
#ifndef _MSL_NO_IO
	using std::basic_ostream;
	using std::ostream;
#ifndef _MSL_NO_WCHART
	using std::wostream;
#endif
	using std::endl;
	using std::ends;
	using std::flush;
#endif
#endif

#endif
/*     Change record
// hh 971207 Changed filename from ostream to ostream.h
// hh 971207 added standard include guards
// hh 971207 Added backward compatibility
// hh 990120 changed name of MSIPL flags
// hh 991113 modified using policy
*/

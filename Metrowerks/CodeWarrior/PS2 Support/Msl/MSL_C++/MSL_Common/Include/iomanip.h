/*  Metrowerks Standard Library  */

/*  $Date: 1999/11/18 23:37:35 $ 
 *  $Revision: 1.9 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright � 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	iomanip.h         // hh 971207 Changed filename from iomanip to iomanip.h
 */

#ifndef _IOMANIP_H              // hh 971207 added standard include guards
#define _IOMANIP_H

#include <iomanip>

#ifndef _MSL_NO_CPP_NAMESPACE        // hh 971207 Added backward compatibility
#ifndef _MSL_NO_IO
	using std::resetiosflags;
	using std::setiosflags;
	using std::setbase;
	using std::setfill;
	using std::setprecision;
	using std::setw;
#endif
#endif

#endif
/*     Change record
// hh 971207 Changed filename from iomanip to iomanip.h
// hh 971207 added standard include guards
// hh 971207 Added backward compatibility
// hh 990120 changed name of MSIPL flags
// hh 991113 modified using policy
*/

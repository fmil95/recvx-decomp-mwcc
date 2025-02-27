/*  Metrowerks Standard Library  */

/*  $Date: 1999/11/18 23:41:52 $ 
 *  $Revision: 1.9 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	set.h          // hh 971207 Changed filename from set to set.h
 */

#ifndef _SET_H            // hh 971207 added standard include guards
#define _SET_H

#include <set>

#ifndef _MSL_NO_CPP_NAMESPACE            // hh 971207 Added backward compatibility
	using std::set;
	using std::multiset;
#endif

#endif
/*     Change record
// hh 971207 Changed filename from set to set.h
// hh 971207 added standard include guards
// hh 971207 Added backward compatibility
// hh 990120 changed name of MSIPL flags
// hh 991113 modified using policy
*/

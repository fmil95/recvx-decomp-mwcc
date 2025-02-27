/*  Metrowerks Standard Library  */

/*  $Date: 1999/11/18 23:44:44 $ 
 *  $Revision: 1.9 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright � 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	valarray.h             // hh 971207 Changed filename from valarray to valarray.h
 */

#ifndef _VALARRAY_H               // hh 971207 added standard include guards
#define _VALARRAY_H

#include <valarray>

#ifndef _MSL_NO_CPP_NAMESPACE            // hh 971207 Added backward compatibility
	using std::valarray;
	using std::slice;
	using std::gslice;
#endif

#endif
/*     Change record
// hh 971207 Changed filename from valarray to valarray.h
// hh 971207 added standard include guards
// hh 971207 Added backward compatibility
// hh 990120 changed name of MSIPL flags
// hh 991113 modified using policy
*/

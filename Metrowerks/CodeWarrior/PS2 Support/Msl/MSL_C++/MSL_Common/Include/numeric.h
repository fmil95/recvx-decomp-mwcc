/*  Metrowerks Standard Library  */

/*  $Date: 1999/11/18 23:41:40 $ 
 *  $Revision: 1.9 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	numeric.h          // hh 971207 Changed filename from numeric to numeric.h
 */

#ifndef _NUMERIC_H             // hh 971207 added standard include guards
#define _NUMERIC_H

#include <numeric>

#ifndef _MSL_NO_CPP_NAMESPACE           // hh 971207 Added backward compatibility
	using std::accumulate;
	using std::inner_product;
	using std::partial_sum;
	using std::adjacent_difference;
#endif

#endif
/*     Change record
// hh 971207 Changed filename from numeric to numeric.h
// hh 971207 added standard include guards
// hh 971207 Added backward compatibility
// hh 990120 changed name of MSIPL flags
// hh 991113 modified using policy
*/

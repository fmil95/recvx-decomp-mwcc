/*  Metrowerks Standard Library  */

/*  $Date: 1999/11/18 23:40:10 $ 
 *  $Revision: 1.9 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright � 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	map.h           // hh 971207 Changed filename from map to map.h
 */

#ifndef _MAP_H             // hh 971207 added standard include guards
#define _MAP_H

#include <map>

#ifndef _MSL_NO_CPP_NAMESPACE       // hh 971207 Added backward compatibility
	using std::map;
	using std::multimap;
#endif

#endif
/*     Change record
// hh 971207 Changed filename from map to map.h
// hh 971207 added standard include guards
// hh 971207 Added backward compatibility
// hh 990120 changed name of MSIPL flags
// hh 991113 modified using policy
*/

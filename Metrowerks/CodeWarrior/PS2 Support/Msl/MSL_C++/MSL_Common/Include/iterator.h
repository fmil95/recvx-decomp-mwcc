/*  Metrowerks Standard Library  */

/*  $Date: 1999/11/18 23:39:21 $ 
 *  $Revision: 1.10 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright � 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	iterator.h               // hh 971207 Changed filename from iterator to iterator.h
 */

#ifndef _ITERATOR_H                 // hh 971207 added standard include guards
#define _ITERATOR_H

#include <iterator>

#ifndef _MSL_NO_CPP_NAMESPACE        // hh 971207 Added backward compatibility
	using std::iterator_traits;
	using std::iterator;
	using std::input_iterator_tag;
	using std::output_iterator_tag;
	using std::forward_iterator_tag;
	using std::bidirectional_iterator_tag;
	using std::random_access_iterator_tag;
	using std::advance;
	using std::distance;
	using std::reverse_iterator;
	using std::back_insert_iterator;
	using std::back_inserter;
	using std::front_insert_iterator;
	using std::front_inserter;
	using std::insert_iterator;
	using std::inserter;
#ifndef _MSL_NO_IO
	using std::istream_iterator;
	using std::ostream_iterator;
	using std::istreambuf_iterator;
	using std::ostreambuf_iterator;
#endif
#endif

#endif
/*     Change record
// hh 971207 Changed filename from iterator to iterator.h
// hh 971207 added standard include guards
// hh 971207 Added backward compatibility
// hh 990120 changed name of MSIPL flags
// hh 991113 modified using policy
*/

/*
 *	newop.cpp
 *	
 *		Copyright © 1996 Metrowerks, Inc.
 *		All rights reserved.
 *
 */

#include <new>
#include <NewHand.h>
#include <stdlib.h>

#pragma exceptions on

// Global variable indicating whether a bad_alloc object should be thrown
// when a new operation failes.


#ifdef _MSL_USING_NAMESPACE          // hh 971207 Added namespace support
	namespace std {
#endif


char	__throws_bad_alloc = 1;

	nothrow_t nothrow;                // hh 980124 added nothrow

#ifdef _MSL_USING_NAMESPACE          // hh 971207 Added namespace support
	}
#endif

void * operator new(size_t size) throw(_STD::bad_alloc)  //cc 991118
{
	// If the user does a new 0 do not try to allocate 0 or we will crash.
	// I should still return a real pointer so I am allocating 1.
	if (size == 0) {
		size = 1;
	}

	void * p;

	while ((p = malloc(size)) == NULL) {
		if (_STD::_new_handler_func) {
			_STD::_new_handler_func();
		} else if (_STD::__throws_bad_alloc) { 
			throw (_STD::bad_alloc()); 
		} else {
			return NULL;
		}
	}
	
	return(p);
}



/* Change History
960830 KO	Changed the second parameter type from nothrow to nothrow_t.
			Now it matches the declaration and compiles.
960905 KO	Made this file. The original MSL file new.cpp was split
			into NewHand.cpp, NewHand.h,  newop.cpp, and delop.cpp so
			the user can overload the new and delete operators.
961216 KO	Added check for new 0. Now the flag __throws_bad_alloc is
			used.
// hh 971207 Added namespace support
// hh 980124 added nothrow
// vss 980110  remove new
//  cc  991118  Added _STD:: to objects that are in the std namespace  
*/

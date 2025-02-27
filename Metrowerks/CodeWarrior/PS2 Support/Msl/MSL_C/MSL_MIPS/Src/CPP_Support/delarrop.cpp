/*
 *	delarrop.cpp
 *	
 *		Copyright © 1996 Metrowerks, Inc.
 *		All rights reserved.
 *
 */

#include <stddef.h>
#include <new>
#include <NewHand.h>
#include <stdlib.h>

#pragma exceptions on


#ifdef _MSL_USING_NAMESPACE          // hh 971207 Added namespace support
	namespace std {
#endif


// Global variable indicating whether a bad_alloc object should be thrown
// when a new operation failes.
extern char	__throws_bad_alloc;

#ifdef _MSL_USING_NAMESPACE          // hh 971207 Added namespace support
	}
#endif

/************************************************************************/
/*	Purpose..: 	Array allocation/deallocation functions					*/
/*	Input....:	---														*/
/*	Return...:	---														*/
/************************************************************************/
#if __MWERKS__>=0x2020

extern void operator delete[](void *ptr) throw()
{
	operator delete(ptr);
}
#endif

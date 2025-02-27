/*
 *	newnothrow.cpp
 *	
 *		Copyright © 1996 Metrowerks, Inc.
 *		All rights reserved.
 *
 */

#include <new>
#include <NewHand.h>
#include <stdlib.h>

#pragma exceptions on

void * operator new(size_t size, const _STD::nothrow_t&) throw()  //cc 991118
{
	// If the user does a new 0 do not try to allocate 0 or we will crash.
	// I should still return a real pointer so I am allocating 1.
	if (size == 0) {
		size = 1;
	}

	void * p;
	
	while ((p = malloc(size)) == NULL)
		if (_STD::_new_handler_func)      //cc991118
		{
			try
			{
				_STD::_new_handler_func;  //cc991118
			}
			catch (_STD::bad_alloc)       //cc 991118
			{
				return(NULL);
			}
		}
		else
			return(NULL);
	
	return(p);
}


extern void *operator new[](size_t size,const _STD::nothrow_t& nt) throw()
{
	return operator new(size,nt);
}


//  cc  991118  Added _STD:: to objects that are in the std namespace  
//  cc  991203  Added pc's changes of missing object and added _STD::
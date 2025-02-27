/*
 *	NewHand.cpp
 *	
 *		Copyright © 1996 Metrowerks, Inc.
 *		All rights reserved.
 *
 *	Modification History
 *	--------------------
 *
 *	5-Sep-96 KO	Made this file. The original MSL file new.cpp was split
 *	            into NewHand.cpp, NewHand.h,  newop.cpp, and delop.cpp so
 *              you can overload the new and delete operators.
 */

#include <new>
#include <NewHand.h>

#pragma exceptions on

#ifdef _MSL_USING_NAMESPACE          // hh 971207 Added namespace support
	namespace std {
#endif

static void default_new_handler() throw(bad_alloc)
{
	throw(bad_alloc());
}

new_handler	_new_handler_func = default_new_handler;

new_handler set_new_handler(new_handler newer_handler) throw()  // hh 980108 added exception specification
{
	new_handler	old_handler = _new_handler_func;
	
	_new_handler_func = newer_handler;
	
	return(old_handler);
}

#ifdef _MSL_USING_NAMESPACE          // hh 971207 Added namespace support
	}
#endif

// hh 971207 Added namespace support
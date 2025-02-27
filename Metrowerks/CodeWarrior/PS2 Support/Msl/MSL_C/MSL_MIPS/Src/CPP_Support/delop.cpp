/*
 *	delop.cpp
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
#include <stdlib.h>

#pragma exceptions on

void operator delete(void * p)
{
	free(p);
}


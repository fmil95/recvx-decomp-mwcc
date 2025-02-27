/************************************************************************/
/*	Project...:	Standard C++ Library									*/
/*	Name......:	MWException.cp											*/
/*	Purpose...:	portable exception handling								*/
/*  Copyright.: Copyright © 1993-1997 Metrowerks, Inc.					*/
/************************************************************************/

#ifdef __MC68K__
#pragma far_data off
#endif

#define __NOSTRING__	//	do not include <string>

#include <stdlib.h>
#include <CPlusLib.h>
#include <exception.h>
#include "NMWException.h"
#include "__config.h"


//	class __partial_array_destructor
//
//		This class is used to guarantee correct destruction of partially
//		constructed arrays if an exception is thrown from an array-element constructor.
//		We use the members of this class to keep track of the partially-constructed
//		state of the array. If the destructor for this class is called and the
//		array was not fully-constructed, we must have thrown an exception, so we
//		destroy each fully-constructed element of the array in reverse order.

class __partial_array_destructor {
private:
	void*					p;		//	pointer to start of array being constructed
	size_t					size;	//	size of each array element
	size_t					n;		//	# of elements being constructed
	ConstructorDestructor	dtor;	//	pointer to destructor for elements or 0
public:
	size_t					i;		//	current element being constructed
	
	__partial_array_destructor(void* array, size_t elementsize, size_t nelements, ConstructorDestructor destructor)
	{
		p=array; size=elementsize; n=nelements; dtor=destructor; i=n;
	}
		
	~__partial_array_destructor()
	{
		char *ptr;
		
		if(i<n && dtor)
		{
			for(ptr=(char *)p+size*i; i>0; i--)
			{
				ptr-=size;
				DTORCALL_COMPLETE(dtor,ptr);
			}
		}
	}
};

/************************************************************************/
/*	Purpose..: 	Initialize a new allocated array of objects				*/
/*	Input....:	pointer to allocated memory (+8 bytes) (0L: error)		*/
/*	Input....:	pointer to default constructor function	(or 0L)			*/
/*	Input....:	pointer to destructor function (or 0L)					*/
/*	Input....:	size of one array element								*/
/*	Input....:	number of array elements								*/
/*	Return...:	pointer to first object in array						*/
/************************************************************************/
extern void *__construct_new_array(void *block,ConstructorDestructor ctor,ConstructorDestructor dtor,size_t size,size_t n)
{
	char	*ptr;

	if((ptr=(char *)block)!=0L)
	{
		//	store number of allocated objects and size of one object at the beginnig of the allocated block
		*(size_t *)ptr=size;
		*(size_t *)(ptr+sizeof(size_t))=n;
		ptr+=__ALIGNMENT__;

		if(ctor)
		{	//	call constructor to initialize array
			__partial_array_destructor pad(ptr,size,n,dtor);
			char	*p;

			for(pad.i=0,p=(char *)ptr; pad.i<n; pad.i++,p+=size) CTORCALL_COMPLETE(ctor,p);
		}
	}
	return ptr;	//	return pointer to first object;
}

/************************************************************************/
/*	Purpose..: 	Construct an array of objects							*/
/*	Input....:	pointer to array memory									*/
/*	Input....:	pointer to default constructor function					*/
/*	Input....:	pointer to destructor function (or 0L)					*/
/*	Input....:	size of one array element								*/
/*	Input....:	number of array elements								*/
/*	Return...:	---														*/
/************************************************************************/
extern void __construct_array(void *ptr,ConstructorDestructor ctor,ConstructorDestructor dtor,size_t size,size_t n)
{
	__partial_array_destructor pad(ptr,size,n,dtor);
	char	*p;

	for(pad.i=0,p=(char *)ptr; pad.i<n; pad.i++,p+=size) CTORCALL_COMPLETE(ctor,p);
}


/************************************************************************/
/*	Purpose..: 	Destroy an array of objects								*/
/*	Input....:	pointer to array memory									*/
/*	Input....:	pointer to destructor function							*/
/*	Input....:	size of one object										*/
/*	Input....:	number of objects										*/
/*	Return...:	---														*/
/************************************************************************/
extern void __destroy_arr(void *block,ConstructorDestructor dtor,size_t size,size_t n)
{
	char	*p;

	for(p=(char *)block+size*n; n>0; n--)
	{
		p-=size;
		DTORCALL_COMPLETE(dtor,p);
	}
}

/************************************************************************/
/*	Purpose..: 	Delete an array of objects								*/
/*	Input....:	pointer to first object	(can be 0L)						*/
/*	Input....:	pointer to destructor function							*/
/*	Return...:	---														*/
/************************************************************************/
extern void __destroy_new_array(void *block,ConstructorDestructor dtor)
{
	if(block)
	{
		if(dtor)
		{
			size_t	i,objects,objectsize;
			char	*p;

			objectsize=*(size_t *)((char *)block-__ALIGNMENT__);
			objects=*(size_t *)((char *)block-__ALIGNMENT__+sizeof(size_t));
			p=(char *)block+objectsize*objects;
			for(i=0; i<objects; i++)
			{
				p-=objectsize;
				DTORCALL_COMPLETE(dtor,p);
			}
		}
		#if __MWERKS__>=0x2020
  			::operator delete[] ((char *)block-__ALIGNMENT__);
 		#else
  			::operator delete ((char *)block-__ALIGNMENT__);
  		#endif
	}
}


/************************************************************************/
/*	Purpose..: 	Destroy a new allocated array of objects				*/
/*	Input....:	pointer to first object	(can be NULL)					*/
/*	Input....:	pointer to destructor function (not NULL)				*/
/*	Return...:	pointer to complete array block							*/
/************************************************************************/
extern void *__destroy_new_array2(void *block,ConstructorDestructor dtor)
{
	size_t	i,objects,objectsize;
	char	*p;

	if(block==NULL) return NULL;

	if(dtor)
	{	//	destroy array members
		objectsize=*(size_t *)((char *)block-__ALIGNMENT__);
		objects=*(size_t *)((char *)block-__ALIGNMENT__+sizeof(size_t));
		p=(char *)block+objectsize*objects;
		for(i=0; i<objects; i++)
		{
			p-=objectsize;
			DTORCALL_COMPLETE(dtor,p);
		}
	}

	//	return pointer to complete array block
	return (char *)block-__ALIGNMENT__;
}

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

#include <ansi_parms.h>
#include <stdlib.h>
#include <CPlusLib.h>
#include <exception.h>
#include "NMWException.h"

#ifdef _MSL_USING_NAMESPACE       // hh 971207 Added namespace support
	namespace std {
#endif

static void dthandler()		{ abort(); };			//	default terminate handler function
static terminate_handler	thandler = dthandler;	//	pointer to terminate handler function
static void duhandler()		{ terminate(); };		//	default unexpected handler function
static unexpected_handler	uhandler = duhandler;	//	pointer to unexpected handler function

/************************************************************************/
/*	Purpose..: 	Set a terminate handler function						*/
/*	Input....:	pointer to terminate handler function					*/
/*	Return...:	---														*/
/************************************************************************/
extern terminate_handler set_terminate(terminate_handler handler) throw()  // hh 980108 added exception specification
{
	terminate_handler old=thandler; thandler=handler; return old;
}

/************************************************************************/
/*	Purpose..: 	Terminate exception handling							*/
/*	Input....:	---														*/
/*	Return...:	--- (shall not return to caller)						*/
/************************************************************************/
extern void terminate()
{
	thandler();
}

/************************************************************************/
/*	Purpose..: 	Set an unexpected handler function						*/
/*	Input....:	pointer to unexpected handler function					*/
/*	Return...:	---														*/
/************************************************************************/
extern unexpected_handler set_unexpected(unexpected_handler handler) throw()  // hh 980108 added exception specification
{
	unexpected_handler old=uhandler; uhandler=handler; return old;
}

/************************************************************************/
/*	Purpose..: 	Handle unexpected exception								*/
/*	Input....:	---														*/
/*	Return...:	--- (shall not return to caller)						*/
/************************************************************************/
extern void unexpected()
{
	uhandler();
}

#ifdef _MSL_USING_NAMESPACE            // hh 971207 Added namespace support
	}
#endif


/************************************************************************/
/* Purpose..: Compare a throw and a catch type							*/
/* Input....: pointer to throw type										*/
/* Input....: pointer to catch type (0L: catch(...)						*/
/* Return...: true: can catch; false: cannot catch						*/
/************************************************************************/
#if __MIPS_WinCE__
extern char __throw_catch_compare(const char *throwtype,const char *catchtype,long *offset_result)
{
	const char	*cptr1,*cptr2;

	*offset_result=0;	
	if((cptr2=catchtype)==0)
	{	//	catch(...)
		return true;
	}
	cptr1=throwtype;

	if(*cptr2=='P')
	{	//	catch(cv T *)
		//  string is PlT where l = A-D for const and volatile
		cptr2 += 2;	// get to the type
		if(*cptr2=='X')
		{	//	catch(cv void *)
			if(*cptr1=='P' || *cptr1=='*')
			{	//	throw T*;
				return true;
			}
		}
		cptr2=catchtype;
	}

	switch(*cptr1)
	{	//	class pointer/reference throw
	case '*':
	case '!':
		if(*cptr1++!=*cptr2++) return false;
		for(;;)
		{	//	class name compare loop
			if(*cptr1==*cptr2++)
			{
				if(*cptr1++=='!')
				{	//	class match found / get offset
					long offset;

					for(offset=0; *cptr1!='!';) offset=offset*10+*cptr1++-'0';
					*offset_result=offset; return true;
				}
			}
			else
			{
				while(*cptr1++!='!') ;			//	skip class name
				while(*cptr1++!='!') ;			//	skip offset
				if(*cptr1==0) return false;		//	no more class names => no match
				cptr2=catchtype+1;				//	retry with next class name
			}
		}
		return false;
	}

	while((*cptr1=='P' || *cptr1=='Q') && *cptr1==*cptr2)
	{	//	pointer/reference catch => match cv-qualifiers
		int cv1, cv2;
		
		cptr1++; cptr2++;
		
		// Get cv qual for catcher and thrower
		
		cv1 = *cptr1++ - 'A';
		cv2 = *cptr2++ - 'A';
		
		// catcher must be at least as qualified as thrower 
		
		if (cv1 & ~cv2 != 0) return false;
	}

	//	plain type throw catch
	for(; *cptr1==*cptr2; cptr1++,cptr2++) if(*cptr1==0) return true;
	return false;
}


#else
extern char __throw_catch_compare(const char *throwtype,const char *catchtype,long *offset_result)
{
	const char *cptr1,*cptr2;

	*offset_result=0; 
	if((cptr2=catchtype)==0)
	{ // catch(...)
		return true;
	}
	cptr1=throwtype;

	if(*cptr2=='P')
	{ // catch(cv T *)
		cptr2++;
		if(*cptr2=='C') cptr2++;
		if(*cptr2=='V') cptr2++;
		if(*cptr2=='v')
		{ // catch(cv void *)
			if(*cptr1=='P' || *cptr1=='*')
			{ // throw T*;
				return true;
			}
		}
		cptr2=catchtype;
	}

	switch(*cptr1)
	{ // class pointer/reference throw
		case '*':
		case '!':
			if(*cptr1++!=*cptr2++) return false;
			for(;;)
			{ // class name compare loop
				if(*cptr1==*cptr2++)
				{
					if(*cptr1++=='!')
					{ // class match found / get offset
						long offset;
			
						for(offset=0; *cptr1!='!';) offset=offset*10+*cptr1++-'0';
						*offset_result=offset; return true;
					}
				}
				else
				{
					while(*cptr1++!='!') ; // skip class name
					while(*cptr1++!='!') ; // skip offset
					if(*cptr1==0) return false; // no more class names => no match
					cptr2=catchtype+1; // retry with next class name
				}
			}
			return false;
	}
	
	while((*cptr1=='P' || *cptr1=='R') && *cptr1==*cptr2)
	{ // pointer/reference catch => match cv-qualifiers
		cptr1++; cptr2++;
		if(*cptr2=='C')
		{ // ignore 'const' in throw type
			if(*cptr1=='C') cptr1++;
			cptr2++;
		}
		if(*cptr1=='C') return false; // throw type is more cv-qualified
	
		if(*cptr2=='V')
		{ // ignore 'volatile' in throw type
			if(*cptr1=='V') cptr1++;
			cptr2++;
		}
		if(*cptr1=='V') return false; // throw type is more cv-qualified
	}
	
	// plain type throw catch
	for(; *cptr1==*cptr2; cptr1++,cptr2++) if(*cptr1==0) return true;
	return false;
}
#endif

/* Change History
961209 KO	Commented out true and false defines which should already be defined.
// hh 971207 Added namespace support
// rjk 980322 Removed global destructor stuff to a separate module.
*/

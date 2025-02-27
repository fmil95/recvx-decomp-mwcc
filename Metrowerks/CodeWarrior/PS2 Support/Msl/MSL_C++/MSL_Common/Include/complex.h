/*  Metrowerks Standard Library  */

/*  $Date: 1999/11/18 23:34:32 $ 
 *  $Revision: 1.10 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright � 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */
                                       
/*
 *	complex.h        // hh 971206 Changed filename from complex to complex.h
 *	
 */

#ifndef _COMPLEX_H         // hh 971206 Made include guards standard
#define _COMPLEX_H

#include <complex>

#ifndef _MSL_NO_CPP_NAMESPACE      // hh 971206 Backward compatibility added with "using"
#ifndef _No_Floating_Point
	using std::complex;
#endif
#endif

#endif
/*     Change record
// hh 971206 Changed filename from complex to complex.h
// hh 971206 Made include guards standard
// hh 971206 Backward compatibility added with "using"
// hh 990120 changed name of MSIPL flags
// hh 991112 modified using policy
*/

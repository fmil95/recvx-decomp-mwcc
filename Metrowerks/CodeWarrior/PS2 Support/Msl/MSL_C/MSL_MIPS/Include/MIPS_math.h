/*  This header is intended for mips specific optimizations and should 
    not compile elsewhere
    
*/    
#pragma warn_unusedarg off
#pragma warn_unusedvar off
#include <errno.h>
#include <MIPS_math_f2i_i2f.h>

inline double fabs(double x)			{	return __d_abs(x);	} 
inline float fabsf(float x)				{	return __s_abs(x);	}

#pragma mark === No FPU ===


#pragma mark === Single FPU ===
#if __MIPS_single_fpu__         // stuff common to all mips processors w/ single precision 
                                // instructions.


	#if __option(sqrt) 

		inline float __s_sqrt(register float x)
		{	register float y;	asm(sqrt.s y, x);	return y;	}
					
		inline float sqrtf(float x)
		{
	    	if( x >= 0.0)	return __s_sqrt(x);
	        else 			{	errno=EDOM; return NAN;	}		
		}

	#endif
	
	#if  __MIPS_ISA3__ 
	
		inline float ceilf(register float x)
		{	
			register float i;		
			asm(ceil.w.s 	i, x
				cvt.s.w 	i, i);		
			return i;	
		}
				
		inline float floorf(register float x)
		{	
			register float i;		
			asm(floor.w.s 	i, x
				cvt.s.w		i, i);	
			return i;	
		}		
	
	#endif	
	
		
	#if __option(rsqrt)
		#define __PI_O2 1.570796327f  

		inline float __s_inv_sqrt(register float x)
		{		
	        if( x >= 0.0)	{	register float y;	asm(rsqrt.s y, x);	return y;	}        
	        else			{	errno=EDOM;	return NAN;	}
		}
		
		
		inline float atan2f(float y, float x)
		{	return atan(y/x);	}
		
		inline float acosf(float x)
		{	 return __PI_O2 - atan(x*__s_inv_sqrt(1.0f- x*x));	}
		
		
		inline float asinf(float x)
		{	return atan(x*__s_inv_sqrt(1.0f- x*x));	}
	
	
	#endif  //__MIPS_ISA4__

#endif  // __MIPS_single_fpu__




#pragma mark === Double FPU ===
#if __MIPS_double_fpu__

	/*
	 *	MIPS ISA2 support
	 *
	 */
	 
	#if __option(sqrt)
	
		inline float __d_sqrt(register double x)
		{	register double y;	asm(sqrt.d y, x);	return y;	}
					
		inline double sqrt(double x)
		{	
			if( x >= 0.0)	return __d_sqrt(x);
		    else			{	errno=EDOM; return NAN; }		
		}
	
	#endif  //__MIPS_ISA2__
	
	
	/*
	 *	MIPS ISA3 support
	 *
	 */
	 
	#if __MIPS_ISA3__
	
		inline double ceil(register double x)
		{	
			register double i;
			asm(ceil.l.d 	i, x
				cvt.d.l 	i, i);		
			return i;	
		}
			
		inline double floor(register double x)
		{
			register double i;		
			asm(floor.l.d 	i, x
				cvt.d.l		i, i);	
			return i;	
		}
		
	#endif
	
	
	/*
	 *	MIPS ISA4 support
	 *
	 */
	 
	#if __option(rsqrt)
	
		inline double __d_inv_sqrt(register double x)
		{
			if (x >= 0.0) 	{	register double y; asm(rsqrt.d y, x); return y; }        
			else 		 	{	errno=EDOM; return NAN;	}
		}
	
	#endif
	
#endif


#pragma mark === MIPS 5900 ===
#if __MIPS_processor__ == __MIPS_R5900__
	
	inline float fminf(register float x, register float y)
	{	register float z;	asm(min.s	z, x, y);	return z;	}

	inline float fmaxf(register float x, register float y)
	{	register float z;	asm(max.s	z, x, y);	return z;	}


#endif

#pragma warn_unusedarg reset
#pragma warn_unusedvar reset

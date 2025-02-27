#ifndef __MIPS_math_f2i_i2f
#define __MIPS_math_f2i_i2f

#pragma warn_unusedarg off
#pragma warn_unusedvar off

#if !__fpu__         

	inline float __int2float__(register int x)
	{	register float y;	asm(move y, x);	return y;	}
		
	inline int __float2int__(register float x)
	{	register int y;	asm(move y, x);	return y;	}

	#if __cpu64__

		inline double __ll2double__(register long long x)
		{	register double y;	asm(dmove y, x);	return y;	}
	
		inline long long __double2ll__(register double x)
		{	register long long y;	asm(dmove y, x);	return y;	}

	#endif

#endif	
	
	
#if __MIPS_single_fpu__         // stuff common to all mips processors w/ single precision 

	inline float __int2float__(register int x)
	{	register float y;	asm(mtc1 x, y);	return y;	}

	inline int __float2int__(register float x)
	{	register int y;	asm(mfc1 y, x);	return y;	}
			
#endif


#if __MIPS_double_fpu__

	#if __cpu64__ && __fpu64__

		inline double __ll2double__(register long long x)
		{	register double y;	asm(dmtc1 x, y);	return y;	}
	
		inline long long __double2ll__(register double x)
		{	register long long y;	asm(dmfc1 y, x);	return y;	}

	#endif

#endif

#pragma warn_unusedarg reset
#pragma warn_unusedvar reset

#endif
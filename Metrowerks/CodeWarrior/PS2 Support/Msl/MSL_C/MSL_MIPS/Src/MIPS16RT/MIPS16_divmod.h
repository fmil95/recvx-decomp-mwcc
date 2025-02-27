
/* Integer multiplication, division, and modulo for 
 * signed and unsigned integer (not long long) in MIPS16.
 */
#ifdef __cplusplus
	#define EXTERN	extern "C"
#else
	#define EXTERN extern
#endif

EXTERN int __MIPS16_l_muls(int left, int right);
EXTERN unsigned int __MIPS16_l_mulu(unsigned int left, unsigned int right);
EXTERN int __MIPS16_l_divs(int left, int right);
EXTERN int __MIPS16_l_mods(int divisor, int dividend);
EXTERN int __MIPS16_l_divu(unsigned int left, unsigned int right);
EXTERN int __MIPS16_l_modu(unsigned int left, unsigned int right);

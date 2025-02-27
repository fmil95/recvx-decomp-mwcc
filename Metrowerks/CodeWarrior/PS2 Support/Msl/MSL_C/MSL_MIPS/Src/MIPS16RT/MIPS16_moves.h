/* Integer multiplication, division, and modulo for 
 * signed and unsigned integer (not long long) in MIPS16.
 */
#ifdef __cplusplus
	#define EXTERN	extern "C"
#else
	#define EXTERN extern
#endif

EXTERN asm int __MIPS16_move_block_byte_loop(unsigned int dest, unsigned int source, unsigned int size);
EXTERN asm int __MIPS16_move_block_half_loop(unsigned int dest, unsigned int source, unsigned int size);
EXTERN asm int __MIPS16_move_block_word_loop(unsigned int dest, unsigned int source, unsigned int size);
EXTERN asm int __MIPS16_move_block_byte_seq(unsigned int dest, unsigned int source, unsigned int size);
EXTERN asm int __MIPS16_move_block_half_seq(unsigned int dest, unsigned int source, unsigned int size);
EXTERN asm int __MIPS16_move_block_word_seq(unsigned int dest, unsigned int source, unsigned int size);

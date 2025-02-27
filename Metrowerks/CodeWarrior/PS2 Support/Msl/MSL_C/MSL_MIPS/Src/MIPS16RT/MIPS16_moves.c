#include <MIPS16_moves.h>

EXTERN asm int __MIPS16_move_block_byte_loop(unsigned int dest, unsigned int source, unsigned int size)
{
		.set reorder
		move	t9,a2		// loop count 	
next:
        lbu		t6,0(a1)
        addi	t9,t9,-1
        sb		t6,0(a0)
        addi	a1,a1,1
        addiu	a0,a0,1
        bgtz	t9,next             
		jr		ra
}

/*
 * _u_block_move_half_loop
 *
 * Entry:	a0/	destination address
 *			a1/	source address
 *			a2/ size of block in bytes
 *
 */
EXTERN asm int __MIPS16_move_block_half_loop(unsigned int dest, unsigned int source, unsigned int size)
{
		.set reorder
		srl		t9,a2,1		// a2/2 gives # load/store pairs
next:
		lh		t6,0(a1)
		addiu	t9,t9,-1
		sh		t6,0(a0)
		addiu	a1,a1,2
		addiu	a0,a0,2
		bgtz	t9,next             

  		andi	a2,a2,1		// odd number of bytes?
		beqz	a2,done
		lbu		t6,0(a1)
		sb		t6,0(a0) 
done:
		move	v0,a0
		jr		ra
}

 
EXTERN asm int __MIPS16_move_block_word_loop(unsigned int dest, unsigned int source, unsigned int size)
{
		.set reorder
  		srl		t9,a2,2		// a2/4 gives # load/store pairs
next:
		lw		t6,0(a1)
		addiu	t9,t9,-1
		sw		t6,0(a0)
		addiu	a1,a1,4
		addiu	a0,a0,4
		bgtz	t9,next             

		andi	t9,a2,2		// 2 or more bytes left?
		beqz	t9,tail
		lhu		t6,0(a1)
		sh		t6,0(a0) 
tail:
		andi	a2,a2,1		// odd number of bytes?
		beqz	a2,done
		lbu		t6,2(a1)
		sb		t6,2(a0) 
done:
		move	v0,a0
		jr		ra
}

 
EXTERN asm int __MIPS16_move_block_byte_seq(unsigned int dest, unsigned int source, unsigned int size)
{
		.set reorder
 
 		addi	a2,a2,-3
 		beqz	a2,mov3
		addi	a2,a2,1
		beqz	a2,mov2 				
 mov1:
		lbu    	t0,0(a1)
        sb		t0,0(a0)
		move	v0,a0
		jr		ra

 mov2:
		lbu    	t0,0(a1)
		lbu		t1,1(a1)
		sb     	t0,0(a0)
 		sb     	t1,1(a0)
		move	v0,a0
		jr		ra

 mov3:
		lbu    	t0,0(a1)
		lbu		t1,1(a1)
		lbu		t2,2(a1)
 		sb		t0,0(a0)
		sb		t1,1(a0)
		lbu		t2,2(a0)
		move	v0,a0
		jr		ra
}

EXTERN asm int __MIPS16_move_block_half_seq(unsigned int dest, unsigned int source, unsigned int size)
{
		.set reorder
 
 		srl		a3,a2,1
 		addi	a3,a3,-3
 		beqz	a3,mov3
		addi	a3,a3,1
		beqz	a3,mov2 				
		move	v0,a0
		jr		ra
 mov1:
		lhu    	t0,0(a1)
        sh		t0,0(a0)
		b		tail
 mov2:
 		lhu    	t0,0(a1)
		lhu		t1,1(a1)
		sh		t0,0(a0)
 		sh     	t1,1(a0)
		b		tail
 mov3:
 		lhu    	t0,0(a1)
 		lhu		t1,1(a1)
		lhu		t2,2(a1)
		sh		t0,0(a0)
		sh		t1,1(a0)
		sh		t2,2(a0)
	
tail:
		andi	a2,a2,1		// odd number of bytes?
		beqz	a2,done
		lbu		t0,0(a1)
		sb		t0,0(a0) 
done:
		move	v0,a0
		jr		ra
}


 EXTERN asm int __MIPS16_move_block_word_seq(unsigned int dest, unsigned int source, unsigned int size)
{
		.set reorder
 		srl		a3,a2,2
 		addi	a3,a3,-3
 		beqz	a3,mov3
		addi	a3,a3,1
		beqz	a3,mov2 				
 mov1:
		lw     	t0,0(a1)
       	sw      t0,0(a0)
		b 		tail1
 mov2:
		lw     	t0,0(a1)
		sw      t0,0(a0)
 		addi	a0,a0,4
 		addi 	a1,a1,4
 		lw     	t0,0(a1)
 		sw      t0,0(a0)
		b		tail1
 mov3:
		lw     	t0,0(a1)
		sw      t0,0(a0)
 		addi	a0,a0,4
 		addi 	a1,a1,4
 		lw     	t0,0(a1)
 		sw      t0,0(a0)
 		addi	a0,a0,4
 		addi 	a1,a1,4
 		lw     	t0,0(a1)
 		sw      t0,0(a0)

tail1:
  		andi	a3,a2,2		// 2 or more bytes left?
		beqz	a3,tail2
		lhu		t6,0(a1)
		sh		t6,0(a0) 

tail2:
  		andi	a2,a2,1		// odd number of bytes?
		beqz	a2,done
  		addi	a0,a0,2
  		addi	a1,a1,2
		lbu		t0,0(a1)
		sb		t0,0(a0) 
done:
		move	v0,a0
		jr		ra
}



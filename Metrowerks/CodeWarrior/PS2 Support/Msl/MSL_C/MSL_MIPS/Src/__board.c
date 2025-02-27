#include <__board.h>
#include <__config.h>

/************************************************************/
/*                                                          */
/* Boards configuration										*/
/*                                                          */
/************************************************************/

#pragma mark -
#pragma mark === IDT SIM Support ===
#if __IDT_SIM__
EXTERN asm void timer_start()
{
	/* Realign the stack to an 8 byte boundary before	*/
	/* calling IDTsym.  This is done by simulating a 	*/
	/* dynamic allocation on stack of the right size.	*/
	/* This allows us to call an ISA3 version of the 	*/
	/* monitor when running with 32 bit registers		*/
	addiu	sp,sp,-16
	sw		ra,12(sp)
	sw		fp,8(sp)
	move	fp,sp
	li		t8,~7
	and		sp,sp,t8
			
	.set	reorder
	li		t8,PROM_TIMER_START
	jalr	ra,t8		// preserve return address!

	move	sp,fp
	lw		ra,12(sp)
	lw		fp,8(sp)
	addiu	sp,sp,16
	jr		ra
}

EXTERN asm unsigned int timer_stop()
{
	/* Realign the stack to an 8 byte boundary before	*/
	/* calling IDTsym.  This is done by simulating a 	*/
	/* dynamic allocation on stack of the right size.	*/
	/* This allows us to call an ISA3 version of the 	*/
	/* monitor when running with 32 bit registers		*/
	addiu	sp,sp,-16
	sw		ra,12(sp)
	sw		fp,8(sp)
	move	fp,sp
	li		t8,~7
	and		sp,sp,t8
			
	.set	reorder
	li		t8,PROM_TIMER_STOP
	jalr	ra,t8		// preserve return address!

	move	sp,fp
	lw		ra,12(sp)
	lw		fp,8(sp)
	addiu	sp,sp,16
	jr		ra
}

/* reset the board											*/
EXTERN asm void __reset(void)
{
	.set	reorder
	li		t8,PROM_RESET
	jr		t8		// preserve return address!
}


/* output a character										*/
EXTERN asm void __putchar(const char c)
{
	/* Realign the stack to an 8 byte boundary before	*/
	/* calling IDTsym.  This is done by simulating a 	*/
	/* dynamic allocation on stack of the right size.	*/
	/* This allows us to call an ISA3 version of the 	*/
	/* monitor when running with 32 bit registers		*/
	addiu	sp,sp,-16
	sw		ra,12(sp)
	sw		fp,8(sp)
	move	fp,sp
	li		t8,~7
	and		sp,sp,t8
			
	.set	reorder
	/* Note: c is already in the right register				*/
	li		t8,PROM_PUTCHAR
	jalr	ra,t8		// preserve return address!

	move	sp,fp
	lw		ra,12(sp)
	lw		fp,8(sp)
	addiu	sp,sp,16
	jr		ra
}


/* output a null terminated string							*/
EXTERN asm void __puts(const char *s)
{
	/* Realign the stack to an 8 byte boundary before	*/
	/* calling IDTsym.  This is done by simulating a 	*/
	/* dynamic allocation on stack of the right size.	*/
	/* This allows us to call an ISA3 version of the 	*/
	/* monitor when running with 32 bit registers		*/
	addiu	sp,sp,-16
	sw		ra,12(sp)
	sw		fp,8(sp)
	move	fp,sp
	addiu	sp,sp,-8
	li		t8,~7
	and		sp,sp,t8
			
	.set	reorder
	/* Note: s is already in the right register				*/
	li		t8,PROM_PUTS
	jalr	ra,t8		// preserve return address!
	
	move	sp,fp
	lw		ra,12(sp)
	lw		fp,8(sp)
	addiu	sp,sp,16
	jr		ra
}

/* ANSI printf routine										*/
EXTERN asm void __printf(char *format, ...)
{
	/* Realign the stack to an 8 byte boundary before	*/
	/* calling IDTsym.  This is done by simulating a 	*/
	/* dynamic allocation on stack of the right size.	*/
	/* This allows us to call an ISA3 version of the 	*/
	/* monitor when running with 32 bit registers		*/

	/* Assume that all ... parameters will be passed in	*/
	/* registers										*/
	
	addiu	sp,sp,-48
	sw		ra,44(sp)
	sw		fp,40(sp)
	move	fp,sp
	li		t8,~7
	and		sp,sp,t8
			
	.set	reorder
	li		t8,PROM_PRINTF
	jalr	ra,t8

	move	sp,fp
	lw		ra,44(sp)
	lw		fp,40(sp)
	addiu	sp,sp,48
	jr		ra
}

#elif __PMON__
#pragma mark -
#pragma mark === PMON Support ===

EXTERN void __putchar(char c)
{
	__printf("%c", c);
}


EXTERN void __puts(const char *s)
{
	__printf("%s", s);
}


EXTERN asm void __printf(char *format, ...)
{
	addiu	sp,sp,-48
	sw		ra,44(sp)
	sw		fp,40(sp)
	move	fp,sp
	li		t8,~7
	and		sp,sp,t8
			
	.set	reorder
	li		t8,PROM_PRINTF
	jalr	ra,t8

	move	sp,fp
	lw		ra,44(sp)
	lw		fp,40(sp)
	addiu	sp,sp,48
	jr		ra
}


/* reset the board											*/
EXTERN asm void __reset(void)
{
	.set	reorder
	li		t8,PROM_EXIT
	lw		t8,0(t8)
	jr		t8		// preserve return address!
}


#endif

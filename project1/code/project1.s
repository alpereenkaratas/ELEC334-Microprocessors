/* asm.s
*
* Prepared by:
* Alperen Karataş - 1801022022
*
* Notes:
* ELEC335 2020 Fall Project 1
* Randomized Counter
*
*/

.syntax unified
.cpu cortex-m0plus
.fpu softvfp
.thumb


/* make linker see this */
.global Reset_Handler

/* get these from linker script */
.word _sdata
.word _edata
.word _sbss
.word _ebss


/* define peripheral addresses from RM0444 page 57, Tables 3-4 */
.equ RCC_BASE,         (0x40021000)          // RCC base address
.equ RCC_IOPENR,       (RCC_BASE   + (0x34)) // RCC IOPENR register offset

.equ GPIOA_BASE,       (0x50000000)          // GPIOA base address
.equ GPIOA_MODER,      (GPIOA_BASE + (0x00)) // GPIOA MODER register offset
.equ GPIOA_ODR,        (GPIOA_BASE + (0x14)) // GPIOA ODR register offset
.equ GPIOB_BASE, 	   (0x50000400)			 // GPIOB base address
.equ GPIOB_MODER,      (GPIOB_BASE + (0x00)) // GPIOB MODER register offset
.equ GPIOB_ODR,     	(GPIOB_BASE + (0x14))// GPIOB ODR register offset
.equ GPIOA_IDR,        (GPIOA_BASE + (0x10)) // GPIOA IDR register offset



/* vector table, +1 thumb mode */
.section .vectors
vector_table:
	.word _estack             /*     Stack pointer */
	.word Reset_Handler +1    /*     Reset handler */
	.word Default_Handler +1  /*       NMI handler */
	.word Default_Handler +1  /* HardFault handler */
	/* add rest of them here if needed */


/* reset handler */
.section .text
Reset_Handler:
	/* set stack pointer */
	ldr r0, =_estack
	mov sp, r0

	/* initialize data and bss
	 * not necessary for rom only code
	 * */
	bl init_data
	/* call main */
	bl main
	/* trap if returned */
	b .


/* initialize data and bss sections */
.section .text
init_data:

	/* copy rom to ram */
	ldr r0, =_sdata
	ldr r1, =_edata
	ldr r2, =_sidata
	movs r3, #0
	b LoopCopyDataInit

	CopyDataInit:
		ldr r4, [r2, r3]
		str r4, [r0, r3]
		adds r3, r3, #4

	LoopCopyDataInit:
		adds r4, r0, r3
		cmp r4, r1
		bcc CopyDataInit

	/* zero bss */
	ldr r2, =_sbss
	ldr r4, =_ebss
	movs r3, #0
	b LoopFillZerobss

	FillZerobss:
		str  r3, [r2]
		adds r2, r2, #4

	LoopFillZerobss:
		cmp r2, r4
		bcc FillZerobss
	bx lr


/* default handler */
.section .text
Default_Handler:
	b Default_Handler


/* main function */
.section .text
main:
	push {lr}
	/* enable GPIOA clock, bit0 on IOPENR */
	ldr r6, =RCC_IOPENR
	ldr r5, [r6]
	/* movs expects imm8, so this should be fine */
	movs r4, 0x3
	orrs r5, r5, r4
	str r5, [r6]

	/*enable a lot of the pins for prevent disruption */
	ldr r6, =GPIOA_MODER
	ldr r5, [r6]
    ldr r4,=# 0x3FFFFF
    mvns r4,r4
    ands r5,r5,r4
    ldr r4,=# 0x15555
    orrs r5, r5, r4
    str r5,[r6]

    /* setup PA1 for led 00 for bits 2-3 in MODER */
	ldr r6, =GPIOA_MODER
	ldr r5, [r6]
	/* cannot do with movs, so use pc relative */
	ldr r4, =0x3
	lsls r4, r4, #2
	mvns r4, r4
	ands r5, r5, r4
	str r5, [r6]

	/* enable a lot of the pins for prevent disruption*/
	ldr r6, =GPIOB_MODER
	ldr r5, [r6]
    ldr r4,=# 0x3FFFFF
    mvns r4,r4
    ands r5,r5,r4
    ldr r4,=# 0x15555
    orrs r5, r5, r4
    str r5,[r6]
    //pop{r1-r7, pc}

/*We can say that the method name is Linear Congruential Generator.
 LCG's are known as pseudo random as they require a seed number
 to generate random number sequence.*/
rand:
    ldr r0, =#0x6270     // i choose that
    movs r1, #0x8
    ldr r2, =#0x2022 	// i choose that
    ldr r7, =#0x270F
    muls r0, r0, r1
generated_rand_num:
    subs r0, r0, r2
    cmp r7, r0
    blt generated_rand_num
    str r0, [r3]

	ldr r1,= #4000000
	bl delay

/*if button was pressed, start counting*/
/*if button was not pressed, display school number*/
button_ctrl:
	/*ctrl button connected to PA1 in IDR.*/
	ldr r6, =GPIOA_IDR
	ldr r5, [r6]
	lsrs r5, r5, #1
	movs r4, #0x1
	ands r5, r5, r4

	cmp r5, #0x1
	beq loop_d1		// branch to loop_d1 if button was pressed
	b school_num	// branch to school_num if button was not pressed

/*loop_d1 for displaying the number in digit1*/
loop_d1:
	/*enable PA0, PA5, PA6, PA7 pins*/
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r5, =#0x0000
	ldr r3, =#0xE1
	orrs r5, r5, r3
	str r5, [r6]

	ldr r4,= #5000

/*this function using for displaying generated number*/
myrand:
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r5, =#0x0000
	ldr r3, =#0x80
	orrs r5, r5, r3
	str r5, [r6]

	ldr r6, =GPIOB_ODR	// 4
	ldr r5, [r6]
	ldr r3, =#0x19
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	/*to ensure continuity, add small delay*/
	ldr r1,= #500
	bl delay

	/*light up digit2 connected to PA6; light off digit1-3-4*/
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r5, =#0x0000
	ldr r3, =#0x40
	orrs r5, r5, r3
	str r5, [r6]

	ldr r6, =GPIOB_ODR // 1
	ldr r5, [r6]
	ldr r3, =#0xF9
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	/*to ensure continuity, add small delay*/
	ldr r1,= #500
	bl delay

	/*light up digit3 connected to PA0; light off digit1-2-4*/
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r5, =#0x0000
	ldr r3, =#0x1
	orrs r5, r5, r3
	str r5, [r6]

	ldr r6, =GPIOB_ODR	// 7
	ldr r5, [r6]
	ldr r3, =#0xF8
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	/*to ensure continuity, add small delay*/
	ldr r1,= #500
	bl delay

	/*light up digit4 connected to PA5; light off digit1-2-3*/
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r5, =#0x0000
	ldr r3, =#0x20
	orrs r5, r5, r3
	str r5, [r6]

	ldr r6, =GPIOB_ODR // 6
	ldr r5, [r6]
	ldr r3, =#0x02
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	/*to ensure continuity, add small delay*/
	ldr r1,= #500
	bl delay

	subs r4, r4, #1
	cmp r4, #0
	bne myrand

	/* light up digit1 connected to PA7; light off digit2-3-4*/
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r5, =#0x0000
	ldr r3, =#0x80 // 1000 0000
	orrs r5, r5, r3
	str r5, [r6]

	/*remember that this is a common anode SSD*/
	/*light up the numbers in range (4,0)*/
	/* turn on led connected to PB1,PB2,PB5,PB6 in ODR */
	ldr r6, =GPIOB_ODR	// 4
	ldr r5, [r6]
	ldr r3, =#0x19
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	/*wait a bit for prevent flickering as soon as possible*/
	ldr r1,= #400000
	bl delay

	bl loop_d2d3d4

	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r5, =#0x0000
	ldr r3, =#0x80
	orrs r5, r5, r3
	str r5, [r6]

	/*wait a bit for prevent flickering as soon as possible*/
	ldr r1,= #400000
	bl delay

	/*From here on, the same operations will continue to be applied for each number (second)*/
	/* turn on led connected to PB0,PB1,PB2,PB3,PB6 in ODR */
	ldr r6, =GPIOB_ODR // 3
	ldr r5, [r6]
	ldr r3, =#0xB0
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	ldr r1,= #400000
	bl delay

	/*to provide miliseconds, branch to loop_d2d3d4 function for each numbers (for seconds)*/
	bl loop_d2d3d4

	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r5, =#0x0000
	ldr r3, =#0x80
	orrs r5, r5, r3
	str r5, [r6]

	ldr r1,= #400000
	bl delay

	/* turn on led connected to PB0,PB1,PB3,PB4,PB6 in ODR */
	ldr r6, =GPIOB_ODR // 2
	ldr r5, [r6]
	ldr r3, =#0x24
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	ldr r1,= #400000
	bl delay

	bl loop_d2d3d4

	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r5, =#0x0000
	ldr r3, =#0x80
	orrs r5, r5, r3
	str r5, [r6]

	ldr r1,= #400000
	bl delay

	/* turn on led connected to PB1,PB2 in ODR */
	ldr r6, =GPIOB_ODR // 1
	ldr r5, [r6]
	ldr r3, =#0xF9
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	ldr r1,= #400000
	bl delay

	bl loop_d2d3d4

	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r5, =#0x0000
	ldr r3, =#0x80
	orrs r5, r5, r3
	str r5, [r6]

	ldr r1,= #400000
	bl delay

	/* turn on led connected to PB0,PB1,PB2,PB3,PB4,PB5 in ODR */
	ldr r6, =GPIOB_ODR // 0
	ldr r5, [r6]
	ldr r3, =#0x40
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	ldr r1,= #400000
	bl delay

	bl loop_d2d3d4

	/*enable all the digits for displaying 0000*/
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r5, =#0x0000
	ldr r3, =#0xE1
	orrs r5, r5, r3
	str r5, [r6]

	/* light up the blue led and get 0000 from SSD. For this, make 1 the PB8 and PB6 pins. (To get 0000, G led of the SSD which is connected to PB6, must be 1(common anode).)*/
	ldr r6, =GPIOB_ODR
	ldr r5, [r6]
	ldr r5, =#0x0000
	ldr r3, =#0x140
	orrs r5, r5, r3
	str r5, [r6]

	/*display 0000 and wait a bit*/
	ldr r1,= #8000000
	bl delay

	b button_ctrl  // branch to button_ctrl function, for checking the next command which is taken from the board*/

/*For displaying my school number, school_num funciton works*/
school_num:
    /* light up digit1 connected to PA7; light off digit2-3-4*/
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r5, =#0x0000
	ldr r3, =#0x80
	orrs r5, r5, r3
	str r5, [r6]

	/*my school numbers first digit*/
	ldr r6, =GPIOB_ODR // 2
	ldr r5, [r6]
	ldr r3, =#0x24
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	/*to ensure continuity, add small delay*/
	ldr r1,= #500
	bl delay

	/*light up digit2 connected to PA6; light off digit1-3-4*/
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r5, =#0x0000
	ldr r3, =#0x40
	orrs r5, r5, r3
	str r5, [r6]

	/*my school numbers second digit*/
	ldr r6, =GPIOB_ODR // 0
	ldr r5, [r6]
	ldr r3, =#0x40
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	/*to ensure continuity, add small delay*/
	ldr r1,= #500
	bl delay

	/*light up digit3 connected to PA0; light off digit1-2-4*/
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r5, =#0x0000
	ldr r3, =#0x1
	orrs r5, r5, r3
	str r5, [r6]

	/*my school numbers third digit*/
	ldr r6, =GPIOB_ODR // 2
	ldr r5, [r6]
	ldr r3, =#0x24
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	/*to ensure continuity, add small delay*/
	ldr r1,= #500
	bl delay

	/*light up digit4 connected to PA5; light off digit1-2-3*/
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r5, =#0x0000
	ldr r3, =#0x20
	orrs r5, r5, r3
	str r5, [r6]

	/*my school numbers fourth digit*/
	ldr r6, =GPIOB_ODR // 2
	ldr r5, [r6]
	ldr r3, =#0x24
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	/*to ensure continuity, add small delay*/
	ldr r1,= #500
	bl delay

	b button_ctrl	// branch to button_ctrl function, for checking the next command which is taken from the board*/

	pop {pc}

loop_d2d3d4:
	push {r0-r7,lr}  // save registers

	/*enable digit2-3-4 for miliseconds, and disable digit1*/
	ldr r6, =GPIOA_ODR
	ldr r5, [r6]
	ldr r5, =#0x0000
	ldr r3, =#0x61
	orrs r5, r5, r3
	str r5, [r6]

	/*same process which is explained in loop_d1*/
	ldr r6, =GPIOB_ODR	// 9
	ldr r5, [r6]
	ldr r3, =#0x10
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	ldr r1,= #300000
	bl delay

	ldr r6, =GPIOB_ODR	// 8
	ldr r5, [r6]
	ldr r3, =#0x00
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	ldr r1,= #300000
	bl delay

	ldr r6, =GPIOB_ODR	// 7
	ldr r5, [r6]
	ldr r3, =#0xF8
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	ldr r1,= #300000
	bl delay

	ldr r6, =GPIOB_ODR // 6
	ldr r5, [r6]
	ldr r3, =#0x02
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	ldr r1,= #300000
	bl delay

	ldr r6, =GPIOB_ODR	// 5
	ldr r5, [r6]
	ldr r3, =#0x12
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	ldr r1,= #300000
	bl delay

	ldr r6, =GPIOB_ODR	// 4
	ldr r5, [r6]
	ldr r3, =#0x19
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	ldr r1,= #300000
	bl delay

	ldr r6, =GPIOB_ODR // 3
	ldr r5, [r6]
	ldr r3, =#0xB0
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	ldr r1,= #300000
	bl delay

	ldr r6, =GPIOB_ODR // 2
	ldr r5, [r6]
	ldr r3, =#0x24
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	ldr r1,= #300000
	bl delay

	ldr r6, =GPIOB_ODR // 1
	ldr r5, [r6]
	ldr r3, =#0xF9
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	ldr r1,= #300000
	bl delay

	ldr r6, =GPIOB_ODR // 0
	ldr r5, [r6]
	ldr r3, =#0x40
	ldr r5, =#0x0000
	orrs r5, r5, r3
	str r5, [r6]

	ldr r1,= #300000
	bl delay

	pop {r0-r7,pc}	// save registers and program counter to continue from loop_d1

delay:
	subs r1,r1,#1
	bne delay
	bx lr

	/* for(;;); */
	b .

	/* this should never get executed */
	nop

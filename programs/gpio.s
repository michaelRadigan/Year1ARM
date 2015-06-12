@Contains routines for manipulating GPIO ports


/*
* According to the EABI, all method calls should use r0-r3 for passing
* parameters, should preserve registers r4-r8,r10-r11,sp between calls, and 
* should return values in r0 (and r1 if needed). 
* It does also stipulate many things about how methods should use the registers
* and stack during calls, but we're using hand coded assembly. All we need to 
* do is obey the start and end conditions, and if all our methods do this, they
* would all work from C.
*/

/* NEW
* GetGpioAddress returns the base address of the GPIO region as a physical address
* in register r0.
* C++ Signature: void* GetGpioAddress()
*/
/*
.globl GetGpioAddress
GetGpioAddress: 
	gpioAddr .req r0
	ldr gpioAddr,=0x20200000
	mov pc,lr
	.unreq gpioAddr
*/
/* NEW
* SetGpioFunction sets the function of the GPIO register addressed by r0 to the
* low  3 bits of r1.
* C++ Signature: void SetGpioFunction(u32 gpioRegister, u32 function)
*/
/*
.globl SetGpioFunction
SetGpioFunction:
    pinNum .req r0
    pinFunc .req r1
	cmp pinNum,#53
	cmpls pinFunc,#7
	movhi pc,lr

	push {lr}
	mov r2,pinNum
	.unreq pinNum
	pinNum .req r2
	bl GetGpioAddress
	gpioAddr .req r0

	functionLoop$:
		cmp pinNum,#9
		subhi pinNum,#10
		addhi gpioAddr,#4
		bhi functionLoop$

	add pinNum, pinNum,lsl #1
	lsl pinFunc,pinNum

	mask .req r3
	mov mask,#7		*/			/* r3 = 111 in binary */
/*	lsl mask,pinNum		*/		/* r3 = 11100..00 where the 111 is in the same position as the function in r1 */
/*	.unreq pinNum

	mvn mask,mask	*/			/* r3 = 11..1100011..11 where the 000 is in the same poisiont as the function in r1 */
 /* oldFunc .req r2
	ldr oldFunc,[gpioAddr]	*/	/* r2 = existing code */
/*	and oldFunc,mask	*/		/* r2 = existing code with bits for this pin all 0 */
/*	.unreq mask       */

/*	orr pinFunc,oldFunc	*/		/* r1 = existing code with correct bits set */
/*	.unreq oldFunc */

/*	str pinFunc,[gpioAddr]
	.unreq pinFunc
	.unreq gpioAddr
	pop {pc}
*/
/* NEW
* SetGpio sets the GPIO pin addressed by register r0 high if r1 != 0 and low
* otherwise. 
* C++ Signature: void SetGpio(u32 gpioRegister, u32 value)
*/
/*
.globl SetGpio
SetGpio:	
    pinNum .req r0
    pinVal .req r1

	cmp pinNum,#53
	movhi pc,lr
	push {lr}
	mov r2,pinNum
    .unreq pinNum
    pinNum .req r2
	bl GetGpioAddress
    gpioAddr .req r0

	pinBank .req r3
	lsr pinBank,pinNum,#5
	lsl pinBank,#2
	add gpioAddr,pinBank
	.unreq pinBank

	and pinNum,#31
	setBit .req r3
	mov setBit,#1
	lsl setBit,pinNum
	.unreq pinNum

	teq pinVal,#0
	.unreq pinVal
	streq setBit,[gpioAddr,#40]
	strne setBit,[gpioAddr,#28]
	.unreq setBit
	.unreq gpioAddr
	pop {pc}

*/


.section .init
.globl flashACT

flashACT:
           ldr r0,=0x20200000    @Loads the physical address of the GPIO region into r0
           mov r1,#1             @r1 = 1
           lsl r1,#18            @logical shift left of the contents of r1 by 18 bits so r1 = 0x00040000
                                 @This hex value has bits 18-20 set to 001 which enables GPIO output to GPIO 16
           str r1,[r0,#4]        @Set the GPIO function select
           mov r1,#1             @r1 = 1
           lsl r1,#16            @logical shift left of the contents of r1 by 16 bits to set the 16th bit of r1
loop1$:
           str r1,[r0,#40]       @Address 40 - 48 is for pin off. Set GPIO 16 to low, causing LED to turn on  

@Creates a delay by moving the value 0x3F0000 into r2 and then subracting 1 from it and comparing


           mov r2,#0x3F0000
wait1$:
           sub r2,#1
           cmp r2,#0
           bne wait1$

           str r1, [r0, #28]    @Set GPIO 16 to high, causing LED to turn off

mov r2,#0x3F0000
wait2$:
	sub r2,#1
	cmp r2,#0
	bne wait2$



b loop1$              @Loop over this process forever


.section .init
.globl flashLED

flashLED:
           ldr r0,=0x20200000    @Loads the physical address of the GPIO region into r0
           mov r1,#1             @r1 = 1
           lsl r1,#0            @logical shift left of the contents of r1 by 18 bits so r1 = 0x00040000
                                 @This hex value has bits 18-20 set to 001 which enables GPIO output to GPIO 16
           str r1,[r0,#4]        @Set the GPIO function select
           mov r1,#1             @r1 = 1
           lsl r1,#16            @logical shift left of the contents of r1 by 16 bits to set the 16th bit of r1
loop2$:
           str r1,[r0,#40]       @Address 40 - 48 is for pin off. Set GPIO 16 to low, causing LED to turn on  

@Creates a delay by moving the value 0x3F0000 into r2 and then subracting 1 from it and comparing


           mov r2,#0x3F0000
wait21$:
           sub r2,#1
           cmp r2,#0
           bne wait21$

           str r1, [r0, #28]    @Set GPIO 16 to high, causing LED to turn off

mov r2,#0x3F0000
wait22$:
	sub r2,#1
	cmp r2,#0
	bne wait22$



b loop2$              @Loop over this process forever


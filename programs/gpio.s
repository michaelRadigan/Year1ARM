@Contains routines for manipulating GPIO ports


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
//loop1$:
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

       mov r15, r14

//b loop1$              @Loop over this process forever

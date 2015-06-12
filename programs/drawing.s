@Contains code to do with drawing shapes on the screen


/**
 * foreColour is the colour which all our methods will draw shapes in 
 * 
 * C signature
 * 
 * char foreColour
 */
.section .data
.align 1
foreColour:
           .hword 0xFFFF

/**
 * graphicsAddress stores the address of the frame buffer info infrastructure
 * 
 * C signature
 * 
 * FrameBufferDescription* graphicsAddress
 */
.align 2
graphicsAddress:
           .int 0

/**
 * SetForeColour changes the current drawing colour to the 16 bit colour in r0
 *
 * C signature
 *
 * void SetForeColour(uint16_t colour)
 */
.section .text
.globl SetForeColour
SetForeColour: 
           cmp r0, #0x10000
           movhi pc, lr
           moveq pc, lr
           
           ldr r1, =foreColour
           strh r0, [r1]
           mov pc, lr

/**
 * SetGraphicsAddress changes the current frame buffer info to graphicsAddress
 * 
 * C signature
 * 
 * void SetGraphicsAddress(FrameBufferDescription *value)
 */
.globl SetGraphicsAddress
SetGraphicsAddress: 
           ldr r1, =graphicsAddress  
           str r0, [r1]
           mov pc, lr

/**
 * DrawPixel draws a single pixel to the screen at the point in (r0, r1)
 * 
 * C signature
 *
 * void DrawPixel(uint32_t x, uint32_t y)
 */
.globl DrawPixel
DrawPixel:
           px .req r0
           py .req r1

                                  @Load in the graphics address
           addr .req r2
           ldr addr, =graphicsAddress
           ldr addr, [addr]
 
                                  @Check that the pixel coordinates are less than width and height 
           height .req r3
           ldr height, [addr, #4] @height is stored at an offset of 4 in the frame buffer description
           sub height, #1
           cmp py, height
           movhi pc, lr 
           .unreq height

           width .req r3
           ldr width, [addr, #0]  @width is stored at an offset of 0 in the frame buffer description 
           sub width, #1
           cmp px, width
           movhi pc, lr

                                  @Compute the address of the pixel to write
                                  @This code is specific to High Colour frame buffers
           ldr addr, [addr, #32]
           add width, #1
           mla px, py, width, px
           .unreq width
           .unreq py
           add addr, px, lsl #1
                             
                                  @Load in the foreColour. This is also high colour specific
           fore .req r3
           ldr fore, =foreColour
           ldrh fore, [fore]       
    
                                  @Store it at the address. This i salso high colour specific
           strh fore, [addr] 
           .unreq fore
           .unreq addr 
           mov pc, lr

/**
 * DrawLine draws a line between two points given in (r0,r1) and (r2,r3)
 * Uses Bresenham's Line Algortihm
 *
 * C signature
 *
 * void DrawLine(uint32_t p1x, uint32_t p1y, uint32_t px2, uint32_t px3);
 */
.globl DrawLine
DrawLine:
	push {r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
	x0 .req r9
	x1 .req r10
	y0 .req r11
	y1 .req r12

	mov x0,r0
	mov x1,r2
	mov y0,r1
	mov y1,r3

	dx .req r4
	dyn .req r5 /* Note that we only ever use -deltay, so I store its negative for speed. (hence dyn) */
	sx .req r6
	sy .req r7
	err .req r8

	cmp x0,x1
	subgt dx,x0,x1
	movgt sx,#-1
	suble dx,x1,x0
	movle sx,#1
	
	cmp y0,y1
	subgt dyn,y1,y0
	movgt sy,#-1
	suble dyn,y0,y1
	movle sy,#1

	add err,dx,dyn
	add x1,sx
	add y1,sy

	pixelLoop$:
		teq x0,x1
		teqne y0,y1
		popeq {r4,r5,r6,r7,r8,r9,r10,r11,r12,pc}

		mov r0,x0
		mov r1,y0
		bl DrawPixel

		cmp dyn, err,lsl #1
		addle err,dyn
		addle x0,sx

		cmp dx, err,lsl #1
		addge err,dx
		addge y0,sy
		b pixelLoop$

	.unreq x0
	.unreq x1
	.unreq y0
	.unreq y1
	.unreq dx
	.unreq dyn
	.unreq sx
	.unreq sy
	.unreq err

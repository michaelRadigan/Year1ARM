@main.s contains the main os


.section .init       @Entry point to main method
.globl _start            
 
_start:                    
           b main    @Branch to main code
                                                                          
.section .text       @Tells assembler to put this code with the rest


/**
 * This is the main method for the os
 * @param  void
 * @return void
 */
main:      
                              @Sets up stack pointer to default load address
                              @.init section is placed at default load address with the 
                              @.text section above it since the stack grows downwards

           mov sp, #0x8000

                              @Sets up the sreen using InitialiseFrameBuffer method to create 
                              @a frame buffer with width 1960, height 1080, and bit depth 16.
           
           mov r0, #1960
           mov r1, #1025
           mov r2, #16
           bl  InitialiseFrameBuffer    
           
                              @Check for a failed frame buffer. r0 will contain the result
                              @of the frame buffer set-up. 0 for success in which case we       
                              @turn on the ACT LED
           teq r0, #0
           bne no_error$
           
           mov r0, #16
           mov r1, #1
           bl  SetGpioFunction
           
           mov r0, #16
           mov r1, #1
           bl  SetGpio
  
  error$:  
           b  error$

  no_error$:
           fbInfoAddr  .req r4    @Aliasing the frame buffer info address stored in r4
           mov fbInfoAddr, r0     @Get the frame buffer pointer from it, to start drawing to the screen


                                  @Set pixels forever 
           
render$:
           fbAddr .req r3
           ldr fbAddr, [fbInfoAddr, #32]

           colour .req r0         @r0 will keep track of the current colour
           y .req r1
           mov y, #768
           draw_row$:
                      x .req r2
                      mov x, #1024
                      draw_pixels$:
                                    strh colour, [fbAddr]
                                    add fbAddr, #2
                                    sub x, #1
                                    teq x, #0
                                    bne draw_pixels$

                      sub y, #1
                      add colour, #1
                      teq y, #0
                      bne draw_row$ 
           b render$    
           
.unreq fbAddr
.unreq fbInfoAddr


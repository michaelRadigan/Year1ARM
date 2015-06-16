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
                              @a frame buffer with width 1024, height 768, and bit depth 32.
           
           mov r0, #1024
           mov r1, #768
           mov r2, #16                            //#32
           bl  InitialiseFrameBuffer    
           
                              @Check for a failed frame buffer. r0 will contain the result
                              @of the frame buffer set-up. Returns 0 for failure in which 
                              @case we do nothing. Flash ACT LED for success
           teq r0, #0
           bne no_error$
  error$:  
           b  error$

  no_error$:
           push {r0 - r4}
           bl flashACT
           pop {r0 - r4}
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
                                    ldr colour, =0xFFa1accb
                                    str colour, [fbAddr]
                                    add fbAddr, #4
                                    sub x, #1
                                    teq x, #0
                                    bne draw_pixels$

                      sub y, #1
                      add colour, #1
                      teq y, #0
                      bne draw_row$
           bl flashACT 
           b render$    
           
.unreq fbAddr
.unreq fbInfoAddr


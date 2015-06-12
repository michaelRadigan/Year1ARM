@frame_buffer creates and manipulates the frame buffer


@We describe the frame buffer that we would like and the graphics card will return 0 if it
@agrees to our request. Non-zero number will indicate failure


@When comunicating with the graphics card about frame buffers, a message consists of a 
@pointer to the structure below


/**
 * C signature
 * 
 * struct FrameBuferDescription {
 *   u32 width; u32 height; u32 vWidth; u32 vHeight; u32 pitch; 
 *   u32 bitDepth; u32 x; u32 y; void* pointer; u32 size;
 * };
 *
 * FrameBuferDescription FrameBufferInfo = { 1960, 1080, 1960, 1080, 0, 16, 0, 0, 0, 0 };
 */


.section .data      @C header file equvalent
.align 12           @Necessary to ensure correct communication with GPU which expects page alignment
.globl FrameBufferInfo
FrameBufferInfo:
    .int 1960 /* #0  physical width */    @GPU scales frame buffer to fit physical screen
    .int 1080 /* #4  physical height */   
    .int 1960 /* #8  virtual  width */    @Frame buffers dimensions
    .int 1080 /* #12 virtual  height */   
    .int 0    /* #16 GPU - Pitch */       @The number of bytes on each row - in this case 2 * 1024 = 2048 
    .int 16   /* #20 Bit depth */         @The number of bits to allocate to each pixel - in this case 16 means we are using High Colour mode
    .int 0    /* #24 X */                 @X offset for top left corner when copying frame buffer to screen
    .int 0    /* #28 Y */                 @Y offset for top right corner when copying frame buffer to screen
    .int 0    /* #32 GPU - Pointer */     @Pointer to frame buffer - filled in by graphics processor
    .int 0    /* #36 GPU - Size */        @Size is in bytes - filled in by graphics processor


/**
 * Creates a frame buffer of width and height specified in r0 and r1, 
 * and bit depth specified in r2 and returns a pointer of the buffer description 
 * 
 * C signature
 * 
 * FrameBuferDescription* InitialiseFrameBuffer(u32 width, u32 height, u32 bitDepth)
 */
.section .text
.globl InitialiseFrameBuffer
InitialiseFrameBuffer:
                                          @Validate our inputs
           width .req r0
           height .req r1
           bitDepth .req r2
           cmp width, #4096
           cmpls height, #4096
           cmpls bitDepth, #32
           result .req r0
           movhi result, #0
           movhi pc, lr

                                          @Write the inputs to the frame buffer 

           push {r4, lr}                  @Push r4 and link register to the stack as we 
                                          @need to store the frame buffer address in r4
           fbInfoAddr .req r4
           ldr fbInfoAddr, =FrameBufferInfo
           str width, [r4, #0]
           str height, [r4, #4]
           str width, [r4, #8]
           str height, [r4, #12]
           str bitDepth, [r4, #20]
           .unreq width
           .unreq height
           .unreq bitDepth
 
                                          @Send the address of the frame buffer + 0x40000000 to the mailbox.
                                                    
                                          @0x40000000 is a special code which tells the GPU not to use 
                                          @its cache for these writes so that we can see changes
           mov r0, fbInfoAddr             @The inputs to the MailboxWrite method are the value to write in r0        
           add r0, #0x40000000                        
           mov r1, #1                     @r1 contains the channel to write to           
           bl  MailboxWrite                        
                                                       
                                          @Receive the reply from the mailbox. 

           mov r0, #1                     @The inputs to the MailboxRead method is the channel to write to in r0, and the output is the value read
           bl MailboxRead

                                          @If the reply of MailboxRead method is not 0, the method has failed. Return 0 to indicate failure.
           
           teq result, #0
           movne result, #0
           popne {r4, pc} 

                                          @Return a pointer to the frame buffer info.  
           mov result, fbInfoAddr
           pop {r4, pc}
           .unreq result
           .unreq fbInfoAddr      

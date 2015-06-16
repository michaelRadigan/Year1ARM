@Contains the code which interacts with the mailbox for communication


/**
 * Returns the base address of the Mailbox region as a physical address
 * in register r0
 *
 * C signature
 *
 * void *GetMailboxBase()
 */
.globl GetMailboxBase
GetMailboxBase: 
           ldr r0, =0x2000B880            @Mailbox read address
           mov pc, lr     


/**
 * Writes the value given in the top 28 bits of r0 to the channel
 * given in the low 4 bits of r1
 *
 * C signature
 * 
 * void MailboxWrite(uint32_t value, uint8_t channel)
 */
.globl MailboxWrite
MailboxWrite:

                                        @This achieves our validation of r0 and r1. Checks that lowest 4 bits of the input in r0 are all 0
           tst r0, #0b1111
           movne pc, lr
           cmp r1, #15
           movhi pc, lr

                                        @This code ensures we will not overwrite our value, or link register and calls GetMailboxBase.
           channel .req r1
           value .req r2
           mov value, r0
           push {lr}
           bl GetMailboxBase
           mailbox .req r0

                                        @This code loads in the current status
  wait1$:  
           status .req r3
           ldr status, [mailbox, #0x18]  
          
                                        @This code checks that the top bit of the status field is 0, and loops back to wait if it is not.

           tst status, #0x80000000
           .unreq status
           bne wait1$

                                        @This code combines the channel and value together
           add value, channel
           .unreq channel

                                        @This code stores the result to the write field.
           str value, [mailbox, #0x20]
           .unreq value
           .unreq mailbox
           pop {pc}


/**
 * MailboxRead returns the current value in the mailbox addressed to a channel 
 * given in the low 4 bits of r0, as the top 28 bits of r0
 *
 * C signature
 *
 * uint32_t MailboxRead(uint8_t channel)
 *
 */
.globl MailboxRead
MailboxRead: 
                                      @Validate that the contents of r0 is a real mailbox
          cmp r0, #15
          movhi pc, lr
                                      @This code ensures we will not overwrite our value, or link register and calls GetMailboxBase
          channel .req r1
          mov channel, r0
          push {lr}
          bl GetMailboxBase
          mailbox .req r0

                                      @This code loads in the current status
  rightmail$:
    wait2$:
              status .req r2
              ldr status, [mailbox, #0x18]

                                      @This code checks that the 30th bit of the status field is 0, and loops back to wait2$. if it is not. 
                
              tst status, #0x40000000
              .unreq status
              bne wait2$

                                      @This code reads the next item from the mailbox
              mail .req r2
              ldr mail, [mailbox, #0]

                                      @This code checks that the channel of the mail we just read is the one we were supplied.If not loops back to rightmail
              inchan .req r3
              and inchan, mail, #0b1111
              teq inchan, channel
              .unreq inchan
              bne rightmail$
              .unreq mailbox
              .unreq channel

                                      @This code moves the answer (the top 28 bits of mail) to r0.
              and r0, mail, #0xFFFFFFF0
              .unreq mail
              pop {pc} 

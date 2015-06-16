@ARM Assembly Language Program to add some data and store the result in R3


@GCC linker is expecting a label “_start” for the entry point of the program. 
@This label also must be made global so that it is visible to the linker.

        .global _start
_start:  MOV R1, #0x25    @R1 = 0x25
         MOV R2, #0x34    @R2 = 0x34
		 ADD R3, R2, R1   @R3 = R2 +R1
      
	     MOV R7, #1
		 SVC 06           @The number 1 placed in Register 7 tells the operating system to terminate this program. 
                          @The instruction "SVC  0" is the system call, that transfers the program execution to the operating system. 
                          @If you place a different number in R7, the operating system will perform a difference service. 
						  @We will visit a system call to write to the console later.

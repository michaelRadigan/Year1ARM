################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
<<<<<<< HEAD
../src/cpu.o \
../src/memory_proc.o 
=======
../src/assemble.o 
>>>>>>> c3c8fe1ee2350412ee2bd124dc3639571f0c9a50

C_SRCS += \
../src/cpu.c \
../src/emulate.c \
../src/memory_proc.c \
../src/stack.c 

OBJS += \
./src/cpu.o \
./src/emulate.o \
./src/memory_proc.o \
./src/stack.o 

C_DEPS += \
./src/cpu.d \
./src/emulate.d \
./src/memory_proc.d \
./src/stack.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c99 -O0 -g3 -Wall -Werror -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



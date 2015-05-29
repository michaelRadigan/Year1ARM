################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cpu.c \
../src/emulate.c \
../src/main.c \
../src/memory_proc.c \
../src/stack.c 

OBJS += \
./src/cpu.o \
./src/emulate.o \
./src/main.o \
./src/memory_proc.o \
./src/stack.o 

C_DEPS += \
./src/cpu.d \
./src/emulate.d \
./src/main.d \
./src/memory_proc.d \
./src/stack.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



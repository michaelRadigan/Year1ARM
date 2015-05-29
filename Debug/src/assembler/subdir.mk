################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/assembler/assemble.c \
../src/assembler/dictionary.c \
../src/assembler/dictionaryOliverChanged.c \
../src/assembler/encode.c 

OBJS += \
./src/assembler/assemble.o \
./src/assembler/dictionary.o \
./src/assembler/dictionaryOliverChanged.o \
./src/assembler/encode.o 

C_DEPS += \
./src/assembler/assemble.d \
./src/assembler/dictionary.d \
./src/assembler/dictionaryOliverChanged.d \
./src/assembler/encode.d 


# Each subdirectory must supply rules for building sources it contributes
src/assembler/%.o: ../src/assembler/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c99 -O0 -g3 -Wall -Werror -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



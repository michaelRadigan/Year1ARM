################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/arm11_1415_testsuite/lib/json_pure-1.5.3/ext/json/ext/generator/generator.c 

OBJS += \
./src/arm11_1415_testsuite/lib/json_pure-1.5.3/ext/json/ext/generator/generator.o 

C_DEPS += \
./src/arm11_1415_testsuite/lib/json_pure-1.5.3/ext/json/ext/generator/generator.d 


# Each subdirectory must supply rules for building sources it contributes
src/arm11_1415_testsuite/lib/json_pure-1.5.3/ext/json/ext/generator/%.o: ../src/arm11_1415_testsuite/lib/json_pure-1.5.3/ext/json/ext/generator/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c99 -O0 -g3 -Wall -Werror -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



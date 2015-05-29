################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/arm11_1415_testsuite/lib/json_pure-1.5.3/ext/json/ext/parser/parser.c 

OBJS += \
./src/arm11_1415_testsuite/lib/json_pure-1.5.3/ext/json/ext/parser/parser.o 

C_DEPS += \
./src/arm11_1415_testsuite/lib/json_pure-1.5.3/ext/json/ext/parser/parser.d 


# Each subdirectory must supply rules for building sources it contributes
src/arm11_1415_testsuite/lib/json_pure-1.5.3/ext/json/ext/parser/%.o: ../src/arm11_1415_testsuite/lib/json_pure-1.5.3/ext/json/ext/parser/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



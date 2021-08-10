################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/aconv.c 

OBJS += \
./code/aconv.o 

C_DEPS += \
./code/aconv.d 


# Each subdirectory must supply rules for building sources it contributes
code/%.o: ../code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c11 -O2 -Wall -c -fmessage-length=0 -mavx512f -mavx512bw -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



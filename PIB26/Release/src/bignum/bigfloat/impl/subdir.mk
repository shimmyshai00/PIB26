################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/bignum/bigfloat/impl/logicalcrop.cpp 

OBJS += \
./src/bignum/bigfloat/impl/logicalcrop.o 

CPP_DEPS += \
./src/bignum/bigfloat/impl/logicalcrop.d 


# Each subdirectory must supply rules for building sources it contributes
src/bignum/bigfloat/impl/%.o: ../src/bignum/bigfloat/impl/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



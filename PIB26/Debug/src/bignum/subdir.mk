################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/bignum/BigFloat.cpp \
../src/bignum/BigInt.cpp 

OBJS += \
./src/bignum/BigFloat.o \
./src/bignum/BigInt.o 

CPP_DEPS += \
./src/bignum/BigFloat.d \
./src/bignum/BigInt.d 


# Each subdirectory must supply rules for building sources it contributes
src/bignum/%.o: ../src/bignum/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/bignum/newton/invsqrt.cpp \
../src/bignum/newton/recip.cpp 

OBJS += \
./src/bignum/newton/invsqrt.o \
./src/bignum/newton/recip.o 

CPP_DEPS += \
./src/bignum/newton/invsqrt.d \
./src/bignum/newton/recip.d 


# Each subdirectory must supply rules for building sources it contributes
src/bignum/newton/%.o: ../src/bignum/newton/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DNDEBUG -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



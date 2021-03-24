################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/bignum/multiplication/ClassicalSmallMul.cpp \
../src/bignum/multiplication/FFT.cpp \
../src/bignum/multiplication/FlexMul2.cpp \
../src/bignum/multiplication/FlexMul3.cpp \
../src/bignum/multiplication/SmallKaratsuba.cpp 

OBJS += \
./src/bignum/multiplication/ClassicalSmallMul.o \
./src/bignum/multiplication/FFT.o \
./src/bignum/multiplication/FlexMul2.o \
./src/bignum/multiplication/FlexMul3.o \
./src/bignum/multiplication/SmallKaratsuba.o 

CPP_DEPS += \
./src/bignum/multiplication/ClassicalSmallMul.d \
./src/bignum/multiplication/FFT.d \
./src/bignum/multiplication/FlexMul2.d \
./src/bignum/multiplication/FlexMul3.d \
./src/bignum/multiplication/SmallKaratsuba.d 


# Each subdirectory must supply rules for building sources it contributes
src/bignum/multiplication/%.o: ../src/bignum/multiplication/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



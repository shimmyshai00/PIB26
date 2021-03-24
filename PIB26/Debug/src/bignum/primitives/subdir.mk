################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/bignum/primitives/add.cpp \
../src/bignum/primitives/addsm.cpp \
../src/bignum/primitives/assign.cpp \
../src/bignum/primitives/compare.cpp \
../src/bignum/primitives/divsm.cpp \
../src/bignum/primitives/muladd.cpp \
../src/bignum/primitives/mulsm.cpp \
../src/bignum/primitives/shift.cpp \
../src/bignum/primitives/sub.cpp 

OBJS += \
./src/bignum/primitives/add.o \
./src/bignum/primitives/addsm.o \
./src/bignum/primitives/assign.o \
./src/bignum/primitives/compare.o \
./src/bignum/primitives/divsm.o \
./src/bignum/primitives/muladd.o \
./src/bignum/primitives/mulsm.o \
./src/bignum/primitives/shift.o \
./src/bignum/primitives/sub.o 

CPP_DEPS += \
./src/bignum/primitives/add.d \
./src/bignum/primitives/addsm.d \
./src/bignum/primitives/assign.d \
./src/bignum/primitives/compare.d \
./src/bignum/primitives/divsm.d \
./src/bignum/primitives/muladd.d \
./src/bignum/primitives/mulsm.d \
./src/bignum/primitives/shift.d \
./src/bignum/primitives/sub.d 


# Each subdirectory must supply rules for building sources it contributes
src/bignum/primitives/%.o: ../src/bignum/primitives/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/bignum/bigfloat/add.cpp \
../src/bignum/bigfloat/assign.cpp \
../src/bignum/bigfloat/compare.cpp \
../src/bignum/bigfloat/divsm.cpp \
../src/bignum/bigfloat/mul.cpp \
../src/bignum/bigfloat/mulsm.cpp \
../src/bignum/bigfloat/sub.cpp \
../src/bignum/bigfloat/uadd.cpp \
../src/bignum/bigfloat/uassign.cpp \
../src/bignum/bigfloat/ucompare.cpp \
../src/bignum/bigfloat/udivsm.cpp \
../src/bignum/bigfloat/umul.cpp \
../src/bignum/bigfloat/umulsm.cpp \
../src/bignum/bigfloat/usub.cpp 

OBJS += \
./src/bignum/bigfloat/add.o \
./src/bignum/bigfloat/assign.o \
./src/bignum/bigfloat/compare.o \
./src/bignum/bigfloat/divsm.o \
./src/bignum/bigfloat/mul.o \
./src/bignum/bigfloat/mulsm.o \
./src/bignum/bigfloat/sub.o \
./src/bignum/bigfloat/uadd.o \
./src/bignum/bigfloat/uassign.o \
./src/bignum/bigfloat/ucompare.o \
./src/bignum/bigfloat/udivsm.o \
./src/bignum/bigfloat/umul.o \
./src/bignum/bigfloat/umulsm.o \
./src/bignum/bigfloat/usub.o 

CPP_DEPS += \
./src/bignum/bigfloat/add.d \
./src/bignum/bigfloat/assign.d \
./src/bignum/bigfloat/compare.d \
./src/bignum/bigfloat/divsm.d \
./src/bignum/bigfloat/mul.d \
./src/bignum/bigfloat/mulsm.d \
./src/bignum/bigfloat/sub.d \
./src/bignum/bigfloat/uadd.d \
./src/bignum/bigfloat/uassign.d \
./src/bignum/bigfloat/ucompare.d \
./src/bignum/bigfloat/udivsm.d \
./src/bignum/bigfloat/umul.d \
./src/bignum/bigfloat/umulsm.d \
./src/bignum/bigfloat/usub.d 


# Each subdirectory must supply rules for building sources it contributes
src/bignum/bigfloat/%.o: ../src/bignum/bigfloat/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



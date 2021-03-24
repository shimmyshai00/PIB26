################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/bignum/bigint/add.cpp \
../src/bignum/bigint/assign.cpp \
../src/bignum/bigint/compare.cpp \
../src/bignum/bigint/divsm.cpp \
../src/bignum/bigint/mul.cpp \
../src/bignum/bigint/mulsm.cpp \
../src/bignum/bigint/sub.cpp \
../src/bignum/bigint/uadd.cpp \
../src/bignum/bigint/uassign.cpp \
../src/bignum/bigint/ucompare.cpp \
../src/bignum/bigint/udivsm.cpp \
../src/bignum/bigint/umul.cpp \
../src/bignum/bigint/umulsm.cpp \
../src/bignum/bigint/usub.cpp 

OBJS += \
./src/bignum/bigint/add.o \
./src/bignum/bigint/assign.o \
./src/bignum/bigint/compare.o \
./src/bignum/bigint/divsm.o \
./src/bignum/bigint/mul.o \
./src/bignum/bigint/mulsm.o \
./src/bignum/bigint/sub.o \
./src/bignum/bigint/uadd.o \
./src/bignum/bigint/uassign.o \
./src/bignum/bigint/ucompare.o \
./src/bignum/bigint/udivsm.o \
./src/bignum/bigint/umul.o \
./src/bignum/bigint/umulsm.o \
./src/bignum/bigint/usub.o 

CPP_DEPS += \
./src/bignum/bigint/add.d \
./src/bignum/bigint/assign.d \
./src/bignum/bigint/compare.d \
./src/bignum/bigint/divsm.d \
./src/bignum/bigint/mul.d \
./src/bignum/bigint/mulsm.d \
./src/bignum/bigint/sub.d \
./src/bignum/bigint/uadd.d \
./src/bignum/bigint/uassign.d \
./src/bignum/bigint/ucompare.d \
./src/bignum/bigint/udivsm.d \
./src/bignum/bigint/umul.d \
./src/bignum/bigint/umulsm.d \
./src/bignum/bigint/usub.d 


# Each subdirectory must supply rules for building sources it contributes
src/bignum/bigint/%.o: ../src/bignum/bigint/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



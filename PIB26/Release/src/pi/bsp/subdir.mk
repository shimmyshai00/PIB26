################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/pi/bsp/bsp.cpp \
../src/pi/bsp/chudnovsky.cpp 

OBJS += \
./src/pi/bsp/bsp.o \
./src/pi/bsp/chudnovsky.o 

CPP_DEPS += \
./src/pi/bsp/bsp.d \
./src/pi/bsp/chudnovsky.d 


# Each subdirectory must supply rules for building sources it contributes
src/pi/bsp/%.o: ../src/pi/bsp/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



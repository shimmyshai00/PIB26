################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/newton/recip.cpp 

OBJS += \
./src/newton/recip.o 

CPP_DEPS += \
./src/newton/recip.d 


# Each subdirectory must supply rules for building sources it contributes
src/newton/%.o: ../src/newton/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



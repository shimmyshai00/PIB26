################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/util/DotsTicker.cpp \
../src/util/LabelTicker.cpp \
../src/util/printB26.cpp \
../src/util/timer.cpp \
../src/util/userInput.cpp 

OBJS += \
./src/util/DotsTicker.o \
./src/util/LabelTicker.o \
./src/util/printB26.o \
./src/util/timer.o \
./src/util/userInput.o 

CPP_DEPS += \
./src/util/DotsTicker.d \
./src/util/LabelTicker.d \
./src/util/printB26.d \
./src/util/timer.d \
./src/util/userInput.d 


# Each subdirectory must supply rules for building sources it contributes
src/util/%.o: ../src/util/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



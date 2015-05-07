################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/codec/avi/EncodeMod.cpp \
../src/codec/avi/encode_file.cpp 

OBJS += \
./src/codec/avi/EncodeMod.o \
./src/codec/avi/encode_file.o 

CPP_DEPS += \
./src/codec/avi/EncodeMod.d \
./src/codec/avi/encode_file.d 


# Each subdirectory must supply rules for building sources it contributes
src/codec/avi/%.o: ../src/codec/avi/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__STDC_CONSTANT_MACROS -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



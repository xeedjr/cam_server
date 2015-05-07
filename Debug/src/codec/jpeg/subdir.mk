################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/codec/jpeg/JPEGReader.cpp \
../src/codec/jpeg/JPEGWriter.cpp \
../src/codec/jpeg/jpeg_codec.cpp \
../src/codec/jpeg/jpeg_to_yuv.cpp 

OBJS += \
./src/codec/jpeg/JPEGReader.o \
./src/codec/jpeg/JPEGWriter.o \
./src/codec/jpeg/jpeg_codec.o \
./src/codec/jpeg/jpeg_to_yuv.o 

CPP_DEPS += \
./src/codec/jpeg/JPEGReader.d \
./src/codec/jpeg/JPEGWriter.d \
./src/codec/jpeg/jpeg_codec.d \
./src/codec/jpeg/jpeg_to_yuv.d 


# Each subdirectory must supply rules for building sources it contributes
src/codec/jpeg/%.o: ../src/codec/jpeg/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__STDC_CONSTANT_MACROS -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



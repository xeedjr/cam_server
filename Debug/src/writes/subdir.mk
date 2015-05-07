################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/writes/database.cpp \
../src/writes/jpeg_file_writer.cpp 

OBJS += \
./src/writes/database.o \
./src/writes/jpeg_file_writer.o 

CPP_DEPS += \
./src/writes/database.d \
./src/writes/jpeg_file_writer.d 


# Each subdirectory must supply rules for building sources it contributes
src/writes/%.o: ../src/writes/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__STDC_CONSTANT_MACROS -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



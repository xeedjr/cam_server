################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/device/FaceCam.cpp \
../src/device/dlink_camera.cpp \
../src/device/edimax_camera.cpp 

OBJS += \
./src/device/FaceCam.o \
./src/device/dlink_camera.o \
./src/device/edimax_camera.o 

CPP_DEPS += \
./src/device/FaceCam.d \
./src/device/dlink_camera.d \
./src/device/edimax_camera.d 


# Each subdirectory must supply rules for building sources it contributes
src/device/%.o: ../src/device/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__STDC_CONSTANT_MACROS -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



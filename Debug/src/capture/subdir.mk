################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/capture/alsa_audio_capture.cpp \
../src/capture/url_audio_capture.cpp \
../src/capture/url_mjpeg_capture.cpp \
../src/capture/v4l_mjpeg_capture.cpp 

OBJS += \
./src/capture/alsa_audio_capture.o \
./src/capture/url_audio_capture.o \
./src/capture/url_mjpeg_capture.o \
./src/capture/v4l_mjpeg_capture.o 

CPP_DEPS += \
./src/capture/alsa_audio_capture.d \
./src/capture/url_audio_capture.d \
./src/capture/url_mjpeg_capture.d \
./src/capture/v4l_mjpeg_capture.d 


# Each subdirectory must supply rules for building sources it contributes
src/capture/%.o: ../src/capture/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__STDC_CONSTANT_MACROS -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



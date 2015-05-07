################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/device/capture/V4LYUVCapture.cpp \
../src/device/capture/alsa_audio_capture.cpp \
../src/device/capture/url_audio_capture.cpp \
../src/device/capture/url_mjpeg_capture.cpp \
../src/device/capture/v4l_mjpeg_capture.cpp 

C_SRCS += \
../src/device/capture/v4l2uvc.c 

OBJS += \
./src/device/capture/V4LYUVCapture.o \
./src/device/capture/alsa_audio_capture.o \
./src/device/capture/url_audio_capture.o \
./src/device/capture/url_mjpeg_capture.o \
./src/device/capture/v4l2uvc.o \
./src/device/capture/v4l_mjpeg_capture.o 

C_DEPS += \
./src/device/capture/v4l2uvc.d 

CPP_DEPS += \
./src/device/capture/V4LYUVCapture.d \
./src/device/capture/alsa_audio_capture.d \
./src/device/capture/url_audio_capture.d \
./src/device/capture/url_mjpeg_capture.d \
./src/device/capture/v4l_mjpeg_capture.d 


# Each subdirectory must supply rules for building sources it contributes
src/device/capture/%.o: ../src/device/capture/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__STDC_CONSTANT_MACROS -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/device/capture/%.o: ../src/device/capture/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



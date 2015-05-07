################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/conteiners/RIFF_conteiner.cpp \
../src/conteiners/conteiner.cpp \
../src/conteiners/jpeg_conteiner.cpp \
../src/conteiners/yuv_conteiner.cpp 

OBJS += \
./src/conteiners/RIFF_conteiner.o \
./src/conteiners/conteiner.o \
./src/conteiners/jpeg_conteiner.o \
./src/conteiners/yuv_conteiner.o 

CPP_DEPS += \
./src/conteiners/RIFF_conteiner.d \
./src/conteiners/conteiner.d \
./src/conteiners/jpeg_conteiner.d \
./src/conteiners/yuv_conteiner.d 


# Each subdirectory must supply rules for building sources it contributes
src/conteiners/%.o: ../src/conteiners/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__STDC_CONSTANT_MACROS -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



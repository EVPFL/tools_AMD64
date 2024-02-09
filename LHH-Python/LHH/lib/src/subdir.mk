################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/HomHash.cpp \
../src/LHHValue.cpp \
../src/LHHHash.cpp \
../src/LHHScheme.cpp

OBJS += \
./src/HomHash.o \
./src/LHHValue.o \
./src/LHHHash.o \
./src/LHHScheme.o

CPP_DEPS += \
./src/HomHash.d \
./src/LHHValue.d \
./src/LHHHash.d \
./src/LHHScheme.d

CFLAGS = -O3 -mtune=native -ggdb -fPIC -std=c++11 -pthread
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++  ${CFLAGS} -I ../src -L /usr/local/lib/libcrypto.so -L /usr/local/lib/libssl.so -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

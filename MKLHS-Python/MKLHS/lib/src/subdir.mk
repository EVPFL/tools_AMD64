################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/SecretKey.cpp \
../src/PublicKey.cpp \
../src/Message.cpp \
../src/Params.cpp \
../src/Signature.cpp \
../src/Linearity.cpp \
../src/Scheme.cpp

OBJS += \
./src/SecretKey.o \
./src/PublicKey.o \
./src/Message.o \
./src/Params.o \
./src/Signature.o \
./src/Linearity.o \
./src/Scheme.o

CPP_DEPS += \
./src/SecretKey.d \
./src/PublicKey.d \
./src/Message.d \
./src/Params.d \
./src/Signature.d \
./src/Linearity.d \
./src/Scheme.d

CFLAGS = -O3 -mtune=native -ggdb -fPIC -std=c++11 -pthread
# RELIC_SRC = ../relic/src
# RELIC_TARGET = ../relic/target
# MKLHS_SRC = ../src
#g++ -I/usr/local/include -O3 -c -fPIC -std=c++11 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
#	g++ ${CFLAGS} -I ${RELIC_SRC}/include -I ${RELIC_TARGET}/include -I ${MKLHS_SRC} -c "$<" -o "$@"
# 	g++ ${CFLAGS} -o "$@" ${RELIC_TARGET}/lib/librelic_s.a -lgmp -lm  "$<"
# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++  ${CFLAGS} -I /usr/local/include -I ../relic/target/include -I ../relic/src/include -I ../src -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

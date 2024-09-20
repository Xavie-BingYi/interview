################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ELF_SRCS += \
../Src/interview.elf 

S_SRCS += \
../Src/ContextSwitch.s 

C_SRCS += \
../Src/gpio.c \
../Src/io.c \
../Src/main.c \
../Src/uart.c 

OBJS += \
./Src/ContextSwitch.o \
./Src/gpio.o \
./Src/io.o \
./Src/main.o \
./Src/uart.o 

S_DEPS += \
./Src/ContextSwitch.d 

C_DEPS += \
./Src/gpio.d \
./Src/io.d \
./Src/main.d \
./Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.s Src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/ContextSwitch.d ./Src/ContextSwitch.o ./Src/gpio.cyclo ./Src/gpio.d ./Src/gpio.o ./Src/gpio.su ./Src/io.cyclo ./Src/io.d ./Src/io.o ./Src/io.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/uart.cyclo ./Src/uart.d ./Src/uart.o ./Src/uart.su

.PHONY: clean-Src


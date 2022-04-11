################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/bsctmr.c \
../Src/dma.c \
../Src/gpio.c \
../Src/i2c.c \
../Src/main.c \
../Src/nvic.c \
../Src/rcc.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/systick.c \
../Src/usart.c 

OBJS += \
./Src/bsctmr.o \
./Src/dma.o \
./Src/gpio.o \
./Src/i2c.o \
./Src/main.o \
./Src/nvic.o \
./Src/rcc.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/systick.o \
./Src/usart.o 

C_DEPS += \
./Src/bsctmr.d \
./Src/dma.d \
./Src/gpio.d \
./Src/i2c.d \
./Src/main.d \
./Src/nvic.d \
./Src/rcc.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/systick.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F746NGHx -DSTM32F7 -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/bsctmr.d ./Src/bsctmr.o ./Src/dma.d ./Src/dma.o ./Src/gpio.d ./Src/gpio.o ./Src/i2c.d ./Src/i2c.o ./Src/main.d ./Src/main.o ./Src/nvic.d ./Src/nvic.o ./Src/rcc.d ./Src/rcc.o ./Src/syscalls.d ./Src/syscalls.o ./Src/sysmem.d ./Src/sysmem.o ./Src/systick.d ./Src/systick.o ./Src/usart.d ./Src/usart.o

.PHONY: clean-Src


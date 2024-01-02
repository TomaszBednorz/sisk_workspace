################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Lib/app_can.c \
../Core/Lib/app_imu.c \
../Core/Lib/app_light.c \
../Core/Lib/app_system.c \
../Core/Lib/app_uart.c 

OBJS += \
./Core/Lib/app_can.o \
./Core/Lib/app_imu.o \
./Core/Lib/app_light.o \
./Core/Lib/app_system.o \
./Core/Lib/app_uart.o 

C_DEPS += \
./Core/Lib/app_can.d \
./Core/Lib/app_imu.d \
./Core/Lib/app_light.d \
./Core/Lib/app_system.d \
./Core/Lib/app_uart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Lib/%.o Core/Lib/%.su: ../Core/Lib/%.c Core/Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Core/Lib -I../Core/Lib/LSM6DSO -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Lib

clean-Core-2f-Lib:
	-$(RM) ./Core/Lib/app_can.d ./Core/Lib/app_can.o ./Core/Lib/app_can.su ./Core/Lib/app_imu.d ./Core/Lib/app_imu.o ./Core/Lib/app_imu.su ./Core/Lib/app_light.d ./Core/Lib/app_light.o ./Core/Lib/app_light.su ./Core/Lib/app_system.d ./Core/Lib/app_system.o ./Core/Lib/app_system.su ./Core/Lib/app_uart.d ./Core/Lib/app_uart.o ./Core/Lib/app_uart.su

.PHONY: clean-Core-2f-Lib


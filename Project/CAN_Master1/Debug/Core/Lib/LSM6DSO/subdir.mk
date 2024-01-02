################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Lib/LSM6DSO/lsm6dso.c \
../Core/Lib/LSM6DSO/lsm6dso_com.c 

OBJS += \
./Core/Lib/LSM6DSO/lsm6dso.o \
./Core/Lib/LSM6DSO/lsm6dso_com.o 

C_DEPS += \
./Core/Lib/LSM6DSO/lsm6dso.d \
./Core/Lib/LSM6DSO/lsm6dso_com.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Lib/LSM6DSO/%.o Core/Lib/LSM6DSO/%.su: ../Core/Lib/LSM6DSO/%.c Core/Lib/LSM6DSO/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Core/Lib -I../Core/Lib/LSM6DSO -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Lib-2f-LSM6DSO

clean-Core-2f-Lib-2f-LSM6DSO:
	-$(RM) ./Core/Lib/LSM6DSO/lsm6dso.d ./Core/Lib/LSM6DSO/lsm6dso.o ./Core/Lib/LSM6DSO/lsm6dso.su ./Core/Lib/LSM6DSO/lsm6dso_com.d ./Core/Lib/LSM6DSO/lsm6dso_com.o ./Core/Lib/LSM6DSO/lsm6dso_com.su

.PHONY: clean-Core-2f-Lib-2f-LSM6DSO


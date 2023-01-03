################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../icode/buzzer/buzzer.c 

OBJS += \
./icode/buzzer/buzzer.o 

C_DEPS += \
./icode/buzzer/buzzer.d 


# Each subdirectory must supply rules for building sources it contributes
icode/buzzer/%.o: ../icode/buzzer/%.c icode/buzzer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-icode-2f-buzzer

clean-icode-2f-buzzer:
	-$(RM) ./icode/buzzer/buzzer.d ./icode/buzzer/buzzer.o

.PHONY: clean-icode-2f-buzzer


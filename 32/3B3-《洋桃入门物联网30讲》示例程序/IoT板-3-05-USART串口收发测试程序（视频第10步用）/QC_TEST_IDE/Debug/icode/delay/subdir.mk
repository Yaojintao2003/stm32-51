################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../icode/delay/delay.c 

OBJS += \
./icode/delay/delay.o 

C_DEPS += \
./icode/delay/delay.d 


# Each subdirectory must supply rules for building sources it contributes
icode/delay/%.o icode/delay/%.su: ../icode/delay/%.c icode/delay/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-icode-2f-delay

clean-icode-2f-delay:
	-$(RM) ./icode/delay/delay.d ./icode/delay/delay.o ./icode/delay/delay.su

.PHONY: clean-icode-2f-delay


################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../icode/adc/adc.c 

OBJS += \
./icode/adc/adc.o 

C_DEPS += \
./icode/adc/adc.d 


# Each subdirectory must supply rules for building sources it contributes
icode/adc/%.o icode/adc/%.su: ../icode/adc/%.c icode/adc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-icode-2f-adc

clean-icode-2f-adc:
	-$(RM) ./icode/adc/adc.d ./icode/adc/adc.o ./icode/adc/adc.su

.PHONY: clean-icode-2f-adc


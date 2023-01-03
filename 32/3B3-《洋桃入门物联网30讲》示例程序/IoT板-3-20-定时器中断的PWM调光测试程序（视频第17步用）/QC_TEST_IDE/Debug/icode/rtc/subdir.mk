################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../icode/rtc/rtc.c 

OBJS += \
./icode/rtc/rtc.o 

C_DEPS += \
./icode/rtc/rtc.d 


# Each subdirectory must supply rules for building sources it contributes
icode/rtc/%.o icode/rtc/%.su: ../icode/rtc/%.c icode/rtc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-icode-2f-rtc

clean-icode-2f-rtc:
	-$(RM) ./icode/rtc/rtc.d ./icode/rtc/rtc.o ./icode/rtc/rtc.su

.PHONY: clean-icode-2f-rtc


################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Peripheral/damper.c \
../Core/Src/Peripheral/fan.c \
../Core/Src/Peripheral/led_button_control.c \
../Core/Src/Peripheral/ten.c 

OBJS += \
./Core/Src/Peripheral/damper.o \
./Core/Src/Peripheral/fan.o \
./Core/Src/Peripheral/led_button_control.o \
./Core/Src/Peripheral/ten.o 

C_DEPS += \
./Core/Src/Peripheral/damper.d \
./Core/Src/Peripheral/fan.d \
./Core/Src/Peripheral/led_button_control.d \
./Core/Src/Peripheral/ten.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Peripheral/%.o Core/Src/Peripheral/%.su Core/Src/Peripheral/%.cyclo: ../Core/Src/Peripheral/%.c Core/Src/Peripheral/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/Project/Stm32f407/Core/Src/Mqtt" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I"D:/Project/Stm32f407/Core/Inc/XGZP6897D" -I"D:/Project/Stm32f407/Core/Inc/cJSON" -I"D:/Project/Stm32f407/Core/Inc/Ds18b20" -I"D:/Project/Stm32f407/Core/Inc/Epm_24c32" -I"D:/Project/Stm32f407/Core/Inc/FLASH_SECTOR_F4" -I"D:/Project/Stm32f407/Core/Inc/MODBUS-LIB" -I"D:/Project/Stm32f407/Core/Inc/Sht30" -I"D:/Project/Stm32f407/Core/Inc/W25qxx" -I"D:/Project/Stm32f407/Core/Src/Peripheral" -I"D:/Project/Stm32f407/Core/Src/Remote_control" -I"D:/Project/Stm32f407/Core/Src/Sensor" -I"D:/Project/Stm32f407/Core/Src/Storage" -I"D:/Project/Stm32f407/Core/Src/Modes" -I../Middlewares/Third_Party/LwIP/src/apps/http -I"D:/Project/Stm32f407/Core/Src/Httpserver" -I"D:/Project/Stm32f407/Middlewares/Third_Party/LwIP/src/apps/mdns" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"D:/Project/Stm32f407/Core/Src/Ota" -I"D:/Project/Stm32f407/Core/Src/Bootloader" -O0 -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Peripheral

clean-Core-2f-Src-2f-Peripheral:
	-$(RM) ./Core/Src/Peripheral/damper.cyclo ./Core/Src/Peripheral/damper.d ./Core/Src/Peripheral/damper.o ./Core/Src/Peripheral/damper.su ./Core/Src/Peripheral/fan.cyclo ./Core/Src/Peripheral/fan.d ./Core/Src/Peripheral/fan.o ./Core/Src/Peripheral/fan.su ./Core/Src/Peripheral/led_button_control.cyclo ./Core/Src/Peripheral/led_button_control.d ./Core/Src/Peripheral/led_button_control.o ./Core/Src/Peripheral/led_button_control.su ./Core/Src/Peripheral/ten.cyclo ./Core/Src/Peripheral/ten.d ./Core/Src/Peripheral/ten.o ./Core/Src/Peripheral/ten.su

.PHONY: clean-Core-2f-Src-2f-Peripheral


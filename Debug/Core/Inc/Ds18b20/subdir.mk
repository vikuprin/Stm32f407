################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/Ds18b20/ds18b20.c \
../Core/Inc/Ds18b20/dwt.c \
../Core/Inc/Ds18b20/onewire.c 

OBJS += \
./Core/Inc/Ds18b20/ds18b20.o \
./Core/Inc/Ds18b20/dwt.o \
./Core/Inc/Ds18b20/onewire.o 

C_DEPS += \
./Core/Inc/Ds18b20/ds18b20.d \
./Core/Inc/Ds18b20/dwt.d \
./Core/Inc/Ds18b20/onewire.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/Ds18b20/%.o Core/Inc/Ds18b20/%.su Core/Inc/Ds18b20/%.cyclo: ../Core/Inc/Ds18b20/%.c Core/Inc/Ds18b20/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Project/Stm32f407/Core/Src/Mqtt" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I"C:/Project/Stm32f407/Core/Inc/XGZP6897D" -I"C:/Project/Stm32f407/Core/Inc/Aht20" -I"C:/Project/Stm32f407/Core/Inc/cJSON" -I"C:/Project/Stm32f407/Core/Inc/Ds18b20" -I"C:/Project/Stm32f407/Core/Inc/Epm_24c32" -I"C:/Project/Stm32f407/Core/Inc/FLASH_SECTOR_F4" -I"C:/Project/Stm32f407/Core/Inc/MODBUS-LIB" -I"C:/Project/Stm32f407/Core/Inc/Sht30" -I"C:/Project/Stm32f407/Core/Inc/W25qxx" -I"C:/Project/Stm32f407/Core/Src/Peripheral" -I"C:/Project/Stm32f407/Core/Src/Remote_control" -I"C:/Project/Stm32f407/Core/Src/Sensor" -I"C:/Project/Stm32f407/Core/Src/Storage" -I"C:/Project/Stm32f407/Core/Src/Modes" -I../Middlewares/Third_Party/LwIP/src/apps/http -I"C:/Project/Stm32f407/Core/Src/Httpserver" -O0 -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-Ds18b20

clean-Core-2f-Inc-2f-Ds18b20:
	-$(RM) ./Core/Inc/Ds18b20/ds18b20.cyclo ./Core/Inc/Ds18b20/ds18b20.d ./Core/Inc/Ds18b20/ds18b20.o ./Core/Inc/Ds18b20/ds18b20.su ./Core/Inc/Ds18b20/dwt.cyclo ./Core/Inc/Ds18b20/dwt.d ./Core/Inc/Ds18b20/dwt.o ./Core/Inc/Ds18b20/dwt.su ./Core/Inc/Ds18b20/onewire.cyclo ./Core/Inc/Ds18b20/onewire.d ./Core/Inc/Ds18b20/onewire.o ./Core/Inc/Ds18b20/onewire.su

.PHONY: clean-Core-2f-Inc-2f-Ds18b20


################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LWIP/Target/ethernetif.c 

OBJS += \
./LWIP/Target/ethernetif.o 

C_DEPS += \
./LWIP/Target/ethernetif.d 


# Each subdirectory must supply rules for building sources it contributes
LWIP/Target/%.o LWIP/Target/%.su LWIP/Target/%.cyclo: ../LWIP/Target/%.c LWIP/Target/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Src/Mqtt" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Inc/XGZP6897D" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Inc/Aht20" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Inc/cJSON" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Inc/Ds18b20" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Inc/FLASH_SECTOR_F4" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Inc/MODBUS-LIB" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Inc/Sht30" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Inc/W25qxx" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Src/Peripheral" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Src/Remote_control" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Src/Sensor" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Src/Storage" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Src/Modes" -I../Middlewares/Third_Party/LwIP/src/apps/http -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Src/Httpserver" -I"C:/Microcontroller/VAKIO/Stm32f407/Middlewares/Third_Party/LwIP/src/apps/mdns" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Src/Ota" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Src/Bootloader" -O0 -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LWIP-2f-Target

clean-LWIP-2f-Target:
	-$(RM) ./LWIP/Target/ethernetif.cyclo ./LWIP/Target/ethernetif.d ./LWIP/Target/ethernetif.o ./LWIP/Target/ethernetif.su

.PHONY: clean-LWIP-2f-Target


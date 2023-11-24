################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Mqtt/master_topic.c \
../Core/Src/Mqtt/mode_topic.c \
../Core/Src/Mqtt/mqtt_client.c \
../Core/Src/Mqtt/mqtt_message.c \
../Core/Src/Mqtt/mqtt_objects.c \
../Core/Src/Mqtt/send_server_handler.c \
../Core/Src/Mqtt/system_topic.c 

OBJS += \
./Core/Src/Mqtt/master_topic.o \
./Core/Src/Mqtt/mode_topic.o \
./Core/Src/Mqtt/mqtt_client.o \
./Core/Src/Mqtt/mqtt_message.o \
./Core/Src/Mqtt/mqtt_objects.o \
./Core/Src/Mqtt/send_server_handler.o \
./Core/Src/Mqtt/system_topic.o 

C_DEPS += \
./Core/Src/Mqtt/master_topic.d \
./Core/Src/Mqtt/mode_topic.d \
./Core/Src/Mqtt/mqtt_client.d \
./Core/Src/Mqtt/mqtt_message.d \
./Core/Src/Mqtt/mqtt_objects.d \
./Core/Src/Mqtt/send_server_handler.d \
./Core/Src/Mqtt/system_topic.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Mqtt/%.o Core/Src/Mqtt/%.su Core/Src/Mqtt/%.cyclo: ../Core/Src/Mqtt/%.c Core/Src/Mqtt/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/Project/Stm32f407/Core/Src/Mqtt" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I"D:/Project/Stm32f407/Core/Inc/XGZP6897D" -I"D:/Project/Stm32f407/Core/Inc/Aht20" -I"D:/Project/Stm32f407/Core/Inc/cJSON" -I"D:/Project/Stm32f407/Core/Inc/Ds18b20" -I"D:/Project/Stm32f407/Core/Inc/Epm_24c32" -I"D:/Project/Stm32f407/Core/Inc/FLASH_SECTOR_F4" -I"D:/Project/Stm32f407/Core/Inc/MODBUS-LIB" -I"D:/Project/Stm32f407/Core/Inc/Sht30" -I"D:/Project/Stm32f407/Core/Inc/W25qxx" -I"D:/Project/Stm32f407/Core/Src/Peripheral" -I"D:/Project/Stm32f407/Core/Src/Remote_control" -I"D:/Project/Stm32f407/Core/Src/Sensor" -I"D:/Project/Stm32f407/Core/Src/Storage" -I"D:/Project/Stm32f407/Core/Src/Modes" -I../Middlewares/Third_Party/LwIP/src/apps/http -I"D:/Project/Stm32f407/Core/Src/Httpserver" -I"D:/Project/Stm32f407/Middlewares/Third_Party/LwIP/src/apps/mdns" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"D:/Project/Stm32f407/Core/Src/Ota" -O0 -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Mqtt

clean-Core-2f-Src-2f-Mqtt:
	-$(RM) ./Core/Src/Mqtt/master_topic.cyclo ./Core/Src/Mqtt/master_topic.d ./Core/Src/Mqtt/master_topic.o ./Core/Src/Mqtt/master_topic.su ./Core/Src/Mqtt/mode_topic.cyclo ./Core/Src/Mqtt/mode_topic.d ./Core/Src/Mqtt/mode_topic.o ./Core/Src/Mqtt/mode_topic.su ./Core/Src/Mqtt/mqtt_client.cyclo ./Core/Src/Mqtt/mqtt_client.d ./Core/Src/Mqtt/mqtt_client.o ./Core/Src/Mqtt/mqtt_client.su ./Core/Src/Mqtt/mqtt_message.cyclo ./Core/Src/Mqtt/mqtt_message.d ./Core/Src/Mqtt/mqtt_message.o ./Core/Src/Mqtt/mqtt_message.su ./Core/Src/Mqtt/mqtt_objects.cyclo ./Core/Src/Mqtt/mqtt_objects.d ./Core/Src/Mqtt/mqtt_objects.o ./Core/Src/Mqtt/mqtt_objects.su ./Core/Src/Mqtt/send_server_handler.cyclo ./Core/Src/Mqtt/send_server_handler.d ./Core/Src/Mqtt/send_server_handler.o ./Core/Src/Mqtt/send_server_handler.su ./Core/Src/Mqtt/system_topic.cyclo ./Core/Src/Mqtt/system_topic.d ./Core/Src/Mqtt/system_topic.o ./Core/Src/Mqtt/system_topic.su

.PHONY: clean-Core-2f-Src-2f-Mqtt


################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/src/api/api_lib.c \
../Middlewares/Third_Party/LwIP/src/api/api_msg.c \
../Middlewares/Third_Party/LwIP/src/api/err.c \
../Middlewares/Third_Party/LwIP/src/api/if_api.c \
../Middlewares/Third_Party/LwIP/src/api/netbuf.c \
../Middlewares/Third_Party/LwIP/src/api/netdb.c \
../Middlewares/Third_Party/LwIP/src/api/netifapi.c \
../Middlewares/Third_Party/LwIP/src/api/sockets.c \
../Middlewares/Third_Party/LwIP/src/api/tcpip.c 

OBJS += \
./Middlewares/Third_Party/LwIP/src/api/api_lib.o \
./Middlewares/Third_Party/LwIP/src/api/api_msg.o \
./Middlewares/Third_Party/LwIP/src/api/err.o \
./Middlewares/Third_Party/LwIP/src/api/if_api.o \
./Middlewares/Third_Party/LwIP/src/api/netbuf.o \
./Middlewares/Third_Party/LwIP/src/api/netdb.o \
./Middlewares/Third_Party/LwIP/src/api/netifapi.o \
./Middlewares/Third_Party/LwIP/src/api/sockets.o \
./Middlewares/Third_Party/LwIP/src/api/tcpip.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/src/api/api_lib.d \
./Middlewares/Third_Party/LwIP/src/api/api_msg.d \
./Middlewares/Third_Party/LwIP/src/api/err.d \
./Middlewares/Third_Party/LwIP/src/api/if_api.d \
./Middlewares/Third_Party/LwIP/src/api/netbuf.d \
./Middlewares/Third_Party/LwIP/src/api/netdb.d \
./Middlewares/Third_Party/LwIP/src/api/netifapi.d \
./Middlewares/Third_Party/LwIP/src/api/sockets.d \
./Middlewares/Third_Party/LwIP/src/api/tcpip.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/src/api/%.o Middlewares/Third_Party/LwIP/src/api/%.su Middlewares/Third_Party/LwIP/src/api/%.cyclo: ../Middlewares/Third_Party/LwIP/src/api/%.c Middlewares/Third_Party/LwIP/src/api/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Src/Mqtt" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Inc/XGZP6897D" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Inc/Aht20" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Inc/cJSON" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Inc/Ds18b20" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Inc/FLASH_SECTOR_F4" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Inc/MODBUS-LIB" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Inc/Sht30" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Inc/W25qxx" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Src/Peripheral" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Src/Remote_control" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Src/Sensor" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Src/Storage" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Src/Modes" -I../Middlewares/Third_Party/LwIP/src/apps/http -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Src/Httpserver" -I"C:/Microcontroller/VAKIO/Stm32f407/Middlewares/Third_Party/LwIP/src/apps/mdns" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Src/Ota" -I"C:/Microcontroller/VAKIO/Stm32f407/Core/Src/Bootloader" -O0 -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-LwIP-2f-src-2f-api

clean-Middlewares-2f-Third_Party-2f-LwIP-2f-src-2f-api:
	-$(RM) ./Middlewares/Third_Party/LwIP/src/api/api_lib.cyclo ./Middlewares/Third_Party/LwIP/src/api/api_lib.d ./Middlewares/Third_Party/LwIP/src/api/api_lib.o ./Middlewares/Third_Party/LwIP/src/api/api_lib.su ./Middlewares/Third_Party/LwIP/src/api/api_msg.cyclo ./Middlewares/Third_Party/LwIP/src/api/api_msg.d ./Middlewares/Third_Party/LwIP/src/api/api_msg.o ./Middlewares/Third_Party/LwIP/src/api/api_msg.su ./Middlewares/Third_Party/LwIP/src/api/err.cyclo ./Middlewares/Third_Party/LwIP/src/api/err.d ./Middlewares/Third_Party/LwIP/src/api/err.o ./Middlewares/Third_Party/LwIP/src/api/err.su ./Middlewares/Third_Party/LwIP/src/api/if_api.cyclo ./Middlewares/Third_Party/LwIP/src/api/if_api.d ./Middlewares/Third_Party/LwIP/src/api/if_api.o ./Middlewares/Third_Party/LwIP/src/api/if_api.su ./Middlewares/Third_Party/LwIP/src/api/netbuf.cyclo ./Middlewares/Third_Party/LwIP/src/api/netbuf.d ./Middlewares/Third_Party/LwIP/src/api/netbuf.o ./Middlewares/Third_Party/LwIP/src/api/netbuf.su ./Middlewares/Third_Party/LwIP/src/api/netdb.cyclo ./Middlewares/Third_Party/LwIP/src/api/netdb.d ./Middlewares/Third_Party/LwIP/src/api/netdb.o ./Middlewares/Third_Party/LwIP/src/api/netdb.su ./Middlewares/Third_Party/LwIP/src/api/netifapi.cyclo ./Middlewares/Third_Party/LwIP/src/api/netifapi.d ./Middlewares/Third_Party/LwIP/src/api/netifapi.o ./Middlewares/Third_Party/LwIP/src/api/netifapi.su ./Middlewares/Third_Party/LwIP/src/api/sockets.cyclo ./Middlewares/Third_Party/LwIP/src/api/sockets.d ./Middlewares/Third_Party/LwIP/src/api/sockets.o ./Middlewares/Third_Party/LwIP/src/api/sockets.su ./Middlewares/Third_Party/LwIP/src/api/tcpip.cyclo ./Middlewares/Third_Party/LwIP/src/api/tcpip.d ./Middlewares/Third_Party/LwIP/src/api/tcpip.o ./Middlewares/Third_Party/LwIP/src/api/tcpip.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-LwIP-2f-src-2f-api


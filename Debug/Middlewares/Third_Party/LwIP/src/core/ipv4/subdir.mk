################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/src/core/ipv4/autoip.c \
../Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.c \
../Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.c \
../Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.c \
../Middlewares/Third_Party/LwIP/src/core/ipv4/igmp.c \
../Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.c \
../Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.c \
../Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.c 

OBJS += \
./Middlewares/Third_Party/LwIP/src/core/ipv4/autoip.o \
./Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.o \
./Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.o \
./Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.o \
./Middlewares/Third_Party/LwIP/src/core/ipv4/igmp.o \
./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.o \
./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.o \
./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/src/core/ipv4/autoip.d \
./Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.d \
./Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.d \
./Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.d \
./Middlewares/Third_Party/LwIP/src/core/ipv4/igmp.d \
./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.d \
./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.d \
./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/src/core/ipv4/%.o Middlewares/Third_Party/LwIP/src/core/ipv4/%.su Middlewares/Third_Party/LwIP/src/core/ipv4/%.cyclo: ../Middlewares/Third_Party/LwIP/src/core/ipv4/%.c Middlewares/Third_Party/LwIP/src/core/ipv4/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Project/Stm32f407/Core/Src/Mqtt" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I"C:/Project/Stm32f407/Core/Inc/XGZP6897D" -I"C:/Project/Stm32f407/Core/Inc/Aht20" -I"C:/Project/Stm32f407/Core/Inc/cJSON" -I"C:/Project/Stm32f407/Core/Inc/Ds18b20" -I"C:/Project/Stm32f407/Core/Inc/Epm_24c32" -I"C:/Project/Stm32f407/Core/Inc/FLASH_SECTOR_F4" -I"C:/Project/Stm32f407/Core/Inc/MODBUS-LIB" -I"C:/Project/Stm32f407/Core/Inc/Sht30" -I"C:/Project/Stm32f407/Core/Inc/W25qxx" -I"C:/Project/Stm32f407/Core/Src/Peripheral" -I"C:/Project/Stm32f407/Core/Src/Remote_control" -I"C:/Project/Stm32f407/Core/Src/Sensor" -I"C:/Project/Stm32f407/Core/Src/Storage" -I"C:/Project/Stm32f407/Core/Src/Modes" -I../Middlewares/Third_Party/LwIP/src/apps/http -I"C:/Project/Stm32f407/Core/Src/Httpserver" -I"C:/Project/Stm32f407/Middlewares/Third_Party/LwIP/src/apps/mdns" -O0 -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-LwIP-2f-src-2f-core-2f-ipv4

clean-Middlewares-2f-Third_Party-2f-LwIP-2f-src-2f-core-2f-ipv4:
	-$(RM) ./Middlewares/Third_Party/LwIP/src/core/ipv4/autoip.cyclo ./Middlewares/Third_Party/LwIP/src/core/ipv4/autoip.d ./Middlewares/Third_Party/LwIP/src/core/ipv4/autoip.o ./Middlewares/Third_Party/LwIP/src/core/ipv4/autoip.su ./Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.cyclo ./Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.d ./Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.o ./Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.su ./Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.cyclo ./Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.d ./Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.o ./Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.su ./Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.cyclo ./Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.d ./Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.o ./Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.su ./Middlewares/Third_Party/LwIP/src/core/ipv4/igmp.cyclo ./Middlewares/Third_Party/LwIP/src/core/ipv4/igmp.d ./Middlewares/Third_Party/LwIP/src/core/ipv4/igmp.o ./Middlewares/Third_Party/LwIP/src/core/ipv4/igmp.su ./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.cyclo ./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.d ./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.o ./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.su ./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.cyclo ./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.d ./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.o ./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.su ./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.cyclo ./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.d ./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.o ./Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-LwIP-2f-src-2f-core-2f-ipv4


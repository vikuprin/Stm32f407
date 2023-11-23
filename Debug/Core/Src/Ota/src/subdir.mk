################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Ota/src/clock.c \
../Core/Src/Ota/src/components.c \
../Core/Src/Ota/src/cpu.c \
../Core/Src/Ota/src/device.c \
../Core/Src/Ota/src/idle.c \
../Core/Src/Ota/src/ipc.c \
../Core/Src/Ota/src/irq.c \
../Core/Src/Ota/src/kservice.c \
../Core/Src/Ota/src/mem.c \
../Core/Src/Ota/src/memheap.c \
../Core/Src/Ota/src/mempool.c \
../Core/Src/Ota/src/object.c \
../Core/Src/Ota/src/scheduler.c \
../Core/Src/Ota/src/signal.c \
../Core/Src/Ota/src/slab.c \
../Core/Src/Ota/src/thread.c \
../Core/Src/Ota/src/timer.c 

OBJS += \
./Core/Src/Ota/src/clock.o \
./Core/Src/Ota/src/components.o \
./Core/Src/Ota/src/cpu.o \
./Core/Src/Ota/src/device.o \
./Core/Src/Ota/src/idle.o \
./Core/Src/Ota/src/ipc.o \
./Core/Src/Ota/src/irq.o \
./Core/Src/Ota/src/kservice.o \
./Core/Src/Ota/src/mem.o \
./Core/Src/Ota/src/memheap.o \
./Core/Src/Ota/src/mempool.o \
./Core/Src/Ota/src/object.o \
./Core/Src/Ota/src/scheduler.o \
./Core/Src/Ota/src/signal.o \
./Core/Src/Ota/src/slab.o \
./Core/Src/Ota/src/thread.o \
./Core/Src/Ota/src/timer.o 

C_DEPS += \
./Core/Src/Ota/src/clock.d \
./Core/Src/Ota/src/components.d \
./Core/Src/Ota/src/cpu.d \
./Core/Src/Ota/src/device.d \
./Core/Src/Ota/src/idle.d \
./Core/Src/Ota/src/ipc.d \
./Core/Src/Ota/src/irq.d \
./Core/Src/Ota/src/kservice.d \
./Core/Src/Ota/src/mem.d \
./Core/Src/Ota/src/memheap.d \
./Core/Src/Ota/src/mempool.d \
./Core/Src/Ota/src/object.d \
./Core/Src/Ota/src/scheduler.d \
./Core/Src/Ota/src/signal.d \
./Core/Src/Ota/src/slab.d \
./Core/Src/Ota/src/thread.d \
./Core/Src/Ota/src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Ota/src/%.o Core/Src/Ota/src/%.su Core/Src/Ota/src/%.cyclo: ../Core/Src/Ota/src/%.c Core/Src/Ota/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/Project/Stm32f407/Core/Src/Mqtt" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I"D:/Project/Stm32f407/Core/Inc/XGZP6897D" -I"D:/Project/Stm32f407/Core/Inc/Aht20" -I"D:/Project/Stm32f407/Core/Inc/cJSON" -I"D:/Project/Stm32f407/Core/Inc/Ds18b20" -I"D:/Project/Stm32f407/Core/Inc/Epm_24c32" -I"D:/Project/Stm32f407/Core/Inc/FLASH_SECTOR_F4" -I"D:/Project/Stm32f407/Core/Inc/MODBUS-LIB" -I"D:/Project/Stm32f407/Core/Inc/Sht30" -I"D:/Project/Stm32f407/Core/Inc/W25qxx" -I"D:/Project/Stm32f407/Core/Src/Peripheral" -I"D:/Project/Stm32f407/Core/Src/Remote_control" -I"D:/Project/Stm32f407/Core/Src/Sensor" -I"D:/Project/Stm32f407/Core/Src/Storage" -I"D:/Project/Stm32f407/Core/Src/Modes" -I../Middlewares/Third_Party/LwIP/src/apps/http -I"D:/Project/Stm32f407/Core/Src/Httpserver" -I"D:/Project/Stm32f407/Middlewares/Third_Party/LwIP/src/apps/mdns" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"D:/Project/Stm32f407/Core/Src/Ota/src" -I"D:/Project/Stm32f407/Core/Src/Ota/include" -O0 -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Ota-2f-src

clean-Core-2f-Src-2f-Ota-2f-src:
	-$(RM) ./Core/Src/Ota/src/clock.cyclo ./Core/Src/Ota/src/clock.d ./Core/Src/Ota/src/clock.o ./Core/Src/Ota/src/clock.su ./Core/Src/Ota/src/components.cyclo ./Core/Src/Ota/src/components.d ./Core/Src/Ota/src/components.o ./Core/Src/Ota/src/components.su ./Core/Src/Ota/src/cpu.cyclo ./Core/Src/Ota/src/cpu.d ./Core/Src/Ota/src/cpu.o ./Core/Src/Ota/src/cpu.su ./Core/Src/Ota/src/device.cyclo ./Core/Src/Ota/src/device.d ./Core/Src/Ota/src/device.o ./Core/Src/Ota/src/device.su ./Core/Src/Ota/src/idle.cyclo ./Core/Src/Ota/src/idle.d ./Core/Src/Ota/src/idle.o ./Core/Src/Ota/src/idle.su ./Core/Src/Ota/src/ipc.cyclo ./Core/Src/Ota/src/ipc.d ./Core/Src/Ota/src/ipc.o ./Core/Src/Ota/src/ipc.su ./Core/Src/Ota/src/irq.cyclo ./Core/Src/Ota/src/irq.d ./Core/Src/Ota/src/irq.o ./Core/Src/Ota/src/irq.su ./Core/Src/Ota/src/kservice.cyclo ./Core/Src/Ota/src/kservice.d ./Core/Src/Ota/src/kservice.o ./Core/Src/Ota/src/kservice.su ./Core/Src/Ota/src/mem.cyclo ./Core/Src/Ota/src/mem.d ./Core/Src/Ota/src/mem.o ./Core/Src/Ota/src/mem.su ./Core/Src/Ota/src/memheap.cyclo ./Core/Src/Ota/src/memheap.d ./Core/Src/Ota/src/memheap.o ./Core/Src/Ota/src/memheap.su ./Core/Src/Ota/src/mempool.cyclo ./Core/Src/Ota/src/mempool.d ./Core/Src/Ota/src/mempool.o ./Core/Src/Ota/src/mempool.su ./Core/Src/Ota/src/object.cyclo ./Core/Src/Ota/src/object.d ./Core/Src/Ota/src/object.o ./Core/Src/Ota/src/object.su ./Core/Src/Ota/src/scheduler.cyclo ./Core/Src/Ota/src/scheduler.d ./Core/Src/Ota/src/scheduler.o ./Core/Src/Ota/src/scheduler.su ./Core/Src/Ota/src/signal.cyclo ./Core/Src/Ota/src/signal.d ./Core/Src/Ota/src/signal.o ./Core/Src/Ota/src/signal.su ./Core/Src/Ota/src/slab.cyclo ./Core/Src/Ota/src/slab.d ./Core/Src/Ota/src/slab.o ./Core/Src/Ota/src/slab.su ./Core/Src/Ota/src/thread.cyclo ./Core/Src/Ota/src/thread.d ./Core/Src/Ota/src/thread.o ./Core/Src/Ota/src/thread.su ./Core/Src/Ota/src/timer.cyclo ./Core/Src/Ota/src/timer.d ./Core/Src/Ota/src/timer.o ./Core/Src/Ota/src/timer.su

.PHONY: clean-Core-2f-Src-2f-Ota-2f-src


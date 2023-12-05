################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f407vetx.s 

OBJS += \
./Core/Startup/startup_stm32f407vetx.o 

S_DEPS += \
./Core/Startup/startup_stm32f407vetx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"D:/Project/Stm32f407/Core/Src/Mqtt" -I"D:/Project/Stm32f407/Core/Inc/XGZP6897D" -I"D:/Project/Stm32f407/Core/Inc/Aht20" -I"D:/Project/Stm32f407/Core/Inc/cJSON" -I"D:/Project/Stm32f407/Core/Inc/Ds18b20" -I"D:/Project/Stm32f407/Core/Inc/FLASH_SECTOR_F4" -I"D:/Project/Stm32f407/Core/Inc/MODBUS-LIB" -I"D:/Project/Stm32f407/Core/Inc/Sht30" -I"D:/Project/Stm32f407/Core/Inc/W25qxx" -I"D:/Project/Stm32f407/Core/Src/Peripheral" -I"D:/Project/Stm32f407/Core/Src/Remote_control" -I"D:/Project/Stm32f407/Core/Src/Sensor" -I"D:/Project/Stm32f407/Core/Src/Storage" -I"D:/Project/Stm32f407/Core/Src/Modes" -I"D:/Project/Stm32f407/Core/Src/Httpserver" -I"D:/Project/Stm32f407/Middlewares/Third_Party/LwIP/src/apps/mdns" -I"D:/Project/Stm32f407/Core/Src/Ota" -I"D:/Project/Stm32f407/Core/Src/Bootloader" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32f407vetx.d ./Core/Startup/startup_stm32f407vetx.o

.PHONY: clean-Core-2f-Startup


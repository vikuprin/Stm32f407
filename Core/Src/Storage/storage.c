#include "storage.h"
#include "epm_24c32.h"
#include "FLASH_SECTOR_F4.h"
#include "first_init.h"
#include <string.h>
#include <stdlib.h>

#define LOCATION_DEVICE 	0x080C0000 //0x081C0000
#define LOCATION_WIRELESS  	0x080E0000 //0x081E0000

void write_wireless_params()
{
	Flash_Write_Data(LOCATION_WIRELESS, wireless_params, sizeof(wireless_parameters_s));
}

void read_wireless_params()
{
	Flash_Read_Data(LOCATION_WIRELESS, wireless_params, sizeof(wireless_parameters_s));
}

void write_device_params()
{
	Flash_Write_Data(LOCATION_DEVICE, device, sizeof(device_s));
}

void read_device_params()
{
	Flash_Read_Data(LOCATION_DEVICE, device, sizeof(device_s));
}

void first_start_init()
{
    first_init_device();
    first_init_wireless();
}

void second_start_init()
{
    read_device_params();
    read_wireless_params();
}

void malloc_memory_parameters()
{
    device = malloc(sizeof(device_s));
    wireless_params = malloc(sizeof(wireless_parameters_s));
    sensors_data = malloc(sizeof(sensors_data_s));
}

void reset_nvs_parameteres()
{
	device->check_1_0_0 = CHECK_VALUE_1_0_0 + 1;
	write_device_params();
}

void set_default_data()
{
	device->state = OFF;
	device->inflow_speed = 1;
	device->ten_state = ON;
	device->temp_limit = TEN_TEMP_LIMIT;
    device->remote_control.error = false;
    sensors_data->out_state = false;
    sensors_data->in_state = false;
}

void init_storage()
{
	malloc_memory_parameters();
	read_device_params();
	// Проверка на первый запуск устройства
	if (device->check_1_0_0 != CHECK_VALUE_1_0_0)
		first_start_init();
	else
		second_start_init();
	set_default_data();
}

uint32_t CRC_CalcBlockCRC(uint32_t pBuffer[], uint32_t BufferLength)
{
	uint32_t index = 0;
	for(index = 0; index < BufferLength; index++)
	{
		CRC->DR = pBuffer[index];
    }
    return (CRC->DR);
}

//////////////////////////////////////
extern int address_flash;
#define APP_START 0x08060000
typedef void(*pFunction)(void);
static pFunction Jump_To_Application;

static void jumpToApp(uint32_t start_program_addr)
{
	HAL_DeInit();
	SCB->VTOR = start_program_addr; // set intrerrupt vector table
    uint32_t JumpAddress = *(__IO uint32_t*) (start_program_addr + 4);
    Jump_To_Application = (pFunction) JumpAddress;
    __set_MSP(*(__IO uint32_t*)start_program_addr);
    Jump_To_Application();
}

void bootloader()
{
    /// проверяем наличие новой прошивки во втором банке памяти МК
    uint32_t *data = (uint32_t*)address_flash; // сектор 6
    uint32_t hasSW = data[0];    // 0 - есть новая прошивка
    uint32_t   len =  data[1]>>2; // размер прошивки в 32-битных словах
    if (hasSW != 0xFFFFFFFF)
    {
        Flash_Delete_Data(0x08080000); // сектор 8
        Flash_Delete_Data(0x080A0000);  // сектор 9
       	Flash_Write_Data(0x08080000, data, len);
//        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address_flash + 4, 0);
//        HAL_FLASH_Lock();
       	HAL_Delay(2000);
        jumpToApp(0x08080000);
    }
}

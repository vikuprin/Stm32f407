#include "storage.h"
#include "epm_24c32.h"
#include "FLASH_SECTOR_F4.h"
#include "first_init.h"
#include <string.h>
#include <stdlib.h>

#define LOCATION_DEVICE 	0x080A0000 //0x081A0000
#define LOCATION_WIRELESS 	0x080C0000 //0x081C0000
#define LOCATION_EXTRA  	0x080E0000 //0x081E0000

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

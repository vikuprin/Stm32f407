#include "storage.h"
#include "epm_24c32.h"
#include "FLASH_SECTOR_F4.h"
#include "first_init.h"
#include <string.h>

#define LOCATION_DEVICE 	0x081A0000
#define LOCATION_WIRELESS 	0x081C0000
#define LOCATION_HEATERS  	0x081E0000

void write_wireless_params()
{
	Flash_Write_Data(LOCATION_WIRELESS, wireless_params, sizeof(wireless_parameters_s));
}

void read_wireless_params()
{
	Flash_Read_Data(LOCATION_WIRELESS, wireless_params, sizeof(wireless_parameters_s));
}

void write_heaters()
{
	Flash_Write_Data(LOCATION_HEATERS, heaters, sizeof(heaters_s));
}

void read_heaters()
{
	Flash_Read_Data(LOCATION_HEATERS, heaters, sizeof(heaters_s));
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
    first_init_heaters();
}

void second_start_init()
{
    read_device_params();
    read_wireless_params();
    read_heaters();
}

void malloc_memory_parameters()
{
    device = malloc(sizeof(device_s));
    wireless_params = malloc(sizeof(wireless_parameters_s));
    sensors_data = malloc(sizeof(sensors_data_s));
    heaters = malloc(sizeof(heaters_s));
}

void reset_nvs_parameteres()
{
	device->check_1_0_0 = CHECK_VALUE_1_0_0 + 1;
	write_device_params();
}

void set_default_data()
{
    device->remote_control.error = false;
    sensors_data->out_state = false;
    sensors_data->in_state = false;
    heaters->ten.state = OFF;
    heaters->update_koef_time = 5000000;
    heaters->koef = 0;
    heaters->delta = 1;
    heaters->delta_high = 15;
    heaters->koef_grow = 50;
}

void init_storage()
{
	malloc_memory_parameters();
	read_device_params();
	// Проверка на первый запуск устройства
	if (device->check_1_0_0 != CHECK_VALUE_1_0_0)
	{
		first_start_init();
	}
	else
	{
		second_start_init();
	}
	set_default_data();
}

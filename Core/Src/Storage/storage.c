#include "storage.h"
#include "FLASH_SECTOR_F4.h"
#include <string.h>
#include <stdlib.h>

void write_wireless_params()
{
	Flash_Write_Data(WIRELESS_ADDR_FLASH, wireless_params, sizeof(wireless_parameters_s));
}

void read_wireless_params()
{
	Flash_Read_Data(WIRELESS_ADDR_FLASH, wireless_params, sizeof(wireless_parameters_s));
}

void write_device_params()
{
	Flash_Write_Data(DEVICE_ADDR_FLASH, device, sizeof(device_s));
}

void read_device_params()
{
	Flash_Read_Data(DEVICE_ADDR_FLASH, device, sizeof(device_s));
}

void first_init_wireless()
{
	// Настройки брокера Вакио
	wireless_params->vakio_mqtt.host[0] = 0;
	wireless_params->vakio_mqtt.login[0] = 0;
	wireless_params->vakio_mqtt.password[0] = 0;
	wireless_params->vakio_mqtt.port = DEFAULT_MQTT_PORT;
	wireless_params->vakio_mqtt.client_id[0] = 0;

	// Настройки брокера пользователя
	wireless_params->user_mqtt.host[0] = 0;
	wireless_params->user_mqtt.login[0] = 0;
	wireless_params->user_mqtt.password[0] = 0;
	wireless_params->user_mqtt.port = DEFAULT_MQTT_PORT;
	wireless_params->user_mqtt.client_id[0] = 0;
	strcpy(wireless_params->user_mqtt.topic, DEFAULT_USER_TOPIC);

	// Идентификаторы прибора
	wireless_params->vakio.device_id[0] = 0;
	wireless_params->vakio.user_id[0] = 0;

	// Тип подключения к брокеру
	wireless_params->mqtt_type = VAKIO_MQTT;

	// Айпи сервера
	wireless_params->server_ip[0] = 0;
	wireless_params->vakio_mqtt_connect_ip = false;
	write_wireless_params();
}

void first_init_device()
{
	device->ota_len = 0;
	device->check_1_0_0 = CHECK_VALUE_1_0_0;
	device->state = OFF;
	device->error_temp_hot = false;
	device->error_temp_cold = false;
	device->error_stop_hot = false;
	device->error_stop_cold = false;
	device->error_ds18b20 = false;
	device->error_fan = false;
	device->mode = INFLOW_MODE;
	device->last_mode = INFLOW_MODE;
	//smart
    device->smart.set = OFF;
    device->smart.master_id = 0;
    device->smart.max_speed = 10;
    device->smart.master_topic_data[0] = 0;
    device->smart.master_topic_system[0] = 0;
    device->smart.parameter[0] = 0;
    //smart
	device->inflow_speed = 1;
	uint8_t speed_arr[8] = SPEED_ARR_DEFAUL;
	memcpy(device->speed_arr, speed_arr, 8);
	device->remote_control.find = OFF;
	device->extra_options.deviation = 3;           // отклонение должно быть 3
	device->extra_options.check_time = 1;          // время проверки должно быть 1-5 минут
	device->extra_options.step_pwm = 5;            // шаг шим должно быть 5
	device->extra_options.Kp = 250;   //750
	device->extra_options.Ki = 10;    //30
	device->extra_options.Kd = 10;   //130

	write_device_params();
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

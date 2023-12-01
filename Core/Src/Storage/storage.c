#include "storage.h"
#include "w25qxx.h"
#include <string.h>
#include <stdlib.h>

void write_check_byte()
{
	W25qxx_WriteByte(device_check_1_0_0, CHECK_EXT_BYTE);
}
void read_check_byte()
{
	W25qxx_ReadByte(&device_check_1_0_0, CHECK_EXT_BYTE);
}

void write_ota_byte()
{
	W25qxx_EraseSector(VAR_EXT_SECTOR);
	uint8_t temp_ota_len[4];
	temp_ota_len[0] = device_ota_len;
	temp_ota_len[1] = device_ota_len / (0xFF + 1);
	temp_ota_len[2] = device_ota_len / (0xFFFF + 1);
	temp_ota_len[3] = device_ota_len / (0xFFFFFF + 1);

	W25qxx_WriteByte(temp_ota_len[0], OTA_LEN_EXT_BYTE);
	W25qxx_WriteByte(temp_ota_len[1], OTA_LEN_EXT_BYTE + 1);
	W25qxx_WriteByte(temp_ota_len[2], OTA_LEN_EXT_BYTE + 2);
	W25qxx_WriteByte(temp_ota_len[3], OTA_LEN_EXT_BYTE + 3);
}
void read_ota_byte()
{
	uint8_t temp_ota_len[4];
	W25qxx_ReadBytes(temp_ota_len, OTA_LEN_EXT_BYTE, 4);

	device_ota_len = (temp_ota_len[3] * (0xFFFFFF + 1)) + (temp_ota_len[2] * (0xFFFF + 1)) + (temp_ota_len[1] * (0xFF + 1)) + temp_ota_len[0];
}

void write_wireless_params()
{
	W25qxx_EraseSector(WIRELESS_EXT_SECTOR);
	W25qxx_WriteSector(device, WIRELESS_EXT_SECTOR, 0, sizeof(device_s));
}
void read_wireless_params()
{
	 W25qxx_ReadSector(device, WIRELESS_EXT_SECTOR, 0, sizeof(device_s));
}

void write_device_params()
{
	W25qxx_EraseSector(DEVICE_EXT_SECTOR);
	W25qxx_WriteSector(device, DEVICE_EXT_SECTOR, 0, sizeof(device_s));
}
void read_device_params()
{
    W25qxx_ReadSector(device, DEVICE_EXT_SECTOR, 0, sizeof(device_s));
}

void first_init_check()
{
	device_check_1_0_0 = CHECK_VALUE_1_0_0;
	write_check_byte();
}

void first_init_ota()
{
	device_ota_len = 0;
	write_ota_byte();
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
	first_init_ota();
	first_init_check();
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
	device_check_1_0_0 = CHECK_VALUE_1_0_0 + 1;
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
	W25qxx_Init();
//	W25qxx_EraseChip();
	read_check_byte();
	// Проверка на первый запуск устройства
	if (device_check_1_0_0 != CHECK_VALUE_1_0_0)
		first_start_init();
	else
		second_start_init();
	set_default_data();
}

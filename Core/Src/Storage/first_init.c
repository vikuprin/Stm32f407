#include "first_init.h"
#include "storage.h"
#include "main.h"
#include <string.h>

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
	device->check_1_0_0 = CHECK_VALUE_1_0_0;
	device->state = OFF;
	device->firmware_flag = false;
	device->error_temp_hot = false;
	device->error_temp_cold = false;
	device->error_stop_hot = false;
	device->error_stop_cold = false;
	device->error_ds18b20 = false;
	device->error_fan = false;
	device->mode = INFLOW_MODE;
	device->last_mode = INFLOW_MODE;
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

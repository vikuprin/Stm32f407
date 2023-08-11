#include "first_init.h"
#include "storage.h"
#include "main.h"
#include <string.h>

void first_init_heaters()
{
    heaters->ten.off_temp = TEN_OFF_TEMP;
    heaters->ten.off_temp_under = TEN_OFF_UNDER;
    heaters->ten.temp_limit = TEN_TEMP_LIMIT;
    heaters->ten.started_value = TEN_STARTED_VALUE;
    heaters->ten.power_koef = TEN_POWER_KOEF;

    heaters->on_off_ten = ON;
    heaters->update_koef_time = 5000000;
    heaters->delta = 1;
    heaters->delta_high = 10;
    heaters->koef_grow = 50;
    write_heaters();
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
	wireless_params->vakio_mqtt_connect_ip;
	wireless_params->soft_ap_off_time = MIN_15_OFF_AP;
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
	device->mode = INFLOW_MODE;
	device->last_mode = INFLOW_MODE;
	device->inflow_speed = 1;
	uint8_t speed_arr[8] = SPEED_ARR_DEFAUL;
	memcpy(device->speed_arr, speed_arr, 8);
	device->sensors_addr.out[0] = 0;
	device->sensors_addr.in[0] = 0;
	device->remote_control.find = OFF;
	device->extra_options.deviation = 3;           // отклонение должно быть 3
	device->extra_options.check_time = 1 * MINUTE; // время проверки должно быть 1-5 минут
	device->extra_options.step_pwm = 5;            // шаг шим должно быть 5
	write_device_params();
}

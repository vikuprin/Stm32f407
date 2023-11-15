#include "sensor.h"
#include "ds18b20.h"
#include <string.h>
#include "usart.h"
#include "onewire.h"
#include "cmsis_os.h"
#include "mqtt_message.h"

DS18B20 temperatureSensor1;
DS18B20 temperatureSensor2;

uint8_t temp[2];
uint32_t count;
uint8_t point[6] = {25, 25, 25, 25, 25, 25};
uint8_t ave = 25;


void middle_temp()
{
    float sum = 0.0; // переменная для суммы чисел в массиве
    for (int i = 0; i < 6; i++)
    {
        sum += point[i]; // накапливаем сумму
    }
    ave = (float)sum / 6; // вычисляем среднее арифметическое
    DEBUG_DS("AVE =%i\n", ave);
}

void mass_temp()
{
    if (count < 6)
    {
        point[count] = temp[1]; //sensors_data->out;
    }
    else
    {
        for (uint8_t i = 0; i < 5; i++)
        {
            point[i] = point[i + 1];
        }
        point[5] = temp[1]; //sensors_data->out;
    }
    count++;
//    DEBUG_DS("POINT[0]=%i\n", point[0]);
//    DEBUG_DS("POINT[1]=%i\n", point[1]);
//    DEBUG_DS("POINT[2]=%i\n", point[2]);
//    DEBUG_DS("POINT[3]=%i\n", point[3]);
//    DEBUG_DS("POINT[4]=%i\n", point[4]);
//    DEBUG_DS("POINT[5]=%i\n", point[5]);
    middle_temp();
}

void get_ds_data()
{
	ds_count = 0;

	//DS_Pin1
    DS18B20_InitializationCommand(&temperatureSensor1);
    DS18B20_SkipRom(&temperatureSensor1);
    DS18B20_ConvertT(&temperatureSensor1, DS18B20_DATA);
    DS18B20_InitializationCommand(&temperatureSensor1);
    DS18B20_SkipRom(&temperatureSensor1);
    DS18B20_ReadScratchpad(&temperatureSensor1);
	temp[ds_count] = temperatureSensor1.temperature;
	sensors_data->in = temp[ds_count];
	DEBUG_DS("Temperature1 = %i\n", temp[ds_count]);
	ds_count++;

	//DS_Pin2
    DS18B20_InitializationCommand(&temperatureSensor2);
    DS18B20_SkipRom(&temperatureSensor2);
    DS18B20_ConvertT(&temperatureSensor2, DS18B20_DATA);
    DS18B20_InitializationCommand(&temperatureSensor2);
    DS18B20_SkipRom(&temperatureSensor2);
    DS18B20_ReadScratchpad(&temperatureSensor2);
	temp[ds_count] = temperatureSensor2.temperature;
	sensors_data->out = temp[ds_count];
	DEBUG_DS("Temperature2 = %i\n", temp[ds_count]);
	ds_count++;

	DEBUG_DS("ds_count = %i\n", ds_count);
	if(ds_count == 2)
		device->error_ds18b20 = false;
	else
		device->error_ds18b20 = true;
}

void get_ds_data_mass()
{
	if(sensors_data->in_state && sensors_data->out_state)
	{
		get_ds_data();
		mass_temp();
	}
}

void init_ds_devices()
{
	uint8_t settings[3];
	settings[0] = temperatureSensor1.temperatureLimitHigh;
	settings[1] = temperatureSensor1.temperatureLimitLow;
	settings[2] = DS18B20_12_BITS_CONFIG;

	//DS_Pin1
	sensors_data->in_state = false;
	DS18B20_Init(&temperatureSensor1, &huart3);
	DS18B20_InitializationCommand(&temperatureSensor1);
	DS18B20_ReadRom(&temperatureSensor1);
	DS18B20_ReadScratchpad(&temperatureSensor1);
	DS18B20_InitializationCommand(&temperatureSensor1);
	DS18B20_SkipRom(&temperatureSensor1);
	if(DS18B20_WriteScratchpad(&temperatureSensor1, settings) == DS18B20_OK)
		sensors_data->in_state = true;

	//DS_Pin2
	sensors_data->out_state = false;
	DS18B20_Init(&temperatureSensor2, &huart6);
	DS18B20_InitializationCommand(&temperatureSensor2);
	DS18B20_ReadRom(&temperatureSensor2);
	DS18B20_ReadScratchpad(&temperatureSensor2);
	DS18B20_InitializationCommand(&temperatureSensor2);
	DS18B20_SkipRom(&temperatureSensor2);
	if(DS18B20_WriteScratchpad(&temperatureSensor2, settings) == DS18B20_OK)
		sensors_data->out_state = true;

	if(sensors_data->in_state && sensors_data->out_state)
		device->error_ds18b20 = false;
	else
		device->error_ds18b20 = true;
}

xSemaphoreHandle xBinarySamaphore;
void DSTask(void const * argument)
{
	vSemaphoreCreateBinary(xBinarySamaphore);
    init_ds_devices();
    for(;;)
    {
    	get_ds_data_mass();
    	publish_temp_log();
        osDelay(1000);
    }
}

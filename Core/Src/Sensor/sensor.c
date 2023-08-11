#include "sensor.h"
#include "ds18b20.h"
#include <string.h>
#include "usart.h"
#include "onewire.h"
#include "dwt.h"
#include "cmsis_os.h"

DS18B20_Drv_t DS;
OneWire_t OW;
DS18B20_Drv_t DS2;
OneWire_t OW2;

uint8_t temp[4];
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
        point[count] = temp[0]; //sensors_data->out;
    }
    else
    {
        for (uint8_t i = 0; i < 5; i++)
        {
            point[i] = point[i + 1];
        }
        point[5] = temp[0]; //sensors_data->out;
    }
    count++;
    DEBUG_DS("POINT[0]=%i\n", point[0]);
    DEBUG_DS("POINT[1]=%i\n", point[1]);
    DEBUG_DS("POINT[2]=%i\n", point[2]);
    DEBUG_DS("POINT[3]=%i\n", point[3]);
    DEBUG_DS("POINT[4]=%i\n", point[4]);
    DEBUG_DS("POINT[5]=%i\n", point[5]);
    middle_temp();
}

void get_ds_data()
{
	DS18B20_StartAll(&OW);
	/* Read temperature from device and store it to DS data structure */
	ds_count = 0;
	for(uint8_t i = 0; i < OW.RomCnt; i++)
	{
		temp[ds_count] = DS18B20_Read(&OW, DS.DevAddr[i], &DS.Temperature[i]);
		DEBUG_DS("Temperature1 = %i\n", temp[ds_count]);
		ds_count++;
	}
	/* Search Alarm triggered and store in DS data structure */
	DS18B20_AlarmSearch(&DS, &OW);

	DS18B20_StartAll(&OW2);
	/* Read temperature from device and store it to DS data structure */
	for(uint8_t i = 0; i < OW2.RomCnt; i++)
	{
		temp[ds_count] = DS18B20_Read(&OW2, DS2.DevAddr[i], &DS2.Temperature[i]);
		DEBUG_DS("Temperature2 = %i\n", temp[ds_count]);
		ds_count++;
	}
	DEBUG_DS("ds_count = %i\n", ds_count);
	/* Search Alarm triggered and store in DS data structure */
	DS18B20_AlarmSearch(&DS2, &OW2);
}

void get_ds_data_mass()
{
	get_ds_data();
	mass_temp();
}

void init_ds_devices()
{
	DwtInit();

	OW.DataPin = DS_Pin;
	OW.DataPort = DS_GPIO_Port;
	DS.Resolution = DS18B20_Resolution_12bits;
	DS18B20_Init(&DS, &OW);
	/* Set high temperature alarm on device number 0, 31 Deg C */
	DS18B20_SetTempAlarm(&OW, DS.DevAddr[0], 0, 31);

	OW2.DataPin = DS_Pin2;
	OW2.DataPort = DS_GPIO_Port2;
	DS2.Resolution = DS18B20_Resolution_12bits;
	DS18B20_Init(&DS2, &OW2);
	/* Set high temperature alarm on device number 0, 31 Deg C */
	DS18B20_SetTempAlarm(&OW2, DS2.DevAddr[0], 0, 60);
}


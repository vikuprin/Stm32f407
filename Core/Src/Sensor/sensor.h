#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_DS(...) printf("DS18B20: "__VA_ARGS__);
#else
#define DEBUG_DS(...)
#endif

#define DS_Pin         GPIO_PIN_8
#define DS_GPIO_Port   GPIOD
#define DS_Pin2        GPIO_PIN_6
#define DS_GPIO_Port2  GPIOC

uint8_t ds_count;
void get_ds_data_mass();
void init_ds_devices();

#endif // __SENSOR_H__

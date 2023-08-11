#ifndef __SHT_H__
#define __SHT_H__

#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_SHT(...) printf("SHT30: "__VA_ARGS__);
#else
#define DEBUG_SHT(...)
#endif

void get_sht_data();
void init_sht_devices();

#endif // __SENSOR_H__

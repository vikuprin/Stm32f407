#ifndef __SHT30_I2C_DRV_H__
#define __SHT30_I2C_DRV_H__

#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_AHT(...) printf("AHT: "__VA_ARGS__);
#else
#define DEBUG_AHT(...)
#endif

#define AHT10_Adress 0x38 << 1

void get_aht_data();

#endif

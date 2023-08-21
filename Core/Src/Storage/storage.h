#ifndef _STORAGE_H
#define _STORAGE_H

#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_STORAGE(...) printf("STORAGE: "__VA_ARGS__);
#else
#define DEBUG_STORAGE(...)
#endif

#define CHECK_VALUE_1_0_0  1

void write_device_params();
void read_device_params();
void write_wireless_params();
void read_wireless_params();
void write_heaters();
void read_heaters();

void init_storage();
void reset_nvs_parameteres();

#endif

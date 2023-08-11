#ifndef __UTILS_H__
#define __UTILS_H__

#include "main.h"
#include "cJSON.h"

#if DEBUG_PRINT == 1
#define DEBUG_UTILS(...) printf("UTILS: "__VA_ARGS__);
#else
#define DEBUG_UTILS(...)
#endif

bool check_js_param_char(cJSON *data_json, char *key, char *set_value);
bool check_js_param_u8(cJSON *data_json, char *key, uint8_t *set_value);
bool check_js_param_int(cJSON *data_json, char *key, int *set_value);
bool check_js_param_double(cJSON *data_json, char *key, double *set_value);
bool check_js_param_u16(cJSON *data_json, char *key, uint16_t *set_value);
bool check_char_mode(cJSON *data_json, char *key, uint8_t *set_value);
bool check_char_on_off(cJSON *data_json, char *key, uint8_t *set_value);
int map(int x, int in_min, int in_max, int out_min, int out_max);

#endif // __UTILS_H__

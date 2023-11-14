#ifndef __SMART_MODE_EXTERNAL_H__
#define __SMART_MODE_EXTERNAL_H__

#include "main.h"

void smart_mode_external();
void reset_smart_mode();
int get_delta_sum();
void init_smart_timer();
void off_smart_mode();
bool get_smart_mode_state();
void calc_smart_speed();

#endif // __SMART_MODE_EXTERNAL_H__

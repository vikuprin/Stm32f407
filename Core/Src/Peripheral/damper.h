#ifndef __DAMPER_H__
#define __DAMPER_H__

#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_DAMP(...) printf("DAMPER: "__VA_ARGS__);
#else
#define DEBUG_DAMP(...)
#endif

uint8_t state_damper; //0-закрыт, 1-открывается, 2-открыт

void close_damper();
void open_damper();
void DamperTask(void *argument);

#endif

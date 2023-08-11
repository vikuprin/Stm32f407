#ifndef __XGZP6897D_H
#define __XGZP6897D_H

#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_XGZ(...) printf("XGZ: "__VA_ARGS__);
#else
#define DEBUG_XGZ(...)
#endif

#define	SLAVE_ADDR     0x6D<<1
#define TIMEOUT        0xFFFF

void get_xgz_data();

#endif





#ifndef __RC_H__
#define __RC_H__
#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_RC(...) printf("REMOTE CONTROL: "__VA_ARGS__);
#else
#define DEBUG_RC(...)
#endif

// PULT HOLDING
#define REG_GET_STATE 48
#define REG_SET_GET_MODE 49
#define REG_SET_GET_WIND 50
#define REG_GET_TEMP 51
#define REG_SET_GET_TEN 52
#define REG_SET_GET_SWITCH 53
// MODE SETING
#define SET_INFLOW_MODE 1
#define SET_INFLOW_MAX_MODE 0
// MODE WIND
#define SET_INFLOW_SPEED_0 3
#define SET_INFLOW_SPEED_2 0
#define SET_INFLOW_SPEED_5 1
#define SET_INFLOW_SPEED_7 2
/*
REG_GET_STATE:
BIT0-BIT1 определение состояния скорости вент-ра: 0 - низкая скорость, 1 - средняя скорость, 2 - высокая скорость, 3 - выкл;
BIT2: состояние клапана: 0 - закрыто, 1 - открыто; (по умолчанию открыт);
BIT3: состояние клапана: 0 - вкл, 1 - выкл.;
BIT4-BIT5: режим: 0 - охлаждение, 1 - обогрев, 2 - вентиляция;
BIT6: режим ожидания;
BIT7:  вкл/выкл прибор: 0 - выкл, 1 - вкл.
*/

void check_remote_control(void);

#endif

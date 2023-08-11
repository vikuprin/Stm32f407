#ifndef __SHT30_I2C_DRV_H__
#define __SHT30_I2C_DRV_H__

#include "main.h"

/* ADDR Pin Conect to VSS */

#define	SHT30_ADDR_WRITE	0x44<<1         //10001000
#define	SHT30_ADDR_READ		(0x44<<1)+1	    //10001011
#define SHT30_TIMEOUT       0xFFFF

typedef enum
{
    SOFT_RESET_CMD = 0x30A2,	
    HIGH_ENABLED_CMD    = 0x2C06,
    MEDIUM_ENABLED_CMD  = 0x2C0D,
    LOW_ENABLED_CMD     = 0x2C10,
    HIGH_DISABLED_CMD   = 0x2400,
    MEDIUM_DISABLED_CMD = 0x240B,
    LOW_DISABLED_CMD    = 0x2416,

    HIGH_0_5_CMD   = 0x2032,
    MEDIUM_0_5_CMD = 0x2024,
    LOW_0_5_CMD    = 0x202F,
    HIGH_1_CMD     = 0x2130,
    MEDIUM_1_CMD   = 0x2126,
    LOW_1_CMD      = 0x212D,
    HIGH_2_CMD     = 0x2236,
    MEDIUM_2_CMD   = 0x2220,
    LOW_2_CMD      = 0x222B,
    HIGH_4_CMD     = 0x2334,
    MEDIUM_4_CMD   = 0x2322,
    LOW_4_CMD      = 0x2329,
    HIGH_10_CMD    = 0x2737,
    MEDIUM_10_CMD  = 0x2721,
    LOW_10_CMD     = 0x272A,
	READOUT_FOR_PERIODIC_MODE = 0xE000,
} SHT30_CMD;

uint8_t SHT30_Read_Dat(uint8_t* dat);
uint8_t SHT30_Init(void);
void SHT30_reset(void);
uint8_t SHT30_Dat_To_Float(uint8_t* const dat, float* temperature, float* humidity);

#endif

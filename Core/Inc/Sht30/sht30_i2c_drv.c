#include "sht30_i2c_drv.h"
#include "i2c.h"

static uint8_t SHT30_Send_Cmd(SHT30_CMD cmd)
{
    uint8_t cmd_buffer[2];
    cmd_buffer[0] = cmd >> 8;
    cmd_buffer[1] = cmd;
//    HAL_Delay(20);
    return HAL_I2C_Master_Transmit(&hi2c1, SHT30_ADDR_WRITE, (uint8_t* )cmd_buffer, 2, SHT30_TIMEOUT);
}

void SHT30_reset(void)
{
    SHT30_Send_Cmd(SOFT_RESET_CMD);
    HAL_Delay(20);
}

uint8_t SHT30_Init(void)
{
    return SHT30_Send_Cmd(MEDIUM_2_CMD);
}

uint8_t SHT30_Read_Dat(uint8_t* dat)
{
	SHT30_Send_Cmd(READOUT_FOR_PERIODIC_MODE);
	return HAL_I2C_Master_Receive(&hi2c1, SHT30_ADDR_READ, dat, 6, SHT30_TIMEOUT);
}

#define CRC8_POLYNOMIAL 0x31

uint8_t CheckCrc8(uint8_t* const message, uint8_t initial_value)
{
    uint8_t  remainder;
    uint8_t  i = 0, j = 0;
    remainder = initial_value;

    for(j = 0; j < 2;j++)
    {
        remainder ^= message[j];
        for (i = 0; i < 8; i++)
        {
            if (remainder & 0x80)
            {
                remainder = (remainder << 1)^CRC8_POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }
    }
    return remainder;
}

uint8_t SHT30_Dat_To_Float(uint8_t* const dat, float* temperature, float* humidity)
{
	uint16_t recv_temperature = 0;
	uint16_t recv_humidity = 0;
	
	if(CheckCrc8(dat, 0xFF) != dat[2] || CheckCrc8(&dat[3], 0xFF) != dat[5])
		return 1;
	
	recv_temperature = ((uint16_t)dat[0]<<8)|dat[1];
	*temperature = -45 + 175*((float)recv_temperature/65535);
	
	recv_humidity = ((uint16_t)dat[3]<<8)|dat[4];
	*humidity = 100 * ((float)recv_humidity / 65535);
	
	return 0;
}


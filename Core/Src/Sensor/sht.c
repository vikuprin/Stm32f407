#include "sht.h"
#include "sht30_i2c_drv.h"

uint8_t recv_dat[6] = {0};
float temperature = 0.0;
float humidity = 0.0;

void get_sht_data()
{
	if(SHT30_Read_Dat(recv_dat) == HAL_OK)
	{
		if(SHT30_Dat_To_Float(recv_dat, &temperature, &humidity)==0)
		{
			DEBUG_SHT("temperature = %f\n", temperature);
			DEBUG_SHT("humidity = %f\n", humidity);
		}
		else
		{
			DEBUG_SHT("crc check fail.\n");
		}
	}
	else
	{
		DEBUG_SHT("read data from sht30 fail.\n");
	}
}

void init_sht_devices()
{
	SHT30_reset();
	if(SHT30_Init() == HAL_OK)
	{
		DEBUG_SHT("sht30 init ok.\n");
	}
	else
	{
		DEBUG_SHT("sht30 init fail.\n");
	}
}


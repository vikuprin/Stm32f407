#include "aht20_i2c_drv.h"
#include "i2c.h"

uint8_t AHT10_RX_Data[6];
uint32_t AHT10_ADC_Raw;
float AHT10_Temperature, AHT10_Humidity;
uint8_t AHT10_TmpHum_Cmd = 0xAC;

void get_aht_data()
{
	HAL_I2C_Master_Transmit(&hi2c1, AHT10_Adress, &AHT10_TmpHum_Cmd, 1, 100);
//	HAL_Delay(100); // Delay must be > 75 ms
	HAL_I2C_Master_Receive(&hi2c1, AHT10_Adress, (uint8_t*)AHT10_RX_Data, 6, 100);
	/* Convert to Temperature in °C */
	AHT10_ADC_Raw = (((uint32_t)AHT10_RX_Data[3] & 15) << 16) | ((uint32_t)AHT10_RX_Data[4] << 8) | AHT10_RX_Data[5];
	AHT10_Temperature = (float)(AHT10_ADC_Raw * 200.00 / 1048576.00) - 50.00;
	AHT10_ADC_Raw = ((uint32_t)AHT10_RX_Data[1] << 12) | ((uint32_t)AHT10_RX_Data[2] << 4) | (AHT10_RX_Data[3] >> 4);
	AHT10_Humidity = (float)(AHT10_ADC_Raw*100.00/1048576.00);
	DEBUG_AHT("Humidity = %f\n", AHT10_Humidity);
}

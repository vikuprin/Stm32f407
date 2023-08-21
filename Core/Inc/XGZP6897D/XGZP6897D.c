#include "XGZP6897D.h"
#include "i2c.h"

#define PRESSURE_RANGE 8192

uint8_t CMD_reg;
int16_t pressure_adc;
uint16_t pressure;
uint8_t pressure_H, pressure_M, pressure_L;
//int16_t temperature_adc;
//uint8_t temperature_H, temperature_L, temperature;

uint8_t I2C_Read_Byte(uint8_t addr)
{
  uint8_t data = 0;
  uint8_t d;
  while (HAL_I2C_GetState(&hi2c3) != HAL_I2C_STATE_READY);
  d = HAL_I2C_Master_Transmit(&hi2c3, SLAVE_ADDR, &addr, 1, TIMEOUT);
  if ( d != HAL_OK) {
      return d;
  }

  while (HAL_I2C_GetState(&hi2c3) != HAL_I2C_STATE_READY);
  d = HAL_I2C_Master_Receive(&hi2c3, SLAVE_ADDR, &data, 1, TIMEOUT);
  if ( d != HAL_OK) {
      return d;
  }
  return data;
}

uint8_t I2C_Write_Byte(uint8_t addr, uint8_t data)
{
  uint8_t buf[] = {addr, data};
  uint8_t d;
  while (HAL_I2C_GetState(&hi2c3) != HAL_I2C_STATE_READY);
  d = HAL_I2C_Master_Transmit(&hi2c3, SLAVE_ADDR, buf, 2, TIMEOUT);
  if ( d != HAL_OK) {
      return d;
  }
  return HAL_OK;
}

void get_xgz_data()
{
	I2C_Write_Byte(0x30, 0x0A);
	do
	  {
		CMD_reg = I2C_Read_Byte(0x30);
	  } while ((CMD_reg & 0x08) > 0);
	pressure_H = I2C_Read_Byte(0x06);
	pressure_M = I2C_Read_Byte(0x07);
	pressure_L = I2C_Read_Byte(0x08);
	pressure_adc = pressure_H * 65536 + pressure_M * 256 + pressure_L;

	if (pressure_adc > 8388608)
		pressure = (pressure_adc - 16777216) / PRESSURE_RANGE;
	else
		pressure = pressure_adc / PRESSURE_RANGE;
	DEBUG_XGZ("pressure = %i\n", pressure);

//	temperature_H = I2C_Read_Byte(0x09);
//	temperature_L = I2C_Read_Byte(0x0A);
//	temperature_adc = temperature_H * 256 + temperature_L;
//	temperature = temperature_adc / 256;
//	DEBUG_XGZ("temperature = %i\n", temperature);
}

//pressure_adc = ((uint32_t)pressure_H << 8) + (uint32_t)pressure_M;
//pressure_adc = (pressure_adc << 8) + (uint32_t)pressure_L;
//temperature_adc = ((uint16_t)temperature_H << 8) + (uint16_t)temperature_L;
//// pressure is a signed2 complement style value, on 24bits.
//// need to extend the bit sign on the full 32bits.
//rawPressure = ((pressure_adc << 8) >> 8);
//rawTemperature = temperature_adc;

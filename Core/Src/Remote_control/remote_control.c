#include "remote_control.h"
#include "FreeRTOS.h"
#include "storage.h"
#include <stdio.h>
#include "ModbusConfig.h"
#include "Modbus.h"

#define REPEAT_RETRY 5

osThreadId_t modbusMasterTaskHandle;
const osThreadAttr_t modbusMasterTask_attributes = {
  .name = "modbusMasterTask",
  .stack_size = 1024 * 6,
  .priority = (osPriority_t) osPriorityNormal,
};

uint8_t reg_state;     // данные с регистра адреса статуса
int16_t new_reg_state; // сохраненные данные с регистра адреса статуса
uint8_t reg_ten;       // данные с регистра адреса установки температуры
int16_t new_reg_ten;   // сохраненные данные с регистра адреса установки температуры
bool limit_ten;        // переменная превышения значения тена
bool bin[8];           // бинарный массив регистра адреса статуса
uint8_t count_state;   // счетчик количества ошибочных пакетов регистра адреса статуса
uint8_t count_ten;     // счетчик количества ошибочных пакетов регистра адреса установки температуры

int8_t modbus_set_holding(uint8_t slave_id, uint16_t reg_start, uint16_t data)
{
	return modbus_master_set_one(slave_id, MB_FC_WRITE_MULTIPLE_REGISTERS, reg_start, data);
}

int32_t modbus_get_holding(uint8_t slave_id, uint16_t reg_start)
{
	return modbus_master_get_one(slave_id, MB_FC_READ_REGISTERS, reg_start);
}

void rc_parse_ten()
{
    if (new_reg_ten >= 0 && reg_ten != new_reg_ten)
    {
        count_ten = 0;
        reg_ten = new_reg_ten;
        if (reg_ten < 10)
        {
            limit_ten = true;
            reg_ten = 10;
        }
        if (reg_ten > 30)
        {
            limit_ten = true;
            reg_ten = 30;
        }
        heaters->ten.temp_limit = reg_ten;
    }
}

void new_state()
{
    if (!bin[7])
    {
        device->state = OFF;
    }
    else
    {
        device->state = ON;
        if (!bin[1] && !bin[0])
            device->inflow_speed = 3;
        if (!bin[1] && bin[0])
            device->inflow_speed = 5;
        if (bin[1] && !bin[0])
            device->inflow_speed = 7;
        if (bin[1] && bin[0])
            device->inflow_speed = 1;

        if (!bin[5] && bin[4])
            device->mode = INFLOW_MODE;
        if (!bin[5] && !bin[4])
            device->mode = INFLOW_MAX_MODE;
    }
}

void dig_to_bin()
{
    uint8_t dig;
    dig = reg_state;
    for (uint8_t i = 0; i < 8; i++)
    {
        bin[i] = dig % 2;
        dig = dig / 2;
    }
    // printf("%i", bin[7]);
    // printf("%i", bin[6]);
    // printf("%i", bin[5]);
    // printf("%i", bin[4]);
    // printf("%i", bin[3]);
    // printf("%i", bin[2]);
    // printf("%i", bin[1]);
    // printf("%i\n", bin[0]);
}

void rc_parse_state()
{
    if (new_reg_state >= 0 && reg_state != new_reg_state)
    {
        count_state = 0;
        DEBUG_RC("GET A NEW VALUE\n");
        reg_state = new_reg_state;
        dig_to_bin();
        new_state();
    }
}

void rc_receive_state()
{
    new_reg_state = modbus_get_holding(PULT_ID, REG_GET_STATE);
    DEBUG_RC("new_reg_state = %i\n", new_reg_state);
    if (new_reg_state < 0 && count_state < REPEAT_RETRY)
    {
        new_reg_state = modbus_get_holding(PULT_ID, REG_GET_STATE);
        count_state++;
    }
    DEBUG_RC("count_state = %i\n", count_state);
    if (count_state == REPEAT_RETRY)
    {
        device->remote_control.error = true;
        write_device_params(device);
        DEBUG_RC("task remote_control delete\n");
//        vTaskDelete(xHandle);
    }
}

void rc_receive_ten()
{
    new_reg_ten = modbus_get_holding(PULT_ID, REG_SET_GET_TEN);
    DEBUG_RC("new_reg_ten = %i\n", new_reg_ten);
    if (new_reg_ten < 0 && count_ten < REPEAT_RETRY)
    {
        new_reg_ten = modbus_get_holding(PULT_ID, REG_SET_GET_TEN);
        count_ten++;
    }
    DEBUG_RC("count_ten = %i\n", count_ten);
    if (count_ten == REPEAT_RETRY)
    {
        device->remote_control.error = true;
        write_device_params(device);
        DEBUG_RC("task remote_control delete\n");
//        vTaskDelete(xHandle);
    }
}

void rc_receive_handler()
{
    // читаем регистр статуса
    rc_receive_state();
    rc_parse_state();
    // читаем регистр тена, если прибор включен
    if (device->state)
    {
//        vTaskDelay(10 / portTICK_RATE_MS);
        rc_receive_ten();
        rc_parse_ten();
    }
}

void check_limit_ten()
{
    if (limit_ten)
    {
        limit_ten = false;
        modbus_set_holding(PULT_ID, REG_SET_GET_TEN, reg_ten);
    }
}

void check_changes()
{
    // проверяем изменения вкл/выкл прибора
    if (device->state != bin[7])
        modbus_set_holding(PULT_ID, REG_SET_GET_SWITCH, device->state);
    if (device->state == ON)
    {
        // проверяем изменения в установке температуры тена
        if (heaters->ten.temp_limit != reg_ten)
        {
            modbus_set_holding(PULT_ID, REG_SET_GET_TEN, heaters->ten.temp_limit);
            DEBUG_RC("sent a message ten\n");
        }

        // проверяем изменения в установке режима INFLOW_MODE
        if (device->mode == INFLOW_MODE && bin[4] == 0 && bin[5] == 0)
        {
            modbus_set_holding(PULT_ID, REG_SET_GET_MODE, SET_INFLOW_MODE);
            DEBUG_RC("sent a message INFLOW_MODE\n");
        }
        // проверяем изменения в установке режима INFLOW_MAX_MODE
        if (device->mode == INFLOW_MAX_MODE && bin[5] == 0 && bin[4] == 1)
        {
            modbus_set_holding(PULT_ID, REG_SET_GET_MODE, SET_INFLOW_MAX_MODE);
            DEBUG_RC("sent a message INFLOW_MAX_MODE\n");
        }

        // проверяем изменения в установке 1 скорости
        if (device->mode == INFLOW_MODE && device->inflow_speed == 1 && (bin[1] * 10 + bin[0]) != 11)
        {
            modbus_set_holding(PULT_ID, REG_SET_GET_WIND, SET_INFLOW_SPEED_0);
            DEBUG_RC("sent a message speed == 1\n");
        }
        // проверяем изменения в установке 3 скорости
        else if (device->mode == INFLOW_MODE && (device->inflow_speed == 2 || device->inflow_speed == 3 || device->inflow_speed == 4) && (bin[1] * 10 + bin[0]) != 0)
        {
            modbus_set_holding(PULT_ID, REG_SET_GET_WIND, SET_INFLOW_SPEED_2);
            DEBUG_RC("sent a message speed == 3\n");
        }
        // проверяем изменения в установке 5 скорости
        else if (device->mode == INFLOW_MODE && (device->inflow_speed == 5 || device->inflow_speed == 6) && (bin[1] * 10 + bin[0]) != 1)
        {
            modbus_set_holding(PULT_ID, REG_SET_GET_WIND, SET_INFLOW_SPEED_5);
            DEBUG_RC("sent a message speed == 5\n");
        }
        // проверяем изменения в установке 7 скорости
        else if (device->mode == INFLOW_MODE && device->inflow_speed == 7 && (bin[1] * 10 + bin[0]) != 10)
        {
            modbus_set_holding(PULT_ID, REG_SET_GET_WIND, SET_INFLOW_SPEED_7);
            DEBUG_RC("sent a message speed == 7\n");
        }
    }
}

void rc_send_handler()
{
    check_limit_ten();
    check_changes();
}

void new_pult()
{

}

void RemoteControlTask(void *argument)
{
	for (;;)
	{
//		new_pult();
	    rc_receive_handler();
	    rc_send_handler();
	    osDelay(1000);
	}
}

void create_remote_control_task()
{
	modbusMasterTaskHandle = osThreadNew(RemoteControlTask, NULL, &modbusMasterTask_attributes);
}

void check_remote_control()
{
	if (modbus_master_get_one(PULT_ID,  MB_FC_READ_REGISTERS, REG_GET_STATE) >= 0)
	{
		device->remote_control.find = ON;
		create_remote_control_task();
	}
	else
	{
	    if (device->remote_control.find)
	    {
	        device->remote_control.error = ON;
	    }
	}
	write_device_params();
}

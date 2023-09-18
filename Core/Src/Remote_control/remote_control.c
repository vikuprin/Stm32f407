#include "remote_control.h"
#include "FreeRTOS.h"
#include "storage.h"
#include <stdio.h>
#include "ModbusConfig.h"
#include "Modbus.h"

osThreadId_t modbusMasterTaskHandle;
const osThreadAttr_t modbusMasterTask_attributes = {
  .name = "modbusMasterTask",
  .stack_size = 256 * 6,
  .priority = (osPriority_t) osPriorityNormal,
};

int8_t modbus_set_holding(uint8_t slave_id, uint16_t reg_start, uint16_t data)
{
	return modbus_master_set_one(slave_id, MB_FC_WRITE_MULTIPLE_REGISTERS, reg_start, data);
}

int32_t modbus_get_holding(uint8_t slave_id, uint16_t reg_start)
{
	return modbus_master_get_one(slave_id, MB_FC_READ_REGISTERS, reg_start);
}

#define LIMIT_ERROR 3
#define LIMIT_LINK 30

uint8_t reg_switch;     // данные с регистра адреса switch
int16_t new_reg_switch; // сохраненные данные с регистра адреса switch
uint8_t reg_work;       // данные с регистра адреса work
int16_t new_reg_work;   // сохраненные данные с регистра адреса work
uint8_t reg_fan;        // данные с регистра адреса fan
int16_t new_reg_fan;    // сохраненные данные с регистра адреса fan
uint8_t reg_temp;       // данные с регистра адреса temp
int16_t new_reg_temp;   // сохраненные данные с регистра адреса temp
bool reg_heater = false;
bool reg_warning = false;

bool limit_temp;         // переменная превышения значения тена
bool limit_fan;          // переменная превышения значения скорости
bool limit_work = false; // переменная если влючается смарт без любых датчиков
bool pult_link = true;   // переменная состояния связи пульта
uint8_t count_error = 0; // счетчик количества ошибочных пакетов
uint8_t count_link = 0;  // счетчик попыток соединений после разрыва связи

void rc_receive_temp()
{
    new_reg_temp = modbus_get_holding(DEFAUL_PULT_ID, REG_TEMP);
    DEBUG_RC("new_reg_temp = %i\n", new_reg_temp);
    if (new_reg_temp >= 0 && reg_temp != new_reg_temp)
    {
        reg_temp = new_reg_temp;
        if (reg_temp < 10)
        {
            limit_temp = true;
            reg_temp = 10;
        }
//        if (reg_temp > 25)
//        {
//            limit_temp = true;
//            reg_temp = 25;
//        }
        heaters->ten.temp_limit = reg_temp;
    }
}

void rc_receive_fan()
{
    new_reg_fan = modbus_get_holding(DEFAUL_PULT_ID, REG_FAN);
    DEBUG_RC("new_reg_fan = %i\n", new_reg_fan);
    if (new_reg_fan >= 0 && reg_fan != new_reg_fan)
    {
        int16_t reg_fan_smart = reg_fan;
        reg_fan = new_reg_fan;
        if (reg_fan == 0)
        {
            reg_fan = 1;
            limit_fan = true;
        }
        if (device->mode == INFLOW_MODE)
        {
            if (reg_fan == 1)
                device->inflow_speed = 1;
            else if (reg_fan == 2)
                device->inflow_speed = 3;
            else if (reg_fan == 3)
                device->inflow_speed = 5;
            else if (reg_fan == 4)
                device->inflow_speed = 7;
        }
        if (device->mode == SMART_MODE)
        {
            reg_fan = reg_fan_smart;
            limit_fan = true;
        }
        if (device->mode == INFLOW_MAX_MODE)
        {
            device->mode = INFLOW_MODE;
            if (reg_fan == 1)
                device->inflow_speed = 1;
            else if (reg_fan == 2)
                device->inflow_speed = 3;
            else if (reg_fan == 3)
                device->inflow_speed = 5;
            else if (reg_fan == 4)
                device->inflow_speed = 7;
        }
    }
}

void rc_receive_work()
{
    new_reg_work = modbus_get_holding(DEFAUL_PULT_ID, REG_WORK);
    DEBUG_RC("new_reg_work = %i\n", new_reg_work);
    if (new_reg_work >= 0 && reg_work != new_reg_work)
    {
        reg_work = new_reg_work;
        if (reg_work == 0)
            device->mode = INFLOW_MODE;
        else
        {
//            if (device->sensor_block_count > 0 || device->smart.set == true)
//            {
                device->mode = SMART_MODE;
//            }
//            else
//            {
//                reg_work = 0;
//                limit_work = true;
//            }
        }
    }
}

void rc_receive_switch()
{
    new_reg_switch = modbus_get_holding(DEFAUL_PULT_ID, REG_SWITCH);
    DEBUG_RC("new_reg_switch = %i\n", new_reg_switch);
    if (new_reg_switch >= 0)
    {
        pult_link = true;
        count_error = 0;
        if (reg_switch != new_reg_switch)
        {
            reg_switch = new_reg_switch;
            device->state = reg_switch;
        }
    }
    else
    {
        count_error++;
        if (count_error == LIMIT_ERROR)
        {
            count_error = 0;
            device->remote_control.error = true;
            write_device_params(device);
            pult_link = false;
            DEBUG_RC("remote_control communication break\n");
        }
    }
}

void rc_receive_handler()
{
    rc_receive_switch();
    if (device->state)
    {
        rc_receive_work();
        rc_receive_fan();
        rc_receive_temp();
    }
}

void check_limit_temp()
{
    if (limit_temp)
    {
        limit_temp = false;
        modbus_set_holding(DEFAUL_PULT_ID, REG_TEMP, reg_temp);
    }
}

void check_limit_fan()
{
    if (limit_fan)
    {
        limit_fan = false;
        modbus_set_holding(DEFAUL_PULT_ID, REG_FAN, reg_fan);
    }
}

void check_limit_work()
{
    if (limit_work)
    {
        limit_work = false;
        modbus_set_holding(DEFAUL_PULT_ID, REG_WORK, reg_work);
    }
}

void check_changes()
{
    // проверяем изменения вкл/выкл прибора
    if (device->state != reg_switch)
        modbus_set_holding(DEFAUL_PULT_ID, REG_SWITCH, device->state);
    if (device->state == ON)
    {
        // проверяем изменения в установке выбранного режима
        if (device->mode != reg_work)
        {
            if (device->mode == INFLOW_MODE)
            {
                modbus_set_holding(DEFAUL_PULT_ID, REG_WORK, 0);
                DEBUG_RC("sent a message INFLOW_MODE\n");
            }
            if (device->mode == SMART_MODE)
            {
                modbus_set_holding(DEFAUL_PULT_ID, REG_WORK, 1);
                DEBUG_RC("sent a message SMART_MODE\n");
            }
            if (device->mode == INFLOW_MAX_MODE)
            {
                modbus_set_holding(DEFAUL_PULT_ID, REG_WORK, 0);
                modbus_set_holding(DEFAUL_PULT_ID, REG_FAN, 4);
                DEBUG_RC("sent a message INFLOW_MAX_MODE\n");
            }
        }
        // проверяем изменения в установке скорости режима INFLOW_MODE
        if (device->mode == INFLOW_MODE)
        {
            // проверяем изменения в установке 1 скорости
            if (device->inflow_speed == 1 && reg_fan != 1)
            {
                modbus_set_holding(DEFAUL_PULT_ID, REG_FAN, 1);
                DEBUG_RC("sent a message speed == 1\n");
            }
            // проверяем изменения в установке 3 скорости
            else if ((device->inflow_speed == 2 || device->inflow_speed == 3 || device->inflow_speed == 4) && reg_fan != 2)
            {
                modbus_set_holding(DEFAUL_PULT_ID, REG_FAN, 2);
                DEBUG_RC("sent a message speed == 3\n");
            }
            // проверяем изменения в установке 5 скорости
            else if ((device->inflow_speed == 5 || device->inflow_speed == 6) && reg_fan != 3)
            {
                modbus_set_holding(DEFAUL_PULT_ID, REG_FAN, 3);
                DEBUG_RC("sent a message speed == 5\n");
            }
            // проверяем изменения в установке 7 скорости
            else if (device->inflow_speed == 7 && reg_fan != 4)
            {
                modbus_set_holding(DEFAUL_PULT_ID, REG_FAN, 4);
                DEBUG_RC("sent a message speed == 7\n");
            }
        }
        // проверяем изменения в установке температуры тена
        if (heaters->ten.temp_limit != reg_temp)
        {
            modbus_set_holding(DEFAUL_PULT_ID, REG_TEMP, heaters->ten.temp_limit);
            DEBUG_RC("sent a message ten");
        }
        // проверяем изменения в установке значка включения тена
        if (heaters->ten.power > 0 && !reg_heater)
        {
            reg_heater = true;
            modbus_set_holding(DEFAUL_PULT_ID, REG_HEATER, ON);
            DEBUG_RC("sent a message ten.power > 0\n");
        }
        if (heaters->ten.power == 0 && reg_heater)
        {
            reg_heater = false;
            modbus_set_holding(DEFAUL_PULT_ID, REG_HEATER, OFF);
            DEBUG_RC("sent a message ten.power = 0\n");
        }
    }
}

void rc_send_handler()
{
    check_limit_fan();
    check_limit_temp();
    check_changes();
    check_limit_work();
}

void try_connect()
{
    count_link++;
    if (count_link == LIMIT_LINK)
    {
        count_link = 0;
        init_modbus_master();
        new_reg_switch = modbus_get_holding(DEFAUL_PULT_ID, REG_SWITCH);
        if (new_reg_switch >= 0)
        {
            DEBUG_RC("connect pult\n");
            pult_link = true;
            device->remote_control.error = OFF;
        }
    }
}

void RemoteControlTask(void *argument)
{
	for (;;)
	{
        if (pult_link)
        {
            rc_receive_handler();
            rc_send_handler();
        }
        else
        {
        	try_connect();
        }
		osDelay(1000);
	}
}

void create_remote_control_task()
{
	modbusMasterTaskHandle = osThreadNew(RemoteControlTask, NULL, &modbusMasterTask_attributes);
}

void check_remote_control()
{
	if (modbus_set_holding(DEFAUL_PULT_ID, REG_SWITCH, 0) == 1)
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

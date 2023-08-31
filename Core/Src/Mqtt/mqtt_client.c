#include "lwip/ip_addr.h"
#include "lwip/mem.h"
#include "lwip/err.h"
#include "mqtt_client.h"
#include "mqtt_message.h"
#include "string.h"
#include "FreeRTOS.h"
#include "stdbool.h"
#include "send_server_handler.h"
#include "netif.h"
#include "cmsis_os.h"

mqtt_client_t *client;
struct mqtt_connect_client_info_t ci;
ip_addr_t ip_addr;
uint16_t mqtt_port;

/* Private function prototypes -----------------------------------------------*/
void connect_mqtt(mqtt_client_t *client);
extern struct netif gnetif;
/* Private function prototypes -----------------------------------------------*/

static void mqtt_pub_request_cb(void *arg, err_t result)
{
	if (result != ERR_OK)
	{
		DEBUG_MQTT("Publish result: %d\n", result);
	}
}

void publish_message_qos0(char topic[], char message[])
{
	void *arg = NULL;
	err_t err;
	u8_t qos = 0; /* 0 1 or 2, see MQTT specification */
	u8_t retain = 0; /* No don't retain such crappy payload... */
	err = mqtt_publish(client, topic, message, strlen(message), qos, retain, mqtt_pub_request_cb, arg);
	if (err != ERR_OK)
	{
		DEBUG_MQTT("Publish err: %d\n", err);
	}
}

void publish_message(char topic[], char message[])
{
	void *arg = NULL;
	err_t err;
	u8_t qos = 1; /* 0 1 or 2, see MQTT specification */
	u8_t retain = 0; /* No don't retain such crappy payload... */
	err = mqtt_publish(client, topic, message, strlen(message), qos, retain, mqtt_pub_request_cb, arg);
	if (err != ERR_OK)
	{
		DEBUG_MQTT("Publish err: %d\n", err);
	}
}

static int inpub_id;
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
	DEBUG_MQTT("Incoming publish at topic %s with total length %u\n", topic, (unsigned int) tot_len);
	/* Decode topic string into a user defined reference */
	if (strcmp(topic, mode_topic) == 0)
		inpub_id = 0;
	else if (strcmp(topic, system_topic) == 0)
		inpub_id = 1;
    if (wireless_params->mqtt_type != VAKIO_MQTT)
    {
        if (strcmp(topic, workmode_topic) == 0)
        	inpub_id = 2;
        else if (strcmp(topic, state_topic) == 0)
        	inpub_id = 3;
        else if (strcmp(topic, speed_topic) == 0)
        	inpub_id = 4;
        else if (strcmp(topic, temp_limit_topic) == 0)
        	inpub_id = 5;
    }
}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
	DEBUG_MQTT("Incoming publish payload with length %d, flags %u\n", len, (unsigned int) flags);
	if (flags & MQTT_DATA_FLAG_LAST)
	{
		DEBUG_MQTT("mqtt_incoming_data_cb: %s\n", (char*) data);
		if (inpub_id == 0)            //mode_topic
			mode_topic_handler(data);
		else if (inpub_id == 1)       //system topic
			system_topic_handler(data);
	    if (wireless_params->mqtt_type != VAKIO_MQTT)
	    {
            if (inpub_id == 2)        //workmode topic)
            {
                if (!iSendMode)
                {
                    if (strcmp((char*)data, "inflow_mode") == 0)
                        device->mode = INFLOW_MODE;
                    else if (strcmp((char*)data, "inflow_max_mode") == 0)
                        device->mode = INFLOW_MAX_MODE;
                    device_check.mode = device->mode;
                    device_send.mode = device->mode;
                }
                iSendMode = false;
            }
            else if (inpub_id == 3)   //state topic
            {
                if (!iSendState)
                {

                    if (strcmp((char*)data, "on") == 0)
                        device->state = true;
                    else if (strcmp((char*)data, "off") == 0)
                        device->state = false;
                    device_check.state = device->state;
                    device_send.state = device->state;
                }
                iSendState = false;
            }
            else if (inpub_id == 4)   //speed topic
            {
                if (!iSendSpeed)
                 {
                     uint8_t speed_ = data[0] - '0';
                     device->inflow_speed = speed_;
                     if (speed_ > 7)
                         device->inflow_speed = 7;
                     device_check.inflow_speed = device->inflow_speed;
                     device_send.inflow_speed = device->inflow_speed;
                 }
                 iSendSpeed = false;
            }
            else if (inpub_id == 5)   //temp_limit topic
            {
                if (!iSendTemp)
                {
                    int temp_ = 0;
                    temp_ = atoi((char*)data);
                    heaters->ten.temp_limit = temp_;
                    if (temp_ > 30)
                        heaters->ten.temp_limit = 30;
                    if (temp_ < 10)
                        heaters->ten.temp_limit = 10;
                    heaters_check.ten.temp_limit = heaters->ten.temp_limit;
                    heaters_send.ten.temp_limit = heaters->ten.temp_limit;
                }
                iSendTemp = false;
            }
	    }
	memset((char*)data, 0, len);
	}
	else
	{
		/* Handle fragmented payload, store in buffer, write to file or whatever */
	}
}

uint8_t count_sub_request_cb = 0;
bool sub_request_cb = false;
static void mqtt_sub_request_cb(void *arg, err_t result)
{
	DEBUG_MQTT("Subscribe result: %d\n", result);
	count_sub_request_cb--;
	if (count_sub_request_cb == 0)
	{
		sub_request_cb = true;
	}
}

char *on_off_str_s[2] = {"off", "on"};
char *modes_str_s[2] = {"inflow_mode", "inflow_max_mode"};
char speeds_str[4];
char temp_str[4];
char gnetif_str[24];

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
	err_t err;
	if (status == MQTT_CONNECT_ACCEPTED)
	{
		DEBUG_MQTT("mqtt_connection_cb: Successfully connected\n");
		mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);

		/* Subscribe to a topic named "subtopic" with QoS level 1, call mqtt_sub_request_cb with result */
		err = mqtt_subscribe(client, mode_topic, 1, mqtt_sub_request_cb, arg);
		if (err == ERR_OK)
		{
			count_sub_request_cb++;
			DEBUG_MQTT("subscribe to mode_topic\n");
		}
		err = mqtt_subscribe(client, system_topic, 1, mqtt_sub_request_cb, arg);
		if (err == ERR_OK)
		{
			count_sub_request_cb++;
			DEBUG_MQTT("subscribe to system_topic\n");
		}
		if (wireless_params->mqtt_type != VAKIO_MQTT)
		{
			err = mqtt_subscribe(client, speed_topic, 1, mqtt_sub_request_cb, arg);
			if (err == ERR_OK)
			{
				count_sub_request_cb++;
				DEBUG_MQTT("subscribe to speed_topic\n");
			}
			err = mqtt_subscribe(client, temp_limit_topic, 1, mqtt_sub_request_cb, arg);
			if (err == ERR_OK)
			{
				count_sub_request_cb++;
				DEBUG_MQTT("subscribe to temp_limit_topic\n");
			}
			err = mqtt_subscribe(client, workmode_topic, 1, mqtt_sub_request_cb, arg);
			if (err == ERR_OK)
			{
				count_sub_request_cb++;
				DEBUG_MQTT("subscribe to workmode_topic\n");
			}
			err = mqtt_subscribe(client, state_topic, 1, mqtt_sub_request_cb, arg);
			if (err == ERR_OK)
			{
				count_sub_request_cb++;
				DEBUG_MQTT("subscribe to state_topic\n");
			}
		}
	}
	else
	{
		DEBUG_MQTT("mqtt_connection_cb: Disconnected, reason: %d\n", status);
		mqtt_status = false;
		mqtt_disconnect(client);
		init_mqtt();
	}
}

void connect_mqtt(mqtt_client_t *client)
{
	err_t err;
	err = mqtt_client_connect(client, &ip_addr, mqtt_port, mqtt_connection_cb, 0, &ci);
	if (err != ERR_OK)
	{
		mqtt_status = false;
		DEBUG_MQTT("mqtt_connect return %d\n", err);
	}
	else
	{
		mqtt_status = true;
		DEBUG_MQTT("client ok\n");
	}
}

void publish_message_topic()
{
	/* Publish message to a topic*/
    if (wireless_params->mqtt_type != VAKIO_MQTT)
    {
        	publish_message(workmode_topic, modes_str_s[device->mode]);
        	publish_message(state_topic, on_off_str_s[device->state]);
            sprintf(speeds_str, "%i", device->inflow_speed);
            publish_message(speed_topic, speeds_str);
            sprintf(temp_str, "%i", heaters->ten.temp_limit);
            publish_message(temp_limit_topic, temp_str);
    }
        sprintf(gnetif_str, "%x:%x:%x:%x:%x:%x", gnetif.hwaddr[0], gnetif.hwaddr[1], gnetif.hwaddr[2], gnetif.hwaddr[3], gnetif.hwaddr[4], gnetif.hwaddr[5]);
        publish_auth(VERSION, gnetif_str, SERIES, SUBTYPE, XTAL_FREQ);
        publish_capabilities();
        publish_settings();
        publish_errors();
}

void start_mqtt()
{
	if(netif_link)
	{
		if(!mqtt_status)
			connect_mqtt(client);
		if(sub_request_cb)
		{
			sub_request_cb = false;
			publish_message_topic();
		}
//			send_server_task();
	}
}

void update_mqtt_parameters(char *host, int port, char *username, char *password, char *client_id)
{
	memset(&ci, 0, sizeof(ci));
	IP4_ADDR(&ip_addr, host[0], host[1], host[2], host[3]);
	mqtt_port = port;
    ci.client_user = username;
    ci.client_pass = password;
    ci.client_id = client_id;
    ci.keep_alive = 60;
    DEBUG_MQTT("***************UPDATE MQTT***********\n");
    DEBUG_MQTT("host = %i.%i.%i.%i\n", host[0], host[1], host[2], host[3]);
    DEBUG_MQTT("login = %s\n", ci.client_user);
    DEBUG_MQTT("password = %s\n", ci.client_pass);
    DEBUG_MQTT("port = %i\n", mqtt_port);
    DEBUG_MQTT("client_id = %s\n", ci.client_id);
    DEBUG_MQTT("***********************************\n");
}

void set_mqtt_parameters()
{
    // Передаем параметры для подключения в зависимости от выбранного типа mqtt
    if (wireless_params->mqtt_type == VAKIO_MQTT)
    {
        sprintf(wireless_params->vakio_mqtt.client_id, "%s_%s", wireless_params->vakio.device_id, gnetif.hwaddr);
        update_mqtt_parameters(wireless_params->vakio_mqtt.host,
                               wireless_params->vakio_mqtt.port,
                               wireless_params->vakio_mqtt.login,
                               wireless_params->vakio_mqtt.password,
                               wireless_params->vakio_mqtt.client_id);
        DEBUG_MQTT("DEVICE LOGIN %s\n", wireless_params->vakio_mqtt.login);
        DEBUG_MQTT("DEVICE PASSWORD %s\n", wireless_params->vakio_mqtt.password);
        // Записываем топики для подписки и публикаций
        sprintf(log_pub_topic, "device/%s/%s/VK200/log", wireless_params->vakio.user_id, wireless_params->vakio.device_id);
        sprintf(mode_topic, "server/%s/%s/VK200/mode", wireless_params->vakio.user_id, wireless_params->vakio.device_id);
        sprintf(mode_pub_topic, "device/%s/%s/VK200/mode", wireless_params->vakio.user_id, wireless_params->vakio.device_id);
        sprintf(system_topic, "server/%s/%s/VK200/system", wireless_params->vakio.user_id, wireless_params->vakio.device_id);
        sprintf(system_pub_topic, "device/%s/%s/VK200/system", wireless_params->vakio.user_id, wireless_params->vakio.device_id);
        sprintf(temp_log_pub_topic, "device/%s/%s/VK200/temp", wireless_params->vakio.user_id, wireless_params->vakio.device_id);
    }
    else
    {
        update_mqtt_parameters(wireless_params->user_mqtt.host,
                               wireless_params->user_mqtt.port,
                               wireless_params->user_mqtt.login,
                               wireless_params->user_mqtt.password,
                               wireless_params->user_mqtt.client_id);
        // Записываем топики для подписки и публикаций
        sprintf(log_pub_topic, "device/%s/VK200/log", wireless_params->user_mqtt.topic);
        sprintf(mode_topic, "server/%s/VK200/mode", wireless_params->user_mqtt.topic);
        sprintf(mode_pub_topic, "device/%s/VK200/mode", wireless_params->user_mqtt.topic);
        sprintf(system_topic, "server/%s/VK200/system", wireless_params->user_mqtt.topic);
        sprintf(system_pub_topic, "device/%s/VK200/system", wireless_params->user_mqtt.topic);
        sprintf(workmode_topic, "%s/workmode", wireless_params->user_mqtt.topic);     // изменения
        sprintf(speed_topic, "%s/speed", wireless_params->user_mqtt.topic);           // изменения
        sprintf(temp_limit_topic, "%s/temp_limit", wireless_params->user_mqtt.topic); // изменения
        sprintf(state_topic, "%s/state", wireless_params->user_mqtt.topic);           // изменения
    }
    DEBUG_MQTT("mode_topic %s\n", mode_topic);
    DEBUG_MQTT("mode_pub_topic %s\n", mode_pub_topic);
    DEBUG_MQTT("system_topic %s\n", system_topic);
    DEBUG_MQTT("system_pub_topic %s\n", system_pub_topic);
}

void set_user_test()
{
	wireless_params->mqtt_type = USER_MQTT;

	uint8_t test_ip[] = { 195, 140, 146, 112 };
	memcpy(wireless_params->user_mqtt.host, test_ip, 4);
	wireless_params->user_mqtt.port = 12212;
	strcpy(wireless_params->user_mqtt.login, "u_Ioo5RJ\0");
	strcpy(wireless_params->user_mqtt.password, "7L8Uuagq\0");
	strcpy(wireless_params->user_mqtt.client_id, "STM32\0");
	strcpy(wireless_params->user_mqtt.topic, "VAKIO");
}

void init_mqtt()
{
	client = mqtt_client_new();
	set_user_test();
	set_mqtt_parameters();
}

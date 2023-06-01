#include "lwip/ip_addr.h"
#include "lwip/mem.h"
#include "lwip/err.h"
#include "mqtt_client.h"
#include "string.h"
#include "FreeRTOS.h"
#include "stdbool.h"
#include "cmsis_os.h"

#define PORT_MQTT    12212
uint8_t server_ip[] = { 195, 140, 146, 112 };

mqtt_client_t *client;
ip_addr_t ip_addr;
/* Private function prototypes -----------------------------------------------*/
void connect_mqtt(mqtt_client_t *client);

static void mqtt_pub_request_cb(void *arg, err_t result) {
	if (result != ERR_OK) {
		DEBUG_MQTT("Publish result: %d\n", result);
	}
}

void publish_mqtt(mqtt_client_t *client, void *arg) {
	char message[10];
	snprintf(message, 10, "on_svet");

	err_t err;
	u8_t qos = 0; /* 0 1 or 2, see MQTT specification */
	u8_t retain = 0; /* No don't retain such crappy payload... */
	err = mqtt_publish(client, "esp/svet", message, 10, qos, retain,
			mqtt_pub_request_cb, arg);
	if (err != ERR_OK) {
		DEBUG_MQTT("Publish err: %d\n", err);
	}
//	mqtt_disconnect(client);
}

/* Called when publish is complete either with sucess or failure */
/* The idea is to demultiplex topic and create some reference to be used in data callbacks
 Example here uses a global variable, better would be to use a member in arg
 If RAM and CPU budget allows it, the easiest implementation might be to just take a copy of
 the topic string and use it in mqtt_incoming_data_cb
 */
static int inpub_id;
static void mqtt_incoming_publish_cb(void *arg, const char *topic,
		u32_t tot_len) {
	DEBUG_MQTT("Incoming publish at topic %s with total length %u\n", topic,
			(unsigned int) tot_len);

	/* Decode topic string into a user defined reference */
	if (strcmp(topic, "print_payload") == 0) {
		inpub_id = 0;
	} else if (topic[0] == 'A') {
		/* All topics starting with 'A' might be handled at the same way */
		inpub_id = 1;
	} else {
		/* For all other topics */
		inpub_id = 2;
	}
}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len,
		u8_t flags) {
	DEBUG_MQTT("Incoming publish payload with length %d, flags %u\n", len,
			(unsigned int) flags);

	if (flags & MQTT_DATA_FLAG_LAST) {
		/* Last fragment of payload received (or whole part if payload fits receive buffer
		 See MQTT_VAR_HEADER_BUFFER_LEN)  */

		/* Call function or do action depending on reference, in this case inpub_id */
		if (inpub_id == 0) {
			/* Don't trust the publisher, check zero termination */
			if (data[len - 1] == 0) {
				DEBUG_MQTT("mqtt_incoming_data_cb: %s\n", (const char*) data);
			}
		} else if (inpub_id == 1) {
			/* Call an 'A' function... */
		} else {
			DEBUG_MQTT("mqtt_incoming_data_cb: Ignoring payload...\n");
		}
	} else {
		/* Handle fragmented payload, store in buffer, write to file or whatever */
	}
}

static void mqtt_sub_request_cb(void *arg, err_t result) {
	/* Just print the result code here for simplicity,
	 normal behaviour would be to take some action if subscribe fails like
	 notifying user, retry subscribe or disconnect from server */
	DEBUG_MQTT("Subscribe result: %d\n", result);
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg,
		mqtt_connection_status_t status) {
	err_t err;
	if (status == MQTT_CONNECT_ACCEPTED) {
		DEBUG_MQTT("mqtt_connection_cb: Successfully connected\n");

		/* Setup callback for incoming publish requests */
		mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb,
				mqtt_incoming_data_cb, arg);

		/* Subscribe to a topic named "subtopic" with QoS level 1, call mqtt_sub_request_cb with result */
		err = mqtt_subscribe(client, "esp/svet", 1, mqtt_sub_request_cb, arg);

		if (err != ERR_OK) {
			DEBUG_MQTT("mqtt_subscribe return: %d\n", err);
		}
	} else {
		DEBUG_MQTT("mqtt_connection_cb: Disconnected, reason: %d\n", status);

		/* Its more nice to be connected, so try to reconnect */
		connect_mqtt(client);
	}
}

bool mqtt_link;

void connect_mqtt(mqtt_client_t *client) {
	struct mqtt_connect_client_info_t ci;
	err_t err;

	/* Setup an empty client info structure */
	memset(&ci, 0, sizeof(ci));

	/* Minimal amount of information required is client identifier, so set it here */
	ci.client_id = "lwip";
	ci.client_user = "u_Ioo5RJ";
	ci.client_pass = "7L8Uuagq";
	//ci.keep_alive = 60;

	/* Initiate client and connect to server, if this fails immediately an error code is returned
	 otherwise mqtt_connection_cb will be called with connection result after attempting
	 to establish a connection with the server.
	 For now MQTT version 3.1.1 is always used */

	err = mqtt_client_connect(client, &ip_addr, PORT_MQTT, mqtt_connection_cb,
			0, &ci);
	if (err != ERR_OK)
	{
		mqtt_link = false;
		DEBUG_MQTT("mqtt_connect return %d\n", err);
	}
	else
	{
		mqtt_link = true;
		DEBUG_MQTT("client ok\n");
	}
}

void init_mqtt()
{
	client = mqtt_client_new();
	IP4_ADDR(&ip_addr, server_ip[0], server_ip[1], server_ip[2], server_ip[3]);
	if (client != NULL)
	{
		connect_mqtt(client);
	}
}

void start_mqtt()
{
	if(mqtt_link == false)
	{
		DEBUG_MQTT("client zanovo\n");
		connect_mqtt(client);
	}
	publish_mqtt(client, NULL);
}

void MqttClientTask(void *argument)
{
//	client = mqtt_client_new();
//	IP4_ADDR(&ip_addr, server_ip[0], server_ip[1], server_ip[2], server_ip[3]);
//	if (client != NULL)
//	{
//		connect_mqtt(client);
//	}
	for (;;)
	{
			client = mqtt_client_new();
			IP4_ADDR(&ip_addr, server_ip[0], server_ip[1], server_ip[2], server_ip[3]);
			if (client != NULL)
			{
				connect_mqtt(client);
			}

		publish_mqtt(client, NULL);
		osDelay(1000);
	}
}

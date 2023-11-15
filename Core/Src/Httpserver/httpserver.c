
/* Includes ------------------------------------------------------------------*/
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/apps/fs.h"
#include "string.h"
#include <stdio.h>
#include "httpserver.h"
#include "cmsis_os.h"
#include "lwip/apps/httpd.h"
#include "mqtt_client.h"
#include "storage.h"
#include "cJSON.h"
#include "FLASH_SECTOR_F4.h"

/************************ CGI HANDLER ***************************/
const char *CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char *CGILED_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

const tCGI FORM_CGI = {"/form.cgi", CGIForm_Handler};
const tCGI LED_CGI = {"/led.cgi", CGILED_Handler};

char name[30];
tCGI CGI_TAB[2];

const char *CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	if (iIndex == 0)
	{
		for (int i=0; i<iNumParams; i++)
		{
			if (strcmp(pcParam[i], "mqttBroker") == 0)  // if the fname string is found
			{
				memset(name, '\0', 30);
				strcpy(name, pcValue[i]);
				wireless_params->mqtt_type = USER_MQTT;
			}

			else if (strcmp(pcParam[i], "mqttIP") == 0)  // if the fname string is found
			{
				memset(wireless_params->user_mqtt.host, '\0', 16);
				strcpy(wireless_params->user_mqtt.host, pcValue[i]);
			}

			else if (strcmp(pcParam[i], "mqttPort") == 0)  // if the fname string is found
			{
				wireless_params->user_mqtt.port = atoi(pcValue[i]);
			}

			else if (strcmp(pcParam[i], "mqttLogin") == 0)  // if the fname string is found
			{
				memset(wireless_params->user_mqtt.login, '\0', 12);
				strcpy(wireless_params->user_mqtt.login, pcValue[i]);
			}

			else if (strcmp(pcParam[i], "mqttPassword") == 0)  // if the fname string is found
			{
				memset(wireless_params->user_mqtt.password, '\0', 12);
				strcpy(wireless_params->user_mqtt.password, pcValue[i]);
			}

			else if (strcmp(pcParam[i], "mqttID") == 0)  // if the fname string is found
			{
				memset(wireless_params->user_mqtt.client_id, '\0', 12);
				strcpy(wireless_params->user_mqtt.client_id, pcValue[i]);
			}

			else if (strcmp(pcParam[i], "mqttTopic") == 0)  // if the fname string is found
			{
				memset(wireless_params->user_mqtt.topic, '\0', 12);
				strcpy(wireless_params->user_mqtt.topic, pcValue[i]);
			}
		}
	}
	write_wireless_params();
	set_mqtt_parameters();
	return "/cgiform.html";
}


const char *CGILED_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	if (iIndex == 1)
	{
		for (int i=0; i<iNumParams; i++)
		{
			if (strcmp(pcParam[i], "fname") == 0)  // if the fname string is found
			{
				memset(name, '\0', 30);
				strcpy(name, pcValue[i]);
			}

			else if (strcmp(pcParam[i], "lname") == 0)  // if the fname string is found
			{
				strcat(name, " ");
				strcat(name, pcValue[i]);
			}
		}
	}

	return "/cgiled.html";
}

void http_cgi_init()
{
	httpd_init();

	CGI_TAB[0] = FORM_CGI;
	CGI_TAB[1] = LED_CGI;

//	http_set_cgi_handlers (&FORM_CGI, 1);
	http_set_cgi_handlers (CGI_TAB, 2);
}
/************************ CGI HANDLER ***************************/

extern struct netif gnetif;

char ota_url[300];
void set_ota_url(char *url)
{
    char *full_url = malloc(300);
    while (full_url == NULL)
    {
        vTaskDelay(10);
        full_url = malloc(300);
    }
    sprintf(full_url, "http://%s/updatefirmware/cityair/esp32/%s/%s/%s", url, SUBTYPE, XTAL_FREQ, wireless_params->vakio.device_id);
    DEBUG_SERVER("set ota url %s", full_url);
    strcpy(ota_url, full_url);
    free(full_url);
}

extern FLASH_ProcessTypeDef pFlash;

static char temp_str[544];
static char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: application/json\r\n\r\n";
static char http_redirect[] = "HTTP/1.1 303 See Other\r\nLocation: /index.html";

int address_flash = OTA_ADDR_FLASH;
static int flash_data(char* buf, int len)
{
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_5);

  HAL_StatusTypeDef ret;

  ret = HAL_FLASH_Lock();
  if(ret != HAL_OK)
  {
    return ret;
  }
  ret = HAL_FLASH_Unlock();
  if (ret != HAL_OK)
  {
    return ret;
  }

  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP
                         | FLASH_FLAG_OPERR
                         | FLASH_FLAG_WRPERR
                         | FLASH_FLAG_PGAERR
                         | FLASH_FLAG_PGSERR);

  if (pFlash.ErrorCode != 0)
  {
      return pFlash.ErrorCode;
  }

  uint32_t start_addr = address_flash;
  for (int i = 0; i < len; i++)
  {
    FLASH_WaitForLastOperation(HAL_MAX_DELAY);
    ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,
                            address_flash,
                            buf[i]);
    if (ret != HAL_OK)
    {
      printf("App Flash Write Error\n");
      break;
    }
    address_flash++;
  }

  FLASH_WaitForLastOperation(500);

  for (int i = 0; i < len; i++)
  {
    if (buf[i] != *(uint8_t*)(start_addr + i))
    {
      printf("Error check\n");
    }
  }

  ret = HAL_FLASH_Lock();
  if(ret != HAL_OK)
  {
    return ret;
  }

  return ret;
}

void erase_sectors()
{
	Flash_Delete_Data(OTA_ADDR_FLASH);
	Flash_Delete_Data(OTA_ADDR1_FLASH);
	Flash_Delete_Data(OTA_ADDR2_FLASH);
	printf("Erase sectors!\n");
}

static int compare_sectors()
{
  HAL_StatusTypeDef ret;

  ret = HAL_FLASH_Unlock();
  if (ret != HAL_OK)
  {
    return ret;
  }

  for (int i = OTA_ADDR_FLASH; i < OTA_ADDR_FLASH + 0x10000; i++)
  {
    if (*(uint8_t*)i != 0xff)
    {
      printf("error\n");
    }
  }

  ret = HAL_FLASH_Lock();
  if (ret != HAL_OK)
  {
    return ret;
  }

  printf("Read sectors!\n");
  return ret;
}

int content_length;
static int http_get_content_length(char* buf)
{
  int length = -1;
  char* str_size = strstr(buf, "Content-Length: ");
  char tmp_str[8];
  if (str_size != NULL)
  {
    str_size += 16;
    int i;
    for (i = 0; str_size[i] != '\r'; i++){}
    strncpy(tmp_str, str_size, i);
    length = atoi(tmp_str);
  }
  content_length = length;
  return length;
}

extern mqtt_client_t *client;
int registration_data_handler_post(char *str)
{
    cJSON *root = cJSON_Parse(str);
    while (root == NULL)
    {
        vTaskDelay(10);
        root = cJSON_Parse(str);
    }

    /* Get value of expected key from query string */
    if (cJSON_GetObjectItem(root, "uuid") != NULL)
    {
    	DEBUG_SERVER("uuid = %s\n", cJSON_GetObjectItem(root, "uuid")->valuestring);
    	char param[40];
		sprintf(param, "%s", cJSON_GetObjectItem(root, "uuid")->valuestring);
		sprintf(wireless_params->vakio_mqtt.login, "%c%c%c%c%c%c%c%c%c%c", param[0], param[1], param[4], param[6], param[7], param[8], param[11], param[10], param[19], param[18]);
		sprintf(wireless_params->vakio_mqtt.password, "%c%c%c%c%c%c%c%c%c%c", param[3], param[5], param[9], param[13], param[12], param[14], param[15], param[16], param[2], param[17]);
    }
    if (cJSON_GetObjectItem(root, "user_id") != NULL)
    {
    	DEBUG_SERVER("user_id = %d\n", cJSON_GetObjectItem(root, "user_id")->valueint);
        char param[30];
        sprintf(param, "%i", cJSON_GetObjectItem(root, "user_id")->valueint);
        strcpy(wireless_params->vakio.user_id, param);
    }
    if (cJSON_GetObjectItem(root, "device_id") != NULL)
    {
    	DEBUG_SERVER("device_id = %d\n", cJSON_GetObjectItem(root, "device_id")->valueint);
        char param[30];
        sprintf(param, "%i", cJSON_GetObjectItem(root, "device_id")->valueint);
        strcpy(wireless_params->vakio.device_id, param);
    }
    if (cJSON_GetObjectItem(root, "mqtt_ip") != NULL)
    {
    	DEBUG_SERVER("mqtt_ip = %s\n", cJSON_GetObjectItem(root, "mqtt_ip")->valuestring);
        char param[80];
        sprintf(param, "%s", cJSON_GetObjectItem(root, "mqtt_ip")->valuestring);
        strcpy(wireless_params->vakio_mqtt.host, param);
    }
    if (cJSON_GetObjectItem(root, "server_ip") != NULL)
    {
    	DEBUG_SERVER("server_ip = %s\n", cJSON_GetObjectItem(root, "server_ip")->valuestring);
        char param[80];
        sprintf(param, "%s", cJSON_GetObjectItem(root, "server_ip")->valuestring);
        strcpy(wireless_params->server_ip, param);
    }
    if (cJSON_GetObjectItem(root, "domain") != NULL)
    {
    	DEBUG_SERVER("domain = %s\n", cJSON_GetObjectItem(root, "domain")->valuestring);
        char param[150];
        sprintf(param, "%s", cJSON_GetObjectItem(root, "domain")->valuestring);
        strcpy(wireless_params->domain, param);
        set_ota_url(wireless_params->domain);
    }
    wireless_params->mqtt_type = VAKIO_MQTT;
    mqtt_disconnect(client);
    init_mqtt();
    write_wireless_params();

    cJSON_Delete(root);

    return 0;
}
static char* http_get_recv_str(char* first_chunk, int size_first_chunk, struct netconn *conn,
                               struct netbuf *inbuf, int content_length)
{
  int cont_len = content_length;
  char *str = strstr(first_chunk, "\r\n\r\n");
  str += 4;

  int size = size_first_chunk - (str - first_chunk);
  char *res_str = (char*)malloc(content_length * sizeof(char) + 1);
  strncpy(res_str, str, size);
  cont_len -= size;

  char* buf;
  u16_t buflen;

  char *cursor = res_str + size;
  while (cont_len != 0)
  {
    netbuf_delete(inbuf);
    netconn_recv(conn, &inbuf);
    netbuf_data(inbuf, (void**) &buf, &buflen);
    strncpy(cursor, buf, buflen);
    cursor += buflen;
    cont_len -= buflen;
  }
  res_str[content_length] = '\0';
  printf("FINAL STR = %s\n", res_str);

  registration_data_handler_post(res_str);

  return res_str;
}


static void http_server(struct netconn *conn)
{
  struct netbuf *inbuf;
  err_t recv_err;
  char* buf;
  u16_t buflen;
  struct fs_file file;

  /* Read the data from the port, blocking if nothing yet there.
   We assume the request (the part we care about) is in one netbuf */
  recv_err = netconn_recv(conn, &inbuf);

  if (recv_err == ERR_OK)
  {
    if (netconn_err(conn) == ERR_OK)
    {
      netbuf_data(inbuf, (void**)&buf, &buflen);

      /* Check if request to get ST.gif */
      if (strncmp((char const *)buf,"GET /index.html", 15) == 0)
      {
        fs_open(&file, "/index.html");
        netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
        fs_close(&file);
      }
      else if (strncmp((char const *)buf,"POST /send",10)==0)
      {
    	http_get_content_length(buf);
        erase_sectors();
        compare_sectors();

        char* temp_buf = NULL;

        if (recv_err == ERR_OK)
        {
          netbuf_delete(inbuf);
          recv_err = netconn_recv(conn, &inbuf);
          netbuf_data(inbuf, (void**) &buf, &buflen);
        }
        if (recv_err == ERR_OK)
        {
          netbuf_delete(inbuf);
          recv_err = netconn_recv(conn, &inbuf);
          netbuf_data(inbuf, (void**) &buf, &buflen);
          memcpy(temp_str, buf, buflen);

          temp_buf = strstr(temp_str, "Content-Type: ");
          temp_buf = strstr(temp_buf, "\r\n\r\n");
          temp_buf += 4;
          buflen -= (temp_buf - temp_str);
          flash_data(temp_buf, buflen);
        }

        int i = 0;
        while(recv_err == ERR_OK)
        {
          netbuf_delete(inbuf);
          recv_err = netconn_recv(conn, &inbuf);
          netbuf_data(inbuf, (void**) &buf, &buflen);
          memcpy(temp_str, buf, buflen);

          flash_data(temp_str, buflen);
          if (buflen < 536)
             break;
          i++;
        }

        netconn_write(conn, http_redirect, sizeof(http_redirect)-1, NETCONN_NOCOPY);

        device->ota_len = content_length;
		write_device_params();

		HAL_RCC_DeInit();
		HAL_DeInit();
		__disable_irq();
		__set_MSP(*((volatile uint32_t *) (BOOT_ADDR_FLASH)));
		__DMB();
		SCB->VTOR = BOOT_ADDR_FLASH;
		__DSB();
	    uint32_t JumpAddress = *((volatile uint32_t*) (BOOT_ADDR_FLASH + 4));
	    void (*reset_handler) (void) = (void*) JumpAddress;
	    reset_handler();
      }
      else if (strncmp((char const *)buf,"POST /dataserver", 16) == 0)
      {
        printf("len = %d, str = %s\n", buflen, buf);
        char* tmp = http_get_recv_str(buf, buflen, conn, inbuf, http_get_content_length(buf));
        free(tmp);
        netconn_write(conn, http_html_hdr, sizeof(http_html_hdr) - 1, NETCONN_NOCOPY);
      }
      else if (strncmp((char const *)buf,"GET /mac", 8) == 0)
      {
    	char mac_str[30] = "\0";
    	sprintf(mac_str, "%x:%x:%x:%x:%x:%x", gnetif.hwaddr[0], gnetif.hwaddr[1], gnetif.hwaddr[2], gnetif.hwaddr[3], gnetif.hwaddr[4], gnetif.hwaddr[5]);
        cJSON *mac_js;
        mac_js = cJSON_CreateObject();
        while (mac_js == NULL)
        {
            vTaskDelay(10);
            mac_js = cJSON_CreateObject();
        }
        cJSON_AddStringToObject(mac_js, "mac", mac_str);
        char *js_str = cJSON_Print(mac_js);
        while (js_str == NULL)
        {
            vTaskDelay(10);
            js_str = cJSON_Print(mac_js);
        }
    	netconn_write(conn, http_html_hdr, sizeof(http_html_hdr) - 1, NETCONN_NOCOPY);
        netconn_write(conn, js_str, strlen(js_str), NETCONN_NOCOPY);
        cJSON_Delete(mac_js);
        cJSON_free(js_str);
      }
      else if (strncmp((char const *)buf,"POST /status", 12) == 0)
      {
        netconn_write(conn, http_html_hdr, sizeof(http_html_hdr) - 1, NETCONN_NOCOPY);
    	if(netif_is_link_up(&gnetif))
    		netconn_write(conn, HTTPD_200, 6, NETCONN_NOCOPY);
    	else
    		netconn_write(conn, HTTPD_304, 16, NETCONN_NOCOPY);
      }
    }
  }
  /* Close the connection (server closes in HTTP) */
  netconn_close(conn);

  /* Delete the buffer (netconn_recv gives us ownership,
   so we have to make sure to deallocate the buffer) */
  netbuf_delete(inbuf);
}

static void http_thread(void *arg)
{
  struct netconn *conn, *newconn;
  err_t err, accept_err;

  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_TCP);

  if (conn!= NULL)
  {
    /* Bind to port 80 (HTTP) with default IP address */
    err = netconn_bind(conn, IP_ADDR_ANY, 80);

    if (err == ERR_OK)
    {
      /* Put the connection into LISTEN state */
      netconn_listen(conn);

      while(1)
      {
        /* accept any incoming connection */
        accept_err = netconn_accept(conn, &newconn);
        if(accept_err == ERR_OK)
        {
          /* serve connection */
          http_server(newconn);

          /* delete connection */
          netconn_delete(newconn);
        }
      }
    }
  }
}

void http_server_init()
{
	if(HAL_GPIO_ReadPin(SW_DIP1_GPIO_Port, SW_DIP1_Pin) == GPIO_PIN_SET)
		sys_thread_new("http_thread", http_thread, NULL, DEFAULT_THREAD_STACKSIZE, osPriorityNormal);
	else
		http_cgi_init();
}

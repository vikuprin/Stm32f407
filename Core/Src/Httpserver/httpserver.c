
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

static void http_server(struct netconn *conn)
{
	struct netbuf *inbuf;
	err_t recv_err;
	char* buf;
	u16_t buflen;
	struct fs_file file;

	/* Read the data from the port, blocking if nothing yet there */
	recv_err = netconn_recv(conn, &inbuf);

	if (recv_err == ERR_OK)
	{
		if (netconn_err(conn) == ERR_OK)
		{
			/* Get the data pointer and length of the data inside a netbuf */
			netbuf_data(inbuf, (void**)&buf, &buflen);

			/* Check if request to get the index.html */
			if (strncmp((char const *)buf,"GET /index.html",15)==0)
			{
				fs_open(&file, "/index.html");
				netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
				fs_close(&file);
			}



		      else if (strncmp((char const *)buf,"POST /newdata",13)==0)
		      {
		        char* temp_buf = NULL;
                printf("buf= %s", buf);
//		        if (recv_err == ERR_OK)
//		        {
//		          netbuf_delete(inbuf);
//		          recv_err = netconn_recv(conn, &inbuf);
//		          netbuf_data(inbuf, (void**) &buf, &buflen);
//		        }
//		        if (recv_err == ERR_OK)
//		        {
//		          netbuf_delete(inbuf);
//		          recv_err = netconn_recv(conn, &inbuf);
//		          netbuf_data(inbuf, (void**) &buf, &buflen);
//		          memcpy(temp_str, buf, buflen);
//
//		          temp_buf = strstr(temp_str, "Content-Type: ");
//		          temp_buf = strstr(temp_buf, "\r\n\r\n");
//		          temp_buf += 4;
//		          buflen -= (temp_buf - temp_str);
//		          flash_data(temp_buf, buflen);
//		        }
//
//		        int i = 0;
//		        while(recv_err == ERR_OK)
//		        {
//		          netbuf_delete(inbuf);
//		          recv_err = netconn_recv(conn, &inbuf);
//		          netbuf_data(inbuf, (void**) &buf, &buflen);
//		          memcpy(temp_str, buf, buflen);
//
//		          flash_data(temp_str, buflen);
//		          if (buflen < 536)
//		          {
//		            break;
//		          }
//		          i++;
//		        }
//
//		        netconn_write(conn, http_redirect, sizeof(http_redirect)-1, NETCONN_NOCOPY);
//
//		        address_flash = 0x8040000;
//		        go_to_app();
		      }





			else
			{
				/* Load Error page */
				fs_open(&file, "/404.html");
				netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
				fs_close(&file);
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
	sys_thread_new("http_thread", http_thread, NULL, DEFAULT_THREAD_STACKSIZE, osPriorityNormal);
}
/************************ CGI HANDLER ***************************/
//const char *CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
//const char *CGILED_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
//
//const tCGI FORM_CGI = {"/form.cgi", CGIForm_Handler};
//const tCGI LED_CGI = {"/led.cgi", CGILED_Handler};
//
//char name[30];
//tCGI CGI_TAB[2];
//
//const char *CGIForm_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
//{
//	if (iIndex == 0)
//	{
//		for (int i=0; i<iNumParams; i++)
//		{
//			if (strcmp(pcParam[i], "mqttBroker") == 0)  // if the fname string is found
//			{
//				memset(name, '\0', 30);
//				strcpy(name, pcValue[i]);
//				wireless_params->mqtt_type = USER_MQTT;
//			}
//
//			else if (strcmp(pcParam[i], "mqttIP") == 0)  // if the fname string is found
//			{
//				memset(wireless_params->user_mqtt.host, '\0', 16);
//				strcpy(wireless_params->user_mqtt.host, pcValue[i]);
//			}
//
//			else if (strcmp(pcParam[i], "mqttPort") == 0)  // if the fname string is found
//			{
//				wireless_params->user_mqtt.port = atoi(pcValue[i]);
//			}
//
//			else if (strcmp(pcParam[i], "mqttLogin") == 0)  // if the fname string is found
//			{
//				memset(wireless_params->user_mqtt.login, '\0', 12);
//				strcpy(wireless_params->user_mqtt.login, pcValue[i]);
//			}
//
//			else if (strcmp(pcParam[i], "mqttPassword") == 0)  // if the fname string is found
//			{
//				memset(wireless_params->user_mqtt.password, '\0', 12);
//				strcpy(wireless_params->user_mqtt.password, pcValue[i]);
//			}
//
//			else if (strcmp(pcParam[i], "mqttID") == 0)  // if the fname string is found
//			{
//				memset(wireless_params->user_mqtt.client_id, '\0', 12);
//				strcpy(wireless_params->user_mqtt.client_id, pcValue[i]);
//			}
//
//			else if (strcmp(pcParam[i], "mqttTopic") == 0)  // if the fname string is found
//			{
//				memset(wireless_params->user_mqtt.topic, '\0', 12);
//				strcpy(wireless_params->user_mqtt.topic, pcValue[i]);
//			}
//		}
//	}
//	write_wireless_params();
//	set_mqtt_parameters();
//	return "/cgiform.html";
//}
//
//
//const char *CGILED_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
//{
//	if (iIndex == 1)
//	{
//		for (int i=0; i<iNumParams; i++)
//		{
//			if (strcmp(pcParam[i], "fname") == 0)  // if the fname string is found
//			{
//				memset(name, '\0', 30);
//				strcpy(name, pcValue[i]);
//			}
//
//			else if (strcmp(pcParam[i], "lname") == 0)  // if the fname string is found
//			{
//				strcat(name, " ");
//				strcat(name, pcValue[i]);
//			}
//		}
//	}
//
//	return "/cgiled.html";
//}
//
//void http_server_init()
//{
//	httpd_init();
//
//	CGI_TAB[0] = FORM_CGI;
//	CGI_TAB[1] = LED_CGI;
//
////	http_set_cgi_handlers (&FORM_CGI, 1);
//	http_set_cgi_handlers (CGI_TAB, 2);
//}

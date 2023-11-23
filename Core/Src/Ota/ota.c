#include "ota.h"
#include "mqtt_message.h"
#include <string.h>
#include <cmsis_os.h>
#include "httpserver.h"
#include <tcp.h>
#include "storage.h"
#include "tim.h"
#include "damper.h"

extern struct netif gnetif;
extern bool sub_request_cb;
extern int content_length;


xSemaphoreHandle ota_mutex;

void start_update_firmware()
{
    xSemaphoreGive(ota_mutex);
}
void start_update_firmware_isr()
{
    xSemaphoreGiveFromISR(ota_mutex, NULL);
}



//void OtaTask(void const * argument)
//{
//	vSemaphoreCreateBinary(ota_mutex);
//    for(;;)
//    {
//        xSemaphoreTake(ota_mutex, portMAX_DELAY);
//        DEBUG_OTA("Take mute OTA\n");
//        if (strlen(ota_url) > 3)
//        {
//            DEBUG_OTA("Start ota %s", ota_url);
//            // Начинаем прошвку
//            publish_firmware_state("start"); // изменения
//            tcp_setup();
////            if (ret == ERR_OK)
////            {
////                publish_firmware_state("done");
//                // Прошивка прошла успешно, записываем в память ФЛАГ ОБ УСПЕШНОЙ ПРОШИВКЕ
////                write_firmware_flag(10);
//                // Перезагружаемся
////                esp_restart();
////            }
////            else
////            {
////                DEBUG_OTA("Error");
////                publish_firmware_state("error"); // изменения
////                osDelay(5000);
////                xSemaphoreGive(ota_mutex);
////            }
//        }
//
//    }
//}












static err_t tcp_send_packet(struct tcp_pcb *tpcb)
{
	err_t ret_err;
    char *string = malloc(256);
    sprintf(string, "GET /updatefirmware/airflow/esp32/default/40/26376 HTTP/1.1\r\nHost: %s\r\n\r\n ", wireless_params->domain);
    /* push to buffer */
    ret_err = tcp_write(tpcb, string, strlen(string), TCP_WRITE_FLAG_COPY);
    if (ret_err != ERR_OK)
    {
        printf("ERROR: Code: %d (tcp_send_packet :: tcp_write)\n", ret_err);
        return 1;
    }
    /* now send */
    ret_err = tcp_output(tpcb);
    if (ret_err != ERR_OK)
    {
        printf("ERROR: Code: %d (tcp_send_packet :: tcp_output)\n", ret_err);
        return 1;
    }
    return ERR_OK;
}

static void tcp_client_connection_close(struct tcp_pcb *tpcb, struct tcp_client_struct *es)
{

  /* remove all callbacks */
  tcp_arg(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_recv(tpcb, NULL);
  tcp_err(tpcb, NULL);
  tcp_poll(tpcb, NULL, 0);

  /* delete es structure */
  if (es != NULL)
  {
    mem_free(es);
  }

  /* close tcp connection */
  tcp_close(tpcb);
}

static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb)
{
  err_t ret_err;
  struct tcp_client_struct *es;

  es = (struct tcp_client_struct *)arg;
  if (es != NULL)
  {
    if (es->p != NULL)
    {
    }
    else
    {
      /* no remaining pbuf (chain)  */
      if(es->state == ES_CLOSING)
      {
        /*  close tcp connection */
        tcp_client_connection_close(tpcb, es);
      }
    }
    ret_err = ERR_OK;
  }
  else
  {
    /* nothing to be done */
    tcp_abort(tpcb);
    ret_err = ERR_ABRT;
  }
  return ret_err;
}

static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
  struct tcp_client_struct *es;
  err_t ret_err;

  LWIP_ASSERT("arg != NULL",arg != NULL);

  es = (struct tcp_client_struct *)arg;

  /* if we receive an empty tcp frame from server => close connection */
  if (p == NULL)
  {
    /* remote host closed connection */
    es->state = ES_CLOSING;
    device->ota_len = content_length;
    write_device_params();
    boot_jump();
    if(es->p == NULL)
    {
       /* we're done sending, close connection */
       tcp_client_connection_close(tpcb, es);
    }
    else
    {
    }
    ret_err = ERR_OK;
  }
  /* else : a non empty frame was received from server but for some reason err != ERR_OK */
  else if(err != ERR_OK)
  {
    /* free received pbuf*/
    if (p != NULL)
    {
      es->p = NULL;
      pbuf_free(p);
    }
    ret_err = err;
  }
  else if(es->state == ES_CONNECTED)
  {
   /* store reference to incoming pbuf (chain) */
    es->p = p;

    /* Acknowledge the received data */
    tcp_recved(tpcb, p->tot_len);

    pbuf_free(p);

    ret_err = ERR_OK;
  }
  else if(es->state == ES_CLOSING)
  {
    /* odd case, remote side closing twice, trash data */
    tcp_recved(tpcb, p->tot_len);
    es->p = NULL;
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  else
  {
    /* unknown es->state, trash data  */
    tcp_recved(tpcb, p->tot_len);
    es->p = NULL;
    pbuf_free(p);
    ret_err = ERR_OK;
  }
//  printf("Number of pbufs %d\n", pbuf_clen(p));
  printf("Contents of pbuf %s\n", (char *)p->payload);
  if (content_length == 0)
  {
	  http_get_content_length((char *)p->payload);
	  printf("content_length = %i\n", content_length);

	  char* temp_buf = NULL;
	  temp_buf = strstr((char *)p->payload, "Vary: Origin");
	  temp_buf = strstr(temp_buf, "\r\n\r\n");
	  temp_buf += 4;

	  u16_t tcp_len;
	  tcp_len = p->tot_len;
	  tcp_len -= (temp_buf - (char *)p->payload);
	  flash_data(temp_buf, tcp_len);
  }
  else
  {
	  flash_data((char *)p->payload, p->tot_len);
  }

  return ret_err;
}

static err_t tcp_client_connected(void *arg, struct tcp_pcb *newpcb, err_t err)
{
  err_t ret_err;
  struct tcp_client_struct *es;

  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(err);

  /* allocate structure es to maintain tcp connection information */
  es = (struct tcp_client_struct *)mem_malloc(sizeof(struct tcp_client_struct));
  if (es != NULL)
  {
    es->state = ES_CONNECTED;
    es->pcb = newpcb;
    es->retries = 0;
    es->p = NULL;

    /* pass newly allocated es structure as argument to newpcb */
    tcp_arg(newpcb, es);

    /* initialize lwip tcp_recv callback function for newpcb  */
    tcp_recv(newpcb, tcp_client_recv);

    /* initialize lwip tcp_poll callback function for newpcb */
    tcp_poll(newpcb, tcp_client_poll, 0);

    /* initialize LwIP tcp_sent callback function */
    tcp_send_packet(newpcb);

    ret_err = ERR_OK;
  }
  else
  {
    /*  close tcp connection */
    tcp_client_connection_close(newpcb, es);
    /* return memory error */
    ret_err = ERR_MEM;
  }
  return ret_err;
}

void tcp_setup(void)
{
	struct tcp_pcb *tpcb;
	tpcb = tcp_new();

	char _server_ip[20];
	memcpy(_server_ip, wireless_params->server_ip, 20);
	char str[4];
	char sep[4]=".";
	char *istr;
	istr = strtok (_server_ip, sep);
	uint8_t i = 0;
	while (istr != NULL)
	{
		str[i] = atoi(istr);
		i++;
		istr = strtok(NULL, sep);
	}

	ip_addr_t destIPADDR;
	IP_ADDR4(&destIPADDR, str[0], str[1], str[2], str[3]);
	tcp_connect(tpcb, &destIPADDR, 5000, tcp_client_connected);
}

void test_wireless_params()
{
	strcpy(wireless_params->server_ip, "51.250.111.175\0");
	strcpy(wireless_params->domain, "service.vakio.ru\0");
}

void OtaTask(void const * argument)
{
	vSemaphoreCreateBinary(ota_mutex);
	start_update_firmware();
	osDelay(5000);
    for(;;)
    {
    	if(netif_is_link_up(&gnetif) && sub_request_cb)
    	{
    		device->firmware_flag = true;
    		xSemaphoreTake(ota_mutex, portMAX_DELAY);
    		test_wireless_params();
    		close_damper();

    		HAL_TIM_Base_Stop(&htim1);                    //таймер для вычисления показаний датчиков
    		HAL_TIM_Base_Stop(&htim2);                    //таймер для работы ПИД регулятора тена
    		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);      //шим вентилятора
    		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);      //шим тена
    		HAL_TIM_IC_Stop(&htim3, TIM_CHANNEL_3);       //захват/сравнение счетчика скорости вентилятора 3 канала
    		HAL_TIM_IC_Stop(&htim3, TIM_CHANNEL_4);       //захват/сравнение счетчика скорости вентилятора 4 канала
    		HAL_TIM_Base_Stop(&htim12);

    		erase_sectors();
    		tcp_setup();
    	}
    	osDelay(1000);
    }
}

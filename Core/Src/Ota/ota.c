#include "ota.h"
#include "mqtt_message.h"
#include <string.h>
#include <cmsis_os.h>
#include "httpserver.h"
#include <tcp.h>
#include "storage.h"
#include "tim.h"
#include "damper.h"
#include "FLASH_SECTOR_F4.h"

extern struct netif gnetif;
extern int content_length;
extern FLASH_ProcessTypeDef pFlash;

xSemaphoreHandle ota_mutex;

void start_update_firmware()
{
    xSemaphoreGive(ota_mutex);
}
void start_update_firmware_isr()
{
    xSemaphoreGiveFromISR(ota_mutex, NULL);
}

void erase_sectors()
{
	Flash_Delete_Data(OTA_ADDR_FLASH);
	Flash_Delete_Data(OTA_ADDR1_FLASH);
	Flash_Delete_Data(OTA_ADDR2_FLASH);
	DEBUG_OTA("Erase sectors!\n");
}

void boot_jump()
{
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

int address_flash = OTA_ADDR_FLASH;
int flash_data(char* buf, int len)
{
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_5);

  HAL_StatusTypeDef ret;

  ret = HAL_FLASH_Unlock();
  if (ret != HAL_OK)
	  return ret;

  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP
                         | FLASH_FLAG_OPERR
                         | FLASH_FLAG_WRPERR
                         | FLASH_FLAG_PGAERR
                         | FLASH_FLAG_PGSERR);

  if (pFlash.ErrorCode != 0)
      return pFlash.ErrorCode;

  for (int i = 0; i < len; i++)
  {
    FLASH_WaitForLastOperation(HAL_MAX_DELAY);
    ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address_flash, buf[i]);
    if (ret != HAL_OK)
    {
    	printf("App Flash Write Error\n");
        break;
    }
    address_flash++;
  }

  FLASH_WaitForLastOperation(500);

  ret = HAL_FLASH_Lock();
  if(ret != HAL_OK)
	  return ret;

  return ret;
}

static err_t tcp_send_packet(struct tcp_pcb *tpcb)
{
	err_t ret_err;
    char *string = malloc(256);
    sprintf(string, "GET /updatefirmware/cityair350/stm32/%s/%s/%s HTTP/1.1\r\nHost: %s\r\n\r\n ", SUBTYPE, XTAL_FREQ, wireless_params->vakio.device_id, wireless_params->domain);
    /* push to buffer */
    ret_err = tcp_write(tpcb, string, strlen(string), TCP_WRITE_FLAG_COPY);
    if (ret_err != ERR_OK)
    {
    	DEBUG_OTA("ERROR: Code: %d (tcp_send_packet :: tcp_write)\n", ret_err);
        return 1;
    }
    /* now send */
    ret_err = tcp_output(tpcb);
    if (ret_err != ERR_OK)
    {
    	DEBUG_OTA("ERROR: Code: %d (tcp_send_packet :: tcp_output)\n", ret_err);
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
    device->ota_len = ota_length;
    write_device_params();
    publish_firmware_state("done");
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
  DEBUG_OTA("Contents of pbuf %s\n", (char *)p->payload);
  if (ota_length == 0)
  {
	  ota_length = http_get_content_length((char *)p->payload);
	  DEBUG_OTA("content_length = %lu\n", ota_length);

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
	ota_length = 0;
}

void test_wireless_params()
{
	strcpy(wireless_params->server_ip, "51.250.111.175\0");
	strcpy(wireless_params->domain, "service.vakio.ru\0");
}

void OtaTask(void const * argument)
{
	ota_mutex = xSemaphoreCreateBinary();
    for(;;)
    {
        xSemaphoreTake(ota_mutex, portMAX_DELAY);
        DEBUG_OTA("Take mute OTA\n");
    	if(netif_is_link_up(&gnetif))
    	{
    		publish_firmware_state("start");
//    		test_wireless_params();
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
        else
        {
            DEBUG_OTA("LWIP isnt connected\n");
            vTaskDelay(5000);
            xSemaphoreGive(ota_mutex);
        }
    }
}

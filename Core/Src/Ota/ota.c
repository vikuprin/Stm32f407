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
#include "w25qxx.h"

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

void jumpToApp(uint32_t start_program_addr)
{
	HAL_DeInit();
	__set_MSP(*((volatile uint32_t *) (start_program_addr)));
	__DMB();
	SCB->VTOR = start_program_addr;
	__DSB();
    uint32_t JumpAddress = *((volatile uint32_t*) (start_program_addr + 4));
    void (*reset_handler) (void) = (void*) JumpAddress;
    reset_handler();
}

void erase_sectors()
{
	//////////////
	ota_length = 260000;
	//////////////
	uint16_t address_sector = OTA_EXT_SECTOR;
	uint8_t num_sector = ota_length / 4096 + 1;

	for (uint8_t i = 0; i < num_sector; i++)
	{
		W25qxx_EraseSector(address_sector);
		address_sector++;
	}
	DEBUG_OTA("Erase sectors!\n");
}

//uint32_t address_byte = OTA_EXT_BYTE;
//void ext_flash_ota(char* buf, int len)
//{
//	for (uint16_t i = 0; i < len; i++)
//	{
//		W25qxx_WriteByte(buf[i], address_byte);
//		address_byte++;
//	}
//}

// uint16_t address_sector = OTA_EXT_SECTOR;
// uint16_t offset = 0;
// uint16_t new_len[4];
// void ext_flash_ota(char* buf, uint16_t len)
// {
// 	new_len[0] = 256 - offset;
// 	W25qxx_WritePage(buf, address_sector, offset, new_len[0]);
// 	offset = 0;
// 	address_sector++;

// 	new_len[1] = len - new_len[0];
// 	if(new_len[1] > 256)
// 	{
// 		new_len[1] = 256;
// 		W25qxx_WritePage(buf[new_len[0]], address_sector, offset, new_len[1]);
// 		offset = 0;
// 		address_sector++;

// 		new_len[2] = len - new_len[0] - new_len[1];
// 		if(new_len[2] > 256)
// 		{
// 			new_len[2] = 256;
// 			W25qxx_WritePage(buf[new_len[0] + new_len[1]], address_sector, offset, new_len[2]);
// 			offset = 0;
// 			address_sector++;

// 			new_len[3] = len - new_len[0] - new_len[1] - new_len[2];
// 			W25qxx_WritePage(buf[new_len[0] + new_len[1] + new_len[2]], address_sector, offset, new_len[3]);
// 			offset = new_len[3];
// 		}
// 		else
// 		{
// 			W25qxx_WritePage(buf[new_len[0] + new_len[1]], address_sector, offset, new_len[2]);
// 			offset = new_len[2];
// 		}
// 	}
// 	else
// 	{
// 		W25qxx_WritePage(buf[new_len[0]], address_sector, offset, new_len[1]);
// 		offset = new_len[1];
// 	}
// }

uint16_t address_sector = OTA_EXT_SECTOR;
uint16_t offset = 0;
void ext_flash_ota(char* buf, uint16_t len)
{
  uint16_t count_len = 4096 - offset;
  if(count_len >= len)
  {
    W25qxx_WriteSector(buf, address_sector, offset, len);
    offset = offset + len;
  }
  else
  {
    W25qxx_WriteSector(buf, address_sector, offset, count_len);
    address_sector++;

    W25qxx_WriteSector(buf[count_len], address_sector, 0, len - count_len);
    offset = len - count_len;
  }
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
    device_ota_len = ota_length;
    write_ota_byte();
//    publish_firmware_state("done");
    jumpToApp(BOOT_ADDR_FLASH);
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
//	  erase_sectors();

	  char* temp_buf = NULL;
	  temp_buf = strstr((char *)p->payload, "Vary: Origin");
	  temp_buf = strstr(temp_buf, "\r\n\r\n");
	  temp_buf += 4;

	  u16_t tcp_len;
	  tcp_len = p->tot_len;
	  tcp_len -= (temp_buf - (char *)p->payload);
	  DEBUG_OTA("length ============================= %lu\n", tcp_len);
	  ext_flash_ota(temp_buf, tcp_len);
  }
  else
  {
	  DEBUG_OTA("length ============================= %lu\n", p->tot_len);
	  ext_flash_ota((char *)p->payload, p->tot_len);
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
	xSemaphoreGive(ota_mutex);
    for(;;)
    {
    	vTaskDelay(15000);
        xSemaphoreTake(ota_mutex, portMAX_DELAY);
        DEBUG_OTA("Take mute OTA\n");
    	if(netif_is_link_up(&gnetif))
    	{
    		erase_sectors();
//    		publish_firmware_state("start");
    		test_wireless_params();//////////////////
    		close_damper();

    		HAL_TIM_Base_Stop(&htim1);                    //таймер для вычисления показаний датчиков
    		HAL_TIM_Base_Stop(&htim2);                    //таймер для работы ПИД регулятора тена
    		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);      //шим вентилятора
    		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_2);      //шим тена
    		HAL_TIM_IC_Stop(&htim3, TIM_CHANNEL_3);       //захват/сравнение счетчика скорости вентилятора 3 канала
    		HAL_TIM_IC_Stop(&htim3, TIM_CHANNEL_4);       //захват/сравнение счетчика скорости вентилятора 4 канала
    		HAL_TIM_Base_Stop(&htim12);

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

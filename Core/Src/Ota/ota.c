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

#include "lwip/opt.h"
#include "lwip/tcpip.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip/sockets.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"


#define RECV_BUF_SIZE			256

extern struct netif gnetif;
extern int content_length;
extern FLASH_ProcessTypeDef pFlash;

static struct netconn *tcp_client_handle = NULL;
static uint8_t recvBuf[RECV_BUF_SIZE];

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
	uint16_t address_sector = OTA_EXT_SECTOR;
	uint8_t num_sector = ota_length / 4096 + 1;

	for (uint8_t i = 0; i < num_sector; i++)
	{
		W25qxx_EraseSector(address_sector);
		address_sector++;
	}
	DEBUG_OTA("Erase sectors!\n");
}

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

static err_t tcp_client_send_data()
{
	err_t ret_err;
	char *string = malloc(256);
	sprintf(string, "GET /updatefirmware/cityair350/stm32/%s/%s/%s HTTP/1.1\r\nHost: %s\r\n\r\n ", SUBTYPE, XTAL_FREQ, wireless_params->vakio.device_id, wireless_params->domain);

	struct netconn *conn = tcp_client_handle;
	ret_err = netconn_write(conn, string, strlen(string), NETCONN_COPY);
	if (ret_err != ERR_OK)
		printf("tcpecho: netconn_write: error \"%s\"\n", lwip_strerr(ret));
	return ret_err;
}

void tcp_setup(void)
{
	struct netconn *conn;
	struct netbuf *buf;
	ip_addr_t DestIPaddr;
	err_t err;

	/* Bind to TCP Client port with default IP address */
	conn = netconn_new(NETCONN_TCP);
	if(conn == NULL)
	{
		printf("[TCP Client]netconn_new: invalid conn\r\n");
	}

//	netconn_bind(conn, IP4_ADDR_ANY, 80);////??????????????????????


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
	IP4_ADDR(&DestIPaddr, str[0], str[1], str[2], str[3]);
	err = netconn_connect(conn, &DestIPaddr, 5000);
	if(err != ERR_OK)
	{
		printf("[TCP Client]netconn_connect is fail!!!\r\n");
	}

	tcp_client_handle = conn;
	printf("[TCP Client] Connect server is successful!!!\r\n");

	err = tcp_client_send_data();

	do
	{
		err = netconn_recv(conn, &buf);

		if (err == ERR_OK) {
			uint8_t len = (buf->p->len < RECV_BUF_SIZE) ? buf->p->len : RECV_BUF_SIZE;
			MEMCPY(recvBuf, buf->p->payload, len);
			recvBuf[len] = '\0';
			printf("[Recv Data]%s\r\n", (char *)recvBuf);
		}

		if (buf != NULL) {
			netbuf_delete(buf);
		}
//		ota_length = http_get_content_length((char *)buf->payload);
//	    DEBUG_OTA("content_length = %lu\n", ota_length);
//	    erase_sectors();
//
//        char* temp_buf = NULL;
//        if (err == ERR_OK)
//        {
//          netbuf_delete(inbuf);
//          err = netconn_recv(conn, &inbuf);
//          netbuf_data(inbuf, (void**) &buf, &buflen);
//          memcpy(temp_str, buf, buflen);
//
//          temp_buf = strstr(temp_str, "Content-Type: ");
//          temp_buf = strstr(temp_buf, "\r\n\r\n");
//          temp_buf += 4;
//          buflen -= (temp_buf - temp_str);
//
//          for (int i = 0; i < buflen; i++)
//          {
//        	  DEBUG_SERVER("%02X\n", temp_buf[i]);
//          }
//          ext_flash_ota(temp_buf, buflen);
//        }
//
//        while(err == ERR_OK)
//        {
//          netbuf_delete(inbuf);
//          recv_err = netconn_recv(conn, &inbuf);
//          netbuf_data(inbuf, (void**) &buf, &buflen);
//          memcpy(temp_str, buf, buflen);
//
//          ext_flash_ota(temp_str, buflen);
//          if (buflen < 536)
//             break;
//        }
//
//        device_ota_len = ota_length;
//        write_ota_byte();
//        jumpToApp(BOOT_ADDR_FLASH);

	} while (1);
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

//    		publish_firmware_state("start");
    		test_wireless_params();//////////////////
    		close_damper();
    		ota_length = 0;

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

#include <iocc2530.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "OSAL.h"
#include "OSAL_Nv.h"

#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"
#include "ZDSecMgr.h"
#include "MT_UART.h"

#if !defined( WIN32 )
  #include "OnBoard.h"
#endif

/* HAL */
#include "hal_adc.h"
#include "hal_mcu.h"
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"
#include "hal_drivers.h"

#include "printf.h"
#include "jsonparse.h"
#include "guu_common_def.h"
#include "guu_serial.h"
#include "guu_base.h"

void guu_usart2(uint8*d, uint8 len)
{
  HalUARTWrite( SER_PORT2,  d, len);
}

uint8 guu_usart(uint8*d, uint8 len)//uint8的类型表示了，他一次最大发送255个字节
{
  //if( _NIB.nwkDevAddress != 0x0000 )
		if(d!=NULL)
		{
    	return HalUARTWrite(SER_PORT, d, len );
		}else
		{
			return ZFailure;
		}
}


void guu_serial_callback(uint8 port, uint8 event)
{
	//这儿只有JSON
  uint8 serial_buff[64];
  uint8 readBytes = 0;

  readBytes = HalUARTRead(SER_PORT, serial_buff, 64);
  if (readBytes > 0)
  {
#if defined( COORD_DELAY ) 
		//协调器处理json指令 
		//_printf("readBytes: %d\r\n",readBytes);
		process_json(serial_buff,readBytes);
#else
		//路由部分
		guu_wire_send(guu_center_addr,readBytes, serial_buff);
#endif
	}
	//路由收到json数据转发至 guu_center_addr
	//guu_finally_report(FROM_WIRE,readBytes,serial_buff);

  return;
}

static void guu_serial_cb2(uint8 port, uint8 event)
{
	
}

void guu_serial_init(void )
{
  halUARTCfg_t uartConfig;

  uartConfig.configured           = TRUE;                                                   // 2x30 don't care - see uart driver.
  uartConfig.baudRate             = MT_UART_DEFAULT_BAUDRATE;                       //38400
  uartConfig.flowControl          = FALSE;
  uartConfig.flowControlThreshold = MT_UART_DEFAULT_THRESHOLD;        // 64.
  uartConfig.rx.maxBufSize        = MT_UART_DEFAULT_MAX_RX_BUFF;                  // 128
  uartConfig.tx.maxBufSize        = MT_UART_DEFAULT_MAX_TX_BUFF;                  // 128
  uartConfig.idleTimeout          = MT_UART_DEFAULT_IDLE_TIMEOUT;                      // 6
  uartConfig.intEnable            = TRUE;                                                 
  uartConfig.callBackFunc         = guu_serial_callback;                   // Call back function
  
  HalUARTOpen (SER_PORT, &uartConfig);
	
}

void guu_serial2_init()
{
	uint8 com2_speed;
	halUARTCfg_t uartConfig;

 	PERCFG |= BV(1); // uart1 alt 2 location
	guu_osal_nv_read(COM2_SPEED,0,sizeof(uint8),&com2_speed);
	if(com2_speed > 0x04)
	{
		com2_speed = HAL_UART_BR_38400;
	}
  uartConfig.configured           = TRUE;                                                   // 2x30 don't care - see uart driver.
  uartConfig.baudRate             = com2_speed;                       //38400
  uartConfig.flowControl          = FALSE;
  uartConfig.flowControlThreshold = MT_UART_DEFAULT_THRESHOLD;        // 64.
  uartConfig.rx.maxBufSize        = MT_UART_DEFAULT_MAX_RX_BUFF;                  // 128
  uartConfig.tx.maxBufSize        = MT_UART_DEFAULT_MAX_TX_BUFF;                  // 128
  uartConfig.idleTimeout          = MT_UART_DEFAULT_IDLE_TIMEOUT;                      // 6
  uartConfig.intEnable            = TRUE;                                                 
  uartConfig.callBackFunc         = guu_serial_cb2;                   // Call back function
  
  HalUARTOpen (SER_PORT2, &uartConfig);
	
}
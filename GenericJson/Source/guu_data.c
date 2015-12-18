/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"
#include "OSAL_Nv.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"

#include "MT_UART.h"
#include "GenericApp.h"
#include "DebugTrace.h"

#if !defined( WIN32 )
  #include "OnBoard.h"
#endif

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"

#include "guu_base.h"
#include "guu_common_def.h"
#include "guu_data.h"
#include "guu_serial.h"
#include "guu_io.h"

#include "printf.h"
/*
几个可能用到的结构体，从afIncomingMSGPacket_t 
引伸出来

typedef struct
{
  osal_event_hdr_t hdr;
  uint8 a;
  uint8 b;
} short_cmd_t;

typedef struct
{
  osal_event_hdr_t hdr;
  uint8 a;
  uint8 b;
  uint8 c;
  uint8 d;
} short_cmd_t2;

typedef struct
{
  osal_event_hdr_t hdr;
  uint16 endpoint;
  uint16 transID;
} long_cmd_t; 
通过 osal_send_msg 传递结构 体信号
在对应的_ProcessEvent中处理 结构体
每个函数,可能只有一个 参数,就是一个 MSGpt
然后相应的_ProcessEvent会自动 free_mem/osal_msg_deallocate  释放内存

*/
//运算Fcs值 从 长度值开始,到最后倒数第2个,不能把FCs也加进FCs运算了

void guu_data_cb(afIncomingMSGPacket_t *msg)//无线传入 
{	
#ifdef USE_DEBUG_LIGHT
	INIT_DEBUG_DC();
	GUU_TOOGLE_DEBUG_DC();
#endif

#if defined( GUU_USE_JSON ) 
#if defined( COORD_DELAY )
	//协调器 必须要吐出包的来源短地址
	// read until \n
	//_printf("{\"src_shortaddr\":\"%04x\",\"data\":\"%s\"}",
	//				msg->srcAddr.addr.shortAddr, msg->cmd.Data);
	guu_usart(msg->cmd.Data,msg->cmd.DataLength);//协调器面对的是openwrt,必须要原封不动的把接收到的数据吐出，故不用_printf
#else
	//路由 也许可以处理  来自无线的 json 指令,过程与 协调器的串口json进 数据 处理 一样
	_printf("%s\n",msg->cmd.Data); //Arduino 收到的数据必须以 \n为分隔，所以用_printf + \n
	//guu_usart(msg->cmd.Data,msg->cmd.DataLength);
	//插线板专用 
	//process_json(msg->cmd.Data,msg->cmd.DataLength); //这代表cc2530也会处理一次arduino的json指令，从无线上发来

#endif
#else
		//非JSON数据情况下,暂时不考虑
#endif

#ifdef USE_DEBUG_LIGHT
	INIT_DEBUG_DC();
	GUU_TOOGLE_DEBUG_DC();
#endif

}
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
���������õ��Ľṹ�壬��afIncomingMSGPacket_t 
�������

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
ͨ�� osal_send_msg ���ݽṹ ���ź�
�ڶ�Ӧ��_ProcessEvent�д��� �ṹ��
ÿ������,����ֻ��һ�� ����,����һ�� MSGpt
Ȼ����Ӧ��_ProcessEvent���Զ� free_mem/osal_msg_deallocate  �ͷ��ڴ�

*/
//����Fcsֵ �� ����ֵ��ʼ,���������2��,���ܰ�FCsҲ�ӽ�FCs������

void guu_data_cb(afIncomingMSGPacket_t *msg)//���ߴ��� 
{	
#ifdef USE_DEBUG_LIGHT
	INIT_DEBUG_DC();
	GUU_TOOGLE_DEBUG_DC();
#endif

#if defined( GUU_USE_JSON ) 
#if defined( COORD_DELAY )
	//Э���� ����Ҫ�³�������Դ�̵�ַ
	// read until \n
	//_printf("{\"src_shortaddr\":\"%04x\",\"data\":\"%s\"}",
	//				msg->srcAddr.addr.shortAddr, msg->cmd.Data);
	guu_usart(msg->cmd.Data,msg->cmd.DataLength);//Э������Ե���openwrt,����Ҫԭ�ⲻ���İѽ��յ��������³����ʲ���_printf
#else
	//·�� Ҳ����Դ���  �������ߵ� json ָ��,������ Э�����Ĵ���json�� ���� ���� һ��
	_printf("%s\n",msg->cmd.Data); //Arduino �յ������ݱ����� \nΪ�ָ���������_printf + \n
	//guu_usart(msg->cmd.Data,msg->cmd.DataLength);
	//���߰�ר�� 
	//process_json(msg->cmd.Data,msg->cmd.DataLength); //�����cc2530Ҳ�ᴦ��һ��arduino��jsonָ��������Ϸ���

#endif
#else
		//��JSON���������,��ʱ������
#endif

#ifdef USE_DEBUG_LIGHT
	INIT_DEBUG_DC();
	GUU_TOOGLE_DEBUG_DC();
#endif

}
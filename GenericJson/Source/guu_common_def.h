#ifndef GUU_COMMON_DEF_H
#define GUU_COMMON_DEF_H

#include "hal_board.h"

/*
C��macroֻ�Ǳ���ʱ�ڱ��������ڴ�ռ��У��൱���滻��ģ��ĸ��������������2���Ƴ�����

*/
/*
��ԭ��ȫ���ܰ��GenericApp ����ȡ�ģ�Ҳ��������0xc0���ڴ�����İ汾
ZTOOL_P1
MT_TASK
MT_SYS_FUNC
MT_ZDO_FUNC
HAL_LCD=FALSE
HAL_UART=TRUE
xNV_INIT
NV_RESTORE
xPOWER_SAVING
HAL_LED=FALSE
HAL_SPI=FALSE
HAL_UART_ISR=0
HAL_OAD_XNV_IS_INT=TRUE

*/
/*
NLME_GetShortAddr()
*/
/*
2015 01 11:������ZDApp.c�е�ZDApp_Init�У�������Ϊhal_key.c�ĸĶ���������DEV_HOLD�Ĳ���
Ȼ�����õ���DTK�İ��ӣ�P1_4,Ҳ����hal_led�е�led3,�������ʼ�������ھ�ʧЧ��
Ҳ����DTK��usart0����P1�ϵģ��ܹ��죬��������HAL_LED,����blink���ֺ��������е��õ�
����Flash֮��Ĳ���������hal_onboard_cfg.h�У����Կ��� HAL_LED��IO�ĳ�ʼ�����Ҵ���
��MT_UTIL�У�����һЩ�������ܣ�����GET DEvice INFOʱ����PaniD�ĳ���

��C ����������Ҫ�ģ��������ݽṹ�����ݽṹ��Ȼһ�еĸ���
����Lisp���⡣����

*/
/*
	openwrtֻҪ����	RETURN_��ͷ�ĺ�ֵ
*/
#define GUU_SYS_EVENT_MSG 0x4000 //�б���SYS_EVENT_MSG,���guu_osal_msg_send���������OSAL.c
#define GUU_EVENT_IN_NOUSE 0xF000 //��osal_start_msg_timerExʹ��

#define PWM_R_HEX 0x0f
#define PWM_G_HEX 0x10
#define PWM_B_HEX 0x11
#define PWM_A_HEX 0x12
#define PWM_RGB_HEX 0x0e
#define PWM_RGBA_HEX 0x0c 

#define TYPE_COORDI_HEX 0x43
#define TYPE_ROUTER_HEX 0x52
#define TYPE_ENDDEV_HEX 0x45
#define TYPE_UNKNOW_HEX 0x55

#define FROM_RS232 0 /// means from serial  of data
#define FROM_WIRE 1 // means from wireless ,of data

#define RESET_NETWORK P0_5

//NV Flash �ĵ�ַ app user ���� �� 0x0401 ��ʼ �� 0x0FFF �� 3070����ַ������
#define HIGHADDR 0x0479 // until 0x047c ,4�ռ䣬������/���ص�����洢
#define NV_ADC_CMD_ADDR 0x047d // 0x047d-0x0480

#define IOTYPE_ADDR 0x0401
#define SERV_ADDR 0x0411 // for echo_result dst addr
#define PWM_ADDR 0x0402
#define NV_SHORTADDR 0x0403

/*
#define PWM_G_ADDR 0x0403
#define PWM_B_ADDR 0x0404
#define PWM_A_ADDR 0x0405
*/
#define SAFE_LV_ADDR 0x0406
#define PI_ADDR 0x0407 //product information ��Ʒ�ͺŵ�Flash��ַ
#define COM2_DST_ADDR 0x0408
#define COM2_SPEED 0x0409
#define CHOSE_COM 0x0410


//------------IO PORT TYPES,�⼸��stringҲ���������λ��openwrt��sqlite���ݿ�
#define UNUSE 0
#define INPUT 1
#define OUTPUT 2
#define BUTTON 3
#define SWITCH 4
#define PWM 5

#define GROUPS 10
#define SER_DATA_LEN 120

//---------------------command flags definatios ,�����������0xfe
#define PWM_ALL 0x14
#define PWM_R 0x15
#define PWM_G 0x16
#define PWM_B 0x17
#define PWM_A 0x18
#define GET_RGBA PWM_ALL
#define RETURN_RGBA 0x19

#define G_ADC 0xdc
#define RETURN_ADC 0xdd
#define SET_ADC_EVENT 0xde 
#define GET_ADC_EVENT 0xdf
#define RETURN_ADC_EVENT 0xdb

#define SET_PANID 0x70
#define SET_SERV_ADDR 0x71
#define SET_COM2_DST_ADDR 0x20
#define SET_COM2_SPEED 0x21
#define SET_CHOSE_COM 0x22
#define SET_PI 0x23
#define SET_KEY_TYPE 0x25

#define EXEC_IO_CMD 0x90 // just like handle_keys ,invoke io command ,like switch command 
#define BIND_SWITCH_CMD 0x91 // [0x91-0x9e] 

#define GET_IO_CMD 0x31 //
#define RETURN_IO_CMD 0x32

#define GET_IO_STAT 0x33
#define RETURN_IO_STAT 0x34

#define GET_RGBA_VALUE 0x35
#define RETURN_RGBA_VALUE 0x36

#define GET_MAC  0x06// ר��Ϊ�˴������Ӷ���
#define RETURN_MAC 0x07//Ϊ��ȷ��openwrt��֮�����������豸��mac ��ַ

#define IO_OPERA 0xed
#define IO_BIT_OPERA1 0xe9 // operated io ports with bits group1 
#define IO_BIT_OPERA2 0xea // operated io ports with bits group2
#define TOGGLE_IO 0xec
#define RESTORE_PWM 0xf1
#define CLOSE_PWM 0xf0

#define PASSOUT 0xFD // ͸������
#define SEND_REMOTE_DATA 0xd8

#define GET_BASEINFO 0x03
#define RETURN_BASEINFO 0x02

#define BE_COORDI 0xc0
#define BE_ROUTER 0xe0
#define BE_ENDDEV 0xe1
#define GET_SAFE_LV 0x72
#define PING_CMD 0x50
#define WIRE_PING_CMD 0x51 // wireless ping
#define SET_CHANNEL 0xcc
#define SET_SADDR 0xcd
#define TYPE 0xbc

//--------------------SENSORS COMMAND FLAGS
#define SENSOR_DATA 0xab
#define RETURN_SENSOR_DATA 0xad

#define AM2301 0xa0
#define GET_PM25 0xa6
#define VOC 0xa8


#define VERSION_NUM0 1 
#define VERSION_NUM1 0
#define GET_VERSION 0x08

#define RETURN_BUTTON_EVENT 0x24

#define OSAL_NV_ITEM_INIT_ERROR 0x01
#define OSAL_NV_WRITE_ERROR 0x02
#define OSAL_NV_READ_ERROR 0x03
#define SEND_FAILED 0x04
#define COMP_FAILED 0x05

#define CPU_BITS 8 //������CPU��λ��

/* �ɱ�
0:p0_0 
1:p0_1
2:p0_4
3:p0_5// �ر�����Ϊ�ǹ��ܼ�
4:p0_6
5:p0_7
6:p1_0
7:p1_1
8:p1_2
9:p1_3
10:p1_4
11:p1_5
12:p2_0
*/
#if defined (USE_DEBUG_LIGHT)

#define DEBUG_DC_BV  BV(2)
#define DEBUG_DC_SBIT P2_2
#define DEBUG_DC_DDR	P2DIR

#ifdef LOW_TO_UP
	#define DEBUG_DC_POLARITY ACTIVE_LOW
#else
	#define DEBUG_DC_POLARITY ACTIVE_HIGH
#endif

#define DEBUG_DC_STATE()          (DEBUG_DC_POLARITY (DEBUG_DC_SBIT))

#define DEBUG_DD_BV  BV(1)
#define DEBUG_DD_SBIT P2_1
#define DEBUG_DD_DDR	P2DIR

#ifdef LOW_TO_UP
	#define DEBUG_DD_POLARITY ACTIVE_LOW
#else
	#define DEBUG_DD_POLARITY ACTIVE_HIGH
#endif

#define DEBUG_DD_STATE()          (DEBUG_DD_POLARITY (DEBUG_DD_SBIT))

#define INIT_DEBUG_DC() 							st( DEBUG_DC_DDR |= DEBUG_DC_BV;)
#define GUU_TURN_OFF_DEBUG_DC()       st( DEBUG_DC_SBIT = DEBUG_DC_POLARITY (0); )
#define GUU_TURN_ON_DEBUG_DC()        st( DEBUG_DC_SBIT = DEBUG_DC_POLARITY (1); )
#define GUU_TOOGLE_DEBUG_DC()				 	if (DEBUG_DC_SBIT ) { DEBUG_DC_SBIT  = 0; } else { DEBUG_DC_SBIT  = 1;} 

#define INIT_DEBUG_DD() 							st( DEBUG_DD_DDR |= DEBUG_DD_BV; )
#define GUU_TURN_OFF_DEBUG_DD()       st( DEBUG_DD_SBIT = DEBUG_DD_POLARITY (0); )
#define GUU_TURN_ON_DEBUG_DD()        st( DEBUG_DD_SBIT = DEBUG_DD_POLARITY (1); )
#define GUU_TOOGLE_DEBUG_DD()					if (DEBUG_DD_SBIT ) { DEBUG_DD_SBIT  = 0; } else { DEBUG_DD_SBIT  = 1;}


#endif



#endif

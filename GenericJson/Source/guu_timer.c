/*
Timer中断 和函数 做到独立很难，是因为有些变量会根据需求，要extern进来
这样就变的复杂了

*/
#include "ZComDef.h"
#include "hal_drivers.h"
#include "OSAL.h"
#include "OSAL_Tasks.h"
#include "OSAL_Nv.h"

//#include "OSAL_Custom.h"

#if defined ( MT_TASK )
  #include "MT.h"
  #include "MT_TASK.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "nwk.h"
#include "APS.h"
#include "ZDApp.h"

#include "osal_nv.h"
#include "NLMEDE.h"
#include "AF.h"
#include "OnBoard.h"
#include "nwk_util.h"
#include "ZDProfile.h"
#include "ZDObject.h"

#include "hal_adc.h"
#include "hal_key.h"

#include "guu_common_def.h"
#include "guu_serial.h"

#include "guu_base.h"
#include "guu_io.h"
#include "guu_timer.h"
#include "guu_event.h"
#include "guu_init.h"

uint16 guu_t4_counter;//timer 4 counter

void init_timer4_as_half_ms_counter(void)
{

    T4CTL &= ~BV(4);             // Stop timer 4 (if it was running)
    T4CTL |= BV(2);              // Clear timer 4
    T4CTL |= BV(3);             // enable Timer 4 overflow interrupts
    T4CTL |= 0x00;              // Timer 4 guu_mode = free-running mode,with overflowe interrupt
    T4CTL |= 0xc0; //  ZSTACK 默认是 [32Mhz Tick frequency]/64 这样，0.000512秒 一次，512us "11000000" 
    T4CTL |= BV(4); // START IT
    T4IE = 1;    
}

void init_timer3_as_second_counter(void)
{
    T3CTL &= ~BV(4);             // Stop timer 3 (if it was running)
    T3CTL = 0; // 清空所有设置,此时分频是1
    T3CTL |= BV(2);              // Clear timer 3
    T3CTL |= BV(3);             // enable Timer 3 overflow interrupts
    T3CTL |= 0x00;              // Timer 3 guu_mode = free-running mode,with overflowe interrupt
    T3CTL |= 224; //  Tick frequency/128, "11100000" 0.001024 秒
    T3IE = 1;
    T3CTL |= BV(4); // Start Timer
}

void guu_init_timer1(void)
{
  //time1 init
   T1CTL |=0x02;
   PERCFG |= 0x40; // move time1 to alt2 location
   T1CTL |=0x02; // module mode
   T1CC0L=0xff;
   T1CC0H=0xff;
   
   T1CCTL1 |= 0x1c;//比较模式，定时器T1，通道1 模式4
   T1CC1L = 0x00;//比较值 高电平时间
   T1CC1H = 0x00;
	 
   T1CCTL2 |= 0x1c;//比较模式，定时器T1，通道1 模式4
   T1CC2L = 0x00;//比较值 高电平时间
   T1CC2H = 0x00;

   T1CCTL3 |= 0x1c;//比较模式，定时器T1，通道1 模式4
   T1CC3L = 0x00;//比较值 高电平时间
   T1CC3H = 0x00;

   T1CCTL4 |= 0x1c;//比较模式，定时器T1，通道1 模式4
   T1CC4L = 0x00;//比较值 高电平时间
   T1CC4H = 0x00;

//   RGBA->R=0; RGBA->G=0; RGBA->B=0;RGBA->A=0;

	/*
  P1SEL |= 0x03; //p1_0,p1_1 用来当timer1的通道 
  P0SEL |= 0xc0; //p0_6,p0_7,用来当Timer1的通道  
	//此处不能在初始化时进行端口的配置 ,特别是P1_0,会影响到cc2530/1的USB初始化
	*/
	
}


#pragma vector=T3_VECTOR
__interrupt static void T3_IRQ(void)
{
  
}


#pragma vector=T4_VECTOR
__interrupt static void T4_IRQ(void)
{
	if(RESET_NETWORK == 0)// 从RESET_NETWOR 按下时才开始计数
	{
  	guu_t4_counter++;
  	if( guu_t4_counter >155 && guu_t4_counter < 160)//如果这个区间太多,会造成数次不停的发送baseinfo,造成巨大的流量
  	{
			if(RESET_NETWORK == 0)
			{
				if( _NIB.nwkDevAddress != INVALID_NODE_ADDR )
				{
          send_baseinfo(FROM_WIRE); //然后是闪一下DC 
					//HalLedSet (HAL_LED_1, HAL_LED_MODE_BLINK); 
					guu_t4_counter = 166;//马上 变成161,不再执行以后的send baseinfo
				}
			}
		}
		else if( guu_t4_counter > 13659 ) //6.993408秒
		{
			//check p0_5
			if(RESET_NETWORK == 0 /*&&  _NIB.nwkDevAddress != 0x0000*/)// 按下 清除网络的 按纽
			{
				osal_set_event(Guu_TaskID,GUU_RESET_TO_NEW_NETWORK);
			}
		}
		
		if(guu_t4_counter > 65534) 
		{
			guu_t4_counter = 0;
		}
		
	}else // RESET_NETWORK 没有被按下,就清0
	{
		guu_t4_counter = 0;
	}
}



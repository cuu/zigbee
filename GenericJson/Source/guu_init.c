#include "OSAL.h"
#include "OSAL_Nv.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"

#include "MT_APP.h"

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


#include "printf.h"
#include "guu_common_def.h"
#include "guu_io.h"
#include "guu_base.h"
#include "guu_serial.h"
#include "guu_timer.h"
#include "guu_event.h"
#include "guu_init.h"
#include "guu_json_cb.h"

byte Guu_TaskID;
uint8* guu_ieeeAddr;

#if defined(GUU_SELF_IO)
static void io_init(void)
{

  P0DIR |= 0x12; // p0_1 p0_4 
  P1DIR |= 0x14; // p1_3, p1_5
  
	GUU_TURN_OFF_IO(0,1);
	GUU_TURN_OFF_IO(0,4);
	GUU_TURN_OFF_IO(1,3);
	GUU_TURN_OFF_IO(1,5);

  P0DIR |= BV(6);
	P0DIR |= BV(7);
	P1DIR |= BV(0);
	//P1DIR |= BV(1);

	GUU_TURN_OFF_IO(1,0);
	//GUU_TURN_OFF_IO(1,1);
	GUU_TURN_OFF_IO(0,6);
	GUU_TURN_OFF_IO(0,7);

/*
	如果全部端口变成input,开电时就会有电流出现
	
*/
	P0DIR |= BV(5);
	P0_5 = 1;
	P0DIR &= ~BV(5);
	
#ifdef USE_DEBUG_LIGHT
	INIT_DEBUG_DC();
	GUU_TURN_OFF_DEBUG_DC();

	INIT_DEBUG_DD();
	GUU_TURN_OFF_DEBUG_DD();
#endif

}
#endif

void guu_init(uint8 task_id)
{
//############################################# 变量先初始化
  Guu_TaskID = task_id;
	guu_ieeeAddr = NLME_GetExtAddr();
	
	guu_center_addr = 0x0000;
//############################################# 函数


#if defined(GUU_SELF_IO)
  io_init();
#endif

#ifdef GUU_USE_JSON
#if defined( COORD_DELAY)
	osal_start_timerEx( Guu_TaskID, GUU_INIT_SER1,9000);// DELAY FOR openwrt init console data attacks
#else
	guu_serial_init();
#endif
#endif
	guu_serial2_init();

//	init_timer3_as_second_counter();
	init_timer4_as_half_ms_counter();
	
	init_printf(NULL,putc);
	init_guu_json_cb();

#if defined( COORD_DELAY )
	osal_start_timerEx( Guu_TaskID, GUU_SHOW_MAC,16000);
#else	
	osal_start_timerEx( Guu_TaskID, GUU_SHOW_MAC,5000);//4秒之后 发射出自身的Mac 地址给底下的单片机
#endif
}
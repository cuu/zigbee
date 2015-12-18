/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"

#include "MT_UART.h"
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

#include "guu_serial.h"
#include "guu_base.h"
#include "guu_timer.h"

#include "guu_data.h"
#include "guu_io.h"
#include "guu_init.h"

#include "guu_event.h"


//byte Guu_TaskID; 

// 我的数据结构是仿 mtOSALSerialData_t ,最简单的结构体在整个zstack架构中

extern uint32 _zgDefaultChannelList;
extern uint16 _zgConfigPANID;

#if defined (USE_DEBUG_LIGHT)
static void guu_check_state(void)//就是点灯,状态一正常就点灯
{
	if(_NIB.nwkDevAddress != INVALID_NODE_ADDR )
	{
		INIT_DEBUG_DC();
		GUU_TURN_ON_DEBUG_DC(); GUU_TURN_ON_DEBUG_DC();
		
		osal_stop_timerEx(Guu_TaskID, GUU_CHECK_STATE);
	}else
	{
   	if ( !osal_get_timeoutEx( Guu_TaskID, GUU_CHECK_STATE) )//重启TimerEx 继续check_state
    {
			osal_start_timerEx( Guu_TaskID, GUU_CHECK_STATE,1000);
    }		
	}
}

#endif

uint16 Guu_ProcessEvent( uint8 task_id, uint16 events )
{
	//事件从某个Taskid上发过来之后，不能再发送到相同的TaskID的任务
	// newlisp bits 某值 
	// 能看到 是否 会 ＆ 成功
	// 1000 和100 ,只要多一位0，就不会& 有结果
	// 很多event值相近的话，全是用case 来判断，而非 & 号
  /* Could be multiple events, so switch won't work */
	afIncomingMSGPacket_t *msg_ptr;
  if ( events & SYS_EVENT_MSG )// 这儿是必须要有数据结构体传入的
  {
    msg_ptr = (afIncomingMSGPacket_t *)osal_msg_receive(task_id);
    while (msg_ptr != NULL)
    {
			switch ( msg_ptr->hdr.event )
			{
				default:
				{
					osal_msg_deallocate((uint8*)msg_ptr);//保证一般操作是释放内存的
				}
				break;
			}

			//如果仍有信息存余 ，接着干
			msg_ptr = (afIncomingMSGPacket_t *)osal_msg_receive(task_id);
		}
			return (events ^ SYS_EVENT_MSG);
	}
  //以下是空头文件。。。
  if (events & ZDO_STATE_CHANGE)
  {
    //HalLedSet (HAL_LED_1, HAL_LED_MODE_TOGGLE); 
#if defined(ZG_BUILD_COORDINATOR_TYPE)
    if(_NIB.nwkDevAddress !=INVALID_NODE_ADDR)
    {
      if(_zgConfigPANID != _NIB.nwkPanId && _NIB.nwkPanId != INVALID_PAN_ID)
      {
#if defined ( NV_RESTORE )
        if ( !osal_get_timeoutEx( ZDAppTaskID, ZDO_NWK_UPDATE_NV ) )
        {
          // Trigger to save info into NV
          ZDApp_NVUpdate();
        }
#endif
        //write to ZCD_NV_PANID
        _zgConfigPANID  = _NIB.nwkPanId;
        if(guu_osal_nv_write(ZCD_NV_PANID,0,sizeof(_zgConfigPANID),(uint8*)&_zgConfigPANID) ==ZFailure)
        {
          //HalLedSet (HAL_LED_1, HAL_LED_MODE_FLASH); 
        }
      }
    }
#else
    
#endif
 
#ifdef USE_DEBUG_LIGHT
   	if ( !osal_get_timeoutEx( Guu_TaskID, GUU_CHECK_STATE) )
    {
			osal_start_timerEx( Guu_TaskID, GUU_CHECK_STATE,1000);
    }
#endif
   
    return (events ^ ZDO_STATE_CHANGE);   
  }

#ifdef USE_DEBUG_LIGHT
	if (events & GUU_CHECK_STATE)
  {	
			guu_check_state();
		return (events ^ GUU_CHECK_STATE);
	}
	
#endif

  if(events & GUU_RESET_TO_NEW_NETWORK)
  {
		guu_set_panid(0xffff);
    if(zgWriteStartupOptions (ZG_STARTUP_SET, ZCD_STARTOPT_DEFAULT_NETWORK_STATE) ==ZSuccess)
		{
     	SystemReset();
		}else
		{
			SystemResetSoft();
		}
		return (events^ GUU_RESET_TO_NEW_NETWORK);
  }
  
	if(events & GUU_SHOW_MAC)
	{
		_printf("{\"mac\":\"%02x%02x%02x%02x%02x%02x%02x%02x\"}\n",
			guu_ieeeAddr[0],guu_ieeeAddr[1],guu_ieeeAddr[2],guu_ieeeAddr[3],
			guu_ieeeAddr[4],guu_ieeeAddr[5],guu_ieeeAddr[6],guu_ieeeAddr[7]);
	
		_printf("{\"sad\":\"%04x\"}\n",_NIB.nwkDevAddress);
		return (events^GUU_SHOW_MAC);
	}
	if(events & GUU_INIT_SER1)
	{
		guu_serial_init();
		return (events ^ GUU_INIT_SER1);
	}
  // Discard unknown events
  return 0;
}
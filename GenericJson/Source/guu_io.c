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
#include "hal_board.h"

#include "guu_base.h"
#include "guu_common_def.h"
#include "guu_data.h"
#include "guu_io.h"

void MCU_IO_SET_HIGH_PREP(uint8 port, uint8 pin)
{
		switch(port)
		{
			case 0:
			{
				P0 |= BV(pin);
			}break;
			case 1:
			{
				P1 |= BV(pin);
			}break;
			case 2:
			{
				P2 |= BV(pin);
			}break;
		}
}

void MCU_IO_SET_LOW_PREP(uint8 port, uint8 pin)
{
		switch(port)
		{
			case 0:
			{
				P0 &= ~BV(pin);
			}break;
			case 1:
			{
				P1 &= ~BV(pin);
			}break;
			case 2:
			{
				P2 &= ~BV(pin);
			}break;
		}
}

void MCU_IO_TGL_PREP(uint8 port, uint8 pin) 
{
//         st( P##port##_##pin## ^= 1; )
}

uint8 MCU_IO_GET_PREP(uint8 port, uint8 pin)
{
		switch(port)
		{
			case 0:
			{
				return (P0 & BV(pin));
			}break;
			case 1:
			{
				return (P1 & BV(pin));
			}break;
			case 2:
			{
				return (P2 & BV(pin));
			}break;
			default:break;
		}
	return 0;
}


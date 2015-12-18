#ifndef GUU_IO_H
#define GUU_IO_H

#include "hal_board_cfg.h"
#include "hal_led.h"

void MCU_IO_SET_HIGH_PREP(uint8 port, uint8 pin);
void MCU_IO_SET_LOW_PREP(uint8 port, uint8 pin);
void MCU_IO_TGL_PREP(uint8 port, uint8 pin);
uint8 MCU_IO_GET_PREP(uint8 port, uint8 pin);

#if defined(LOW_TO_UP) //下拉电平是开灯
	#define GUU_TURN_ON_IO(port,pin)		MCU_IO_SET_LOW_PREP(port, pin) 
	#define GUU_TURN_OFF_IO(port,pin)   MCU_IO_SET_HIGH_PREP(port, pin) 
#else
	#define GUU_TURN_ON_IO(port,pin)		MCU_IO_SET_HIGH_PREP(port, pin)
	#define GUU_TURN_OFF_IO(port,pin)   MCU_IO_SET_LOW_PREP(port, pin) 
#endif


#endif
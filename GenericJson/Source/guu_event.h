#ifndef GUU_EVENT_H
#define GUU_EVENT_H

#define GUU_RESET_TO_NEW_NETWORK 0x0100
#define GUU_CHECK_STATE 0x0200
#define GUU_RESTORE_USART0 0x0400
#define GUU_SHOW_MAC 0x0800
#define GUU_INIT_SER1 0x1000

#define GUU_MT_EVENT  0x01
#define	GUU_ADC_EVENT 0x02

uint16 Guu_ProcessEvent( uint8 task_id, uint16 events );
#endif
#ifndef GUU_BASE_H
#define GUU_BASE_H

extern uint16 guu_center_addr;//配合echo_data,可以由串口或是无线函数改变这个值

uint8 guu_osal_nv_item_init(uint16 id, uint16 len, void *buf);
uint8 guu_osal_nv_write( uint16 id, uint16 ndx, uint16 len, void *buf );
uint8 guu_osal_nv_read( uint16 id, uint16 ndx, uint16 len, void *buf );
void guu_osal_mem_free(void *ptr);

uint8 guu_wire_send ( uint16 , uint8, uint8 * );
uint8 guu_finally_report(uint8 flag, uint8 len ,uint8*data);

void guu_set_panid( uint16 u16NewPanid);

void putc ( void* p, char c);

void send_baseinfo(uint8 from);

uint8*alloc_and_cpy(uint8*source, uint8 start, uint8 length);
void process_json(uint8*json_string, uint8 length);

#endif
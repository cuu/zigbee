#ifndef GUU_DATA_H
#define GUU_DATA_H

/*
typedef struct
{
  uint8  event;
  uint8  status;
} osal_event_hdr_t;
*/



void guu_data_cb(afIncomingMSGPacket_t *);//处理命令

void guu_parse_data(uint8*pbuf, uint8 len);//这个是做最简单的最终命令解析
//如 ed 02 ,ed 03之类的
//d8这个功能,去掉了多层转发的功能,只限一层
//为了节约有限memory


#endif
#ifndef GUU_DATA_H
#define GUU_DATA_H

/*
typedef struct
{
  uint8  event;
  uint8  status;
} osal_event_hdr_t;
*/



void guu_data_cb(afIncomingMSGPacket_t *);//��������

void guu_parse_data(uint8*pbuf, uint8 len);//���������򵥵������������
//�� ed 02 ,ed 03֮���
//d8�������,ȥ���˶��ת���Ĺ���,ֻ��һ��
//Ϊ�˽�Լ����memory


#endif
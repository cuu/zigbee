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
#include "mac_radio.h"
#include "mac_low_level.h"

#include "hal_adc.h"
#include "hal_key.h"

#include "printf.h"
#include "jsmn.h"

#include "guu_common_def.h"
#include "guu_serial.h"

#include "guu_base.h"
#include "guu_json_cb.h"
#include "guu_init.h"

#include "GenericApp.h"

extern endPointDesc_t GenericApp_epDesc;
extern byte GenericApp_TransID;

uint16 guu_center_addr;//���echo_data,�����ɴ��ڻ������ߺ����ı����ֵ
//###################################################################################
/// �й���osal_nv�ĺ���������Ҫ�á�ZSuccesss�͡�ZFailure���жϣ���������if����! �����ķ���ʡ��
uint8 guu_osal_nv_item_init(uint16 id, uint16 len, void *buf)
{
  //TI�����֪�� Ϊ��,Ҫinit 2�β���success
  osal_nv_item_init( id, len, buf );
  return osal_nv_item_init( id, len, buf );
}


uint8  guu_osal_nv_read( uint16 id, uint16 ndx, uint16 len, void *buf )
{
   if(guu_osal_nv_item_init(id, len,NULL) ==ZSuccess)
   {
     return osal_nv_read( id, ndx, len, buf );
   }else
   {
     return ZFailure;
   }
}

uint8 guu_osal_nv_write( uint16 id, uint16 ndx, uint16 len, void *buf )
{
   if(guu_osal_nv_item_init(id, len,NULL) == ZSuccess)
   {
     return osal_nv_write( id, ndx, len, buf );
   }else
   {
     return ZFailure;
   }  
}

void guu_osal_mem_free(void *ptr)//���˷�ֹfree NULL��ָ�����ϵͳ������������
{
	if(ptr !=NULL){
		osal_mem_free(ptr);
	}
}

void guu_set_panid( uint16 u16NewPanid)
{
  _NIB.nwkPanId = u16NewPanid;
  macRadioSetPanID ( _NIB.nwkPanId);
  ZMacSetReq( ZMacPanId, (byte *)& _NIB.nwkPanId);
	NLME_UpdateNV(NWK_NV_NIB_ENABLE);  
	osal_nv_write(ZCD_NV_PANID, 0, osal_nv_item_len( ZCD_NV_PANID ), &u16NewPanid);

}

uint8 guu_wire_send ( uint16 destination, uint8 len, uint8 *pData )
{
  afAddrType_t dstAddr;
  if( _NIB.nwkDevAddress == INVALID_NODE_ADDR ) { return ZFailure; } //������û�еõ���Ч�Ķ̵�ַ,��ʾû������,������
  if( destination == INVALID_NODE_ADDR) { return ZFailure; } //Ŀ���ַ�ǲ��Ϸ��̵�ַ,������
  if( destination == _NIB.nwkDevAddress) { return ZFailure; } //��ַ�뱾����ͬ,������
  //if( _NIB.nwkDevAddress == guu_center_addr) { return ZFailure; } //������������ ��ַһ��
	// ���������ĵ�ַһ���ǿ��Եģ�Э����ҲҪ�������ݵ�ѽ������ֻ����guu_wire_send�������棬������
	//sAddrExtCpy(dstAddr.addr.extAddr,guu_ieeeAddr);
	
  if (destination == 0xFFFF)
  {
    dstAddr.addrMode = (afAddrMode_t)AddrBroadcast;
    dstAddr.addr.shortAddr = NWK_BROADCAST_SHORTADDR; 
    dstAddr.endPoint = AF_BROADCAST_ENDPOINT;
    return AF_DataRequest( &dstAddr, 
                       &GenericApp_epDesc,// =_=
                       GENERICAPP_CLUSTERID,//=_=
                       len,
                       pData,
                       &GenericApp_TransID,//=_=
                       //AF_DISCV_ROUTE,
                       0,
                       AF_DEFAULT_RADIUS );
  }
  else if (destination == 0xFFFC)
  {
    dstAddr.addrMode  = (afAddrMode_t)AddrBroadcast;;
    dstAddr.addr.shortAddr = 0xFFFC; 
    dstAddr.endPoint = GENERICAPP_ENDPOINT;
    return AF_DataRequest( &dstAddr, 
                       &GenericApp_epDesc,
                       GENERICAPP_CLUSTERID,
                       len,
                       pData,
                       &GenericApp_TransID,
                       AF_DISCV_ROUTE | AF_SKIP_ROUTING,
                       //1 
                       AF_DEFAULT_RADIUS );
  } 
  else // p2p   
  {
   
    dstAddr.addrMode = afAddr16Bit;
    dstAddr.addr.shortAddr = destination; 
    dstAddr.endPoint = GenericApp_epDesc.endPoint;
    return AF_DataRequest( &dstAddr, 
                       &GenericApp_epDesc,
                       GENERICAPP_CLUSTERID,
                       len,
                       pData,
                       &GenericApp_TransID,
                       AF_DISCV_ROUTE,
                       AF_DEFAULT_RADIUS );
 } 
} // end SendData2

uint8 guu_finally_report(uint8 flag, uint8 len ,uint8*data)
{
	//�����������Flag��־,��ѡ��İ�dataͨ�����߻������߷���
	if(flag == FROM_RS232)
	{
		return guu_usart(data,len);
	}else // FROM_WIRE
	{
		if(guu_center_addr == _NIB.nwkDevAddress)//�����ͬһ�豸��״̬"����",����ֱ�Ӵ����³�,����Э������״̬����
		{
			return guu_usart(data,len);
		}else
		{
			return guu_wire_send(guu_center_addr,len,data);
		}
	}
}

void putc ( void* p, char c)
{
	guu_usart(&c, 1);
}


void send_baseinfo(uint8 from)//����mac֮�⻹����io���ͺͲ�Ʒ��� 
{
	
}


uint8*alloc_and_cpy(uint8*source, uint8 start, uint8 length)
{
	uint8*cache=NULL;
	cache = osal_mem_alloc( (length+1)*sizeof(uint8));
	strncpy(cache,source+start,length);
	cache[length]=0;
	return cache;
}

void process_json(uint8*json_string, uint8 length)
{
		uint8 i,j;
		uint8 e;
		jsmn_parser p;
		jsmntok_t tok[10];
		jsmnerr_t r;
		jsmn_init(&p);
		r = jsmn_parse(&p, json_string, length,tok, 10);
		if( r < 0 ) 
		{
#if defined (COORD_DELAY) //Э���������������Ϣ,�Է���rt5350������������

#else
			_printf("jsmn_parse error %d\r\n",r);
#endif
			r =0;
		}
		e=0;
		for(j=1;j<r;j++)
		{
			for(i=0;i< GUU_JSON_CALLBACK; i++)
			{
				if (jsoneq(json_string, &tok[j], json_cb[i].func_name ) == 0) 
				{
					uint8*cache=alloc_and_cpy(json_string,tok[j+1].start, tok[j+1].end-tok[j+1].start);
					json_cb[i].func(tok[j+1].end-tok[j+1].start, cache);
					guu_osal_mem_free(cache);
					j++;//����
					e++;
				}
			}
		}

		if(e == 0) //��ʾ���ݲ�û�д����� json cb���б��У���ô�㲥֮
		{
#if defined( COORD_DELAY )
			guu_wire_send(0xffff,length, json_string);
#endif
		}
}
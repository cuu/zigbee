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
#include "ZGlobals.h"

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
#include "jsonparse.h"
#include "guu_base.h"
#include "guu_io.h"
#include "guu_json_cb.h"
#include "guu_init.h"
#include "jsmn.h"

guu_json_cb json_cb[GUU_JSON_CALLBACK];

static void print_macaddress(unsigned char argc,char*argv)
{
	_printf("{\"Mac\":\"%08x\"}\n",guu_ieeeAddr);
}

static void print_shortaddress(unsigned char argc,char*argv)
{
	_printf("{\"shortaddress\":\"%04x\"}\n",_NIB.nwkDevAddress);
}

static void print_devicetype(unsigned char argc,char*argv)
{
	_printf("{\"device_type\":%x}\n",zgDeviceLogicalType);
}

static void print_toremote(unsigned char argc,char*argv)
{
	//{"re:{"短地址":"fuckyoudata"}} 原始数据
	// here decode json again
		uint16 addr;
		uint8 buf[5];
		uint8 i;
		char*ptr;
		jsmn_parser p;
		jsmntok_t tok[8];
		jsmnerr_t r;
		jsmn_init(&p);
		r = jsmn_parse(&p, argv,argc,tok, 8);
		if( r < 0 ) 
		{
			_printf("print_toremote jsmn_parse error %d\r\n",r);
			return;
		}
		
		//_printf("%s\r\n",argv);
		for(i=1;i<r;i++)
		{
			//只是把短地址从第一个token提取，然后break;掉
			strncpy(buf,argv+tok[i].start, tok[i].end-tok[i].start);
			buf[tok[i].end-tok[i].start] = 0;
			//_printf("%d %s\r\n",tok[i].end-tok[i].start, buf);
		
			addr = strtol(buf,&ptr,16);
			//_printf("addr: %04x\r\n",addr);
			i++;
			break;
		}
		i = r>2?2:r; //强制 i是2，这样的话等于提取了 短地址之后的token 为发送的数据
		guu_wire_send(addr,tok[i].end-tok[i].start,argv+tok[i].start);

		_printf("{\"send\":\"%04x\",\"stat\":\"ok\",\"length\":%d}\n",addr,tok[i].end-tok[i].start);
}	

static void zigbee_io(unsigned char argc,char*argv)
{
	//{"zio":{"p":0,"i":1,"o":0/1}}  "o"是操作，0是关，1是开
	// 2530上的json是以{为一个token 所以上层应用只分析到了zio
	//就把zio对应的值给到了下一层函数
	// 所以 zio 对应的就该 是一个object 
		uint8 buf[5];
		uint8 i;
		char*ptr;
		jsmn_parser p;
		jsmntok_t tok[7];
		jsmnerr_t r;
		jsmn_init(&p);
		r = jsmn_parse(&p, argv,argc,tok, 7);
		uint8 port;
		uint8 pin;
		uint8 opera;

		
		port = 0;
		pin = 0;
		opera = 0;
		if( r < 0 ) 
		{
			_printf("zigbee_io jsmn_parse error %d\r\n",r);
			return;
		}
		//_printf("%s\r\n",argv);// 这个函数得到的json字符串
		for(i=1;i<r-1;i++)
		{
			if (jsoneq(argv, &tok[i], "p" ) == 0) 
			{
				strncpy(buf,argv+tok[i+1].start, tok[i+1].end-tok[i+1].start);// next token will be the value
				buf[tok[i+1].end-tok[i+1].start] = 0;
				port = atoi(buf);
			}else if (jsoneq(argv, &tok[i], "i" ) == 0) 
			{
				strncpy(buf,argv+tok[i+1].start, tok[i+1].end-tok[i+1].start);// next token will be the value
				buf[tok[i+1].end-tok[i+1].start] = 0;
				pin = atoi(buf);
			}else if (jsoneq(argv, &tok[i], "o" ) == 0) 
			{
				strncpy(buf,argv+tok[i+1].start, tok[i+1].end-tok[i+1].start);// next token will be the value
				buf[tok[i+1].end-tok[i+1].start] = 0;
				opera = atoi(buf);
			}						
		}
		
		switch(opera)
		{
			case 0:
				GUU_TURN_OFF_IO(port,pin);
			break;
			case 1:
				GUU_TURN_ON_IO(port,pin);
			break;
			default:break;
		}

		_printf("{\"iostat\":[{\"P0\":%d},{\"P1\":%d},{\"P2\":%d}]}\n",P0 ,P1 ,P2);		
		
}

void init_guu_json_cb(void)
{
	uint8 i;

	strcpy(json_cb[0].func_name,"sa"); //显示短地址
	json_cb[0].func = print_shortaddress;

	strcpy(json_cb[1].func_name,"dv");/// 显示设备类型
	json_cb[1].func = print_devicetype;

	strcpy(json_cb[2].func_name,"re"); // send remote data
	json_cb[2].func = print_toremote;

	strcpy(json_cb[3].func_name,"zio");//zigbee io
	json_cb[3].func = zigbee_io;
}


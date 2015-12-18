#ifndef GUU_JSON_CB_H
#define GUU_JSON_CB_H
//Ò»¶Ñ json cb
#define GUU_JSON_CB_FUNC_NAME 4
#define GUU_JSON_CALLBACK 4 // number of the callbacks

typedef struct _guu_json_cb
{
	char func_name[GUU_JSON_CB_FUNC_NAME];
	void(*func) (unsigned char argc,char*argv);
}guu_json_cb;


extern guu_json_cb json_cb[GUU_JSON_CALLBACK];

void init_guu_json_cb(void);

#endif
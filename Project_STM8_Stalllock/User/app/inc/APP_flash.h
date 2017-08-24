#ifndef _APP_FLASH_H_
#define _APP_FLASH_H_

#define EQUIPMENTCHAR1	0x43	//设备项目字符1"U"
#define EQUIPMENTCHAR2	0x4C	//设备项目字符2"L"
#define SNSTARTADD	0x13E0	//SN数据的开始地址
#define SNLENG		5		//SN的数据长度
#define SNFLAGADD	0x13E7	//SN数据标志位的地址
#define SNFLAGDATA	0x79	//SN的标志位数据

#define SNHAVEADD	0X13E5

typedef struct 
{
	u8 SNdata[SNLENG];
	u8 SNflag;
}SN_STRUCT;

extern SN_STRUCT APP_SN_StrPar;		   


void APP_SN_Write(void);
void APP_SN_Init(void);



#endif
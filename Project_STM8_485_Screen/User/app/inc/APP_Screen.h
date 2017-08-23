#ifndef __APP_SCREEN_H
#define __APP_SCREEN_H

#include "main.h"
#include <string.h>

//--------STM8L151C8内部EEPROM存储空车位数的地址
#define  VEHICLE_EEPADDR 0x01010

#define Screen_Ack_Lenght 13
#define Screen_Tx_Lenght 140

#define ACK_PKG_CLASS 1<<3	//应答数据包类型 
#define Screen_Addr 0x05 	//引导屏地址

typedef struct{
	u8	Data[Screen_Ack_Lenght];		//数据
	u16	Status;							//状态
}Screen_Ack_Struct;						//Screen to DDA receipt 

typedef struct{
	u8	Tx_Data[Screen_Tx_Lenght];		//转发缓存
	u16	Status;						//状态
}Screen_Struct;						//to Screen

typedef struct
{
	 u16 vehicle_count;	//空余车位计数

}APP_LED_STRUCT;

extern APP_LED_STRUCT APP_LED;
extern Screen_Ack_Struct APP_Screen_Ack;
extern Screen_Struct	APP_Screen;

void APP_LED_ValueInit(void);
void APP_LedShow_init(void);
void APP_LED_ShowDataReady(void);
void APP_LEDSetShow(void);
void APP_Screen_dealSendCmdToSpider(void);
void APP_Deal_U_Package_From_GW(void);
void APP_Deal_Vehicle_By_Key(void);
// void APP_LEDSetShow(void);
#endif

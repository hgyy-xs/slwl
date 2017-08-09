#ifndef __APP_SCREEN_H
#define __APP_SCREEN_H

#include "main.h"
#include <string.h>

#define Screen_Ack_Lenght 12
#define Screen_Tx_Lenght 140

typedef struct{
	u8	Data[Screen_Ack_Lenght];		//数据
	u16	Status;							//状态
}Screen_Ack_Struct;						//Screen to DDA receipt 

typedef struct{
	u8	Tx_Data[Screen_Tx_Lenght];		//转发缓存
	u16	Status;						//状态
}Screen_Struct;						//to Screen

extern Screen_Ack_Struct APP_Screen_Ack;
extern Screen_Struct	APP_Screen;

void APP_Screen_dealSendCmdToSpider(void);

#endif

#ifndef __APP_CAMERA_H
#define __APP_CAMERA_H

#include "main.h"
#include <string.h>

#define Screen_Ack_Lenght 3
typedef struct{
	u8	Data[Screen_Ack_Lenght];		//数据
	u16	Status;							//状态
}Screen_Ack_Struct;						//Screen to DDA receipt 

extern Screen_Ack_Struct APP_Screen_Ack;
void APP_Screen_dealSendCmdToSpider(void);
#endif

#include "APP_Screen.h"

Screen_Ack_Struct APP_Screen_Ack;
Screen_Struct	APP_Screen;

void APP_Screen_dealSendCmdToSpider(void){
	APP_Screen_Ack.Data[0]=UART3.RxBuff[7];	//screen to dda 数据长度
	APP_Screen_Ack.Data[1]=0x80;	
	APP_Screen_Ack.Data[2]=0x00;
	PRO_spider_BuildCMDForPar(CMD_TYPE_TRFER,TRFER_CMD_SENDUPACK,APP_Screen_Ack.Data,Screen_Ack_Lenght);//生成U包命令
	
	HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,USER_USART3);	//mcu to spider
	APP_SPIDER.DealSendCmdToSpiderAddr = 1;//标志有命令需要发送到spider
}


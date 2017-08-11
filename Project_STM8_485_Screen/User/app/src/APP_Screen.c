#include "APP_Screen.h"

Screen_Ack_Struct APP_Screen_Ack;
Screen_Struct	APP_Screen;

void APP_Screen_dealSendCmdToSpider(void){
	u8 i=0;
	APP_Screen_Ack.Data[0]=0x0A;//服务质量
	APP_Screen_Ack.Data[1]=APP_Screen.Tx_Data[7];	//screen ack length 	
	APP_Screen_Ack.Data[2]=0x80;					// screen ack status
	APP_Screen_Ack.Data[3]=0x00;
	PRO_spider_BuildCMDForPar(CMD_TYPE_TRFER,TRFER_CMD_SENDUPACK,APP_Screen_Ack.Data,Screen_Ack_Lenght);//生成U包命令
	HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,USER_USART3);	//mcu to spider
	APP_SPIDER.DealSendCmdToSpiderAddr = 1;//标志有命令需要发送到spider
#ifdef DEBUG_MCU_TO_DDA
    printf("\r\nScreen_to_DDA ACK U Package:");
    for(i=0; i<APP_SPIDER.TxUsartLeng; i++)
    {
      printf("%02X ",APP_SPIDER.TxUsartData[i]);
    }
	printf("\r\n");
#endif
}


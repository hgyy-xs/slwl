#include "APP_Screen.h"

Screen_Ack_Struct APP_Screen_Ack;

void APP_Screen_dealSendCmdToSpider(void){
	PRO_spider_BuildCMDForPar(CMD_TYPE_TRFER,TRFER_CMD_SENDUPACK,APP_Screen_Ack.Data,Screen_Ack_Lenght);//生成U包命令
	APP_SPIDER.DealSendCmdToSpiderAddr = 1;//标志有命令需要发送到spider

}




#include "main.h"

ULTRAS_STRUCT  APP_ULTRASONIC;


unsigned char Ultrasonic_CmdCrcCal(unsigned char *pdata, unsigned int len);
/*
	变量初始化 main函数中
*/
void APP_Ultrasonic_ValueInit(void)
{
	APP_ULTRASONIC.InitOKFlag = FALSE;
	APP_ULTRASONIC.InitOverTimeCnt = 0;
	APP_ULTRASONIC.SendCmdOverTimeCnt = 0;
	
	APP_ULTRASONIC.InitDealAddr = 1;
	APP_ULTRASONIC.LocalAddr =  0;

	APP_ULTRASONIC.LedShakeFlag = 0;

	
}



/*
	超声波初始化
*/

void APP_Ultrasonic_ConfigInit(void)
{
	APP_ULTRASONIC.InitOverTimeCnt = 30;

	while(APP_ULTRASONIC.InitOverTimeCnt !=0 && APP_ULTRASONIC.InitOKFlag == FALSE)
	{
		//printf("APP_Ultrasonic_ConfigInit\r\n");
		APP_Ultrasonic_UsartDealForInit();
		APP_Ultrasonic_UartRxDealforInit();
	}
}



const u8 Ultrasonic_PollAddrCMD[] = {0xFA, 0xF9, 0x03};

void APP_Ultrasonic_UsartDealForInit(void)
{
	switch(APP_ULTRASONIC.InitDealAddr)
	{
		case 0:
			break;
		case 1:
			//发送查询地址命令
			printf("send Ultrasonic_PollAddrCMD\r\n");
			HAL_RS485_TxNbyte((u8 *)Ultrasonic_PollAddrCMD,sizeof(Ultrasonic_PollAddrCMD),USER_USART2);
			APP_ULTRASONIC.InitDealAddr ++;
			APP_ULTRASONIC.SendCmdOverTimeCnt = 200;
			break;
		case 2:
			if(APP_ULTRASONIC.SendCmdOverTimeCnt == 0)
			{
				if(APP_ULTRASONIC.InitOKFlag == TRUE)
				{
					printf("APP_ULTRASONIC.InitDealAddr2.1\r\n");
					APP_ULTRASONIC.InitDealAddr = 0;
				}
				else
				{
					printf("APP_ULTRASONIC.InitDealAddr2.2\r\n");
					APP_ULTRASONIC.InitDealAddr--;
				}
			}
			break;
		case 3:
			break;
		case 4:
			break;
		default:
			break;
	}
}
/*
	超声波初始化阶段串口接收处理
*/
void APP_Ultrasonic_UartRxDealforInit(void)
{
	u8 cnt;
	/// UART2  接收处理
	if(UART2.RxDataSucFlag==TRUE){
		for(cnt=0;cnt< UART2.RxCnt;cnt++)
		{
			printf("%02X ",UART2.RxBuff[cnt]);
		}
		printf("\r\n");
		if(UART2.RxCnt >= 3)
		{
			APP_ULTRASONIC.LocalAddr = UART2.RxBuff[3];
			APP_ULTRASONIC.InitOKFlag = TRUE;
			
			UART2.RxDealStep=1;//重新开始接收
		}
		
	}
}

/*
	超声波定时查询车位状态，在定时任务处理函数中
*/

void APP_Ultrasonic_FixedPollState(void)
{
	UART2.TxLeng = 0;
	UART2.TxBuff[UART2.TxLeng++] = 0xFA;
	UART2.TxBuff[UART2.TxLeng++] = APP_ULTRASONIC.LocalAddr;
	UART2.TxBuff[UART2.TxLeng++] = Ultrasonic_CmdCrcCal(UART2.TxBuff,3);
	HAL_RS485_TxNbyte(UART2.TxBuff,UART2.TxLeng,USER_USART2);
}

/*
	超声波工作模式下串口接收处理
*/
void APP_Ultrasonic_UartRxDealforWork(void)
{
	u8 s_tmp[18];
	u8 cnt;
	/// UART2  接收处理
	if(UART2.RxDataSucFlag==TRUE){

		if(UART2.RxCnt >= 3)
		{
			if(APP_ULTRASONIC.RevBuffCashe != UART2.RxBuff[1])
			{
				s_tmp[0] = 0x01;
				s_tmp[1] = UART2.RxBuff[1] >> 7;  //有车无车状态
				s_tmp[2] = APP_ULTRASONIC.LocalAddr;  //地址值
				s_tmp[4] = APP_ULTRASONIC.LedShakeFlag;
				for(cnt=0;cnt<15;cnt++)
				{
					s_tmp[cnt+1] = 0x00;
				}
				
				s_tmp[16] = DEV_TypeClass;
			
				PRO_spider_BuildCMDForPar(CMD_TYPE_TRFER,TRFER_CMD_SENDSTATUS,s_tmp,17);
				APP_SPIDER.DealSendCmdToSpiderAddr = 1;
			}
		}
		UART2.RxDealStep=1;//重新开始接收
		
	}
}

/*
	超声波传感器中的crc校验
*/
unsigned char Ultrasonic_CmdCrcCal(unsigned char *pdata, unsigned int len)
{
	unsigned int i;
	unsigned char xorByte = pdata[0];
	
    for(i=1;i<len;i++)
	{
		
		xorByte ^= pdata[i];
	}
    return xorByte; 
}




















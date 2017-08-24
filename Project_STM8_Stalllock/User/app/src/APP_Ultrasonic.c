#include "main.h"



#define ULTRASONIC_PRINT_SWITCH 0



ULTRAS_STRUCT  APP_ULTRASONIC;



const static u8 Ultrasonic_PollAddrCMD[3] = {0xFA, 0xF9, 0x03};



unsigned char Ultrasonic_CmdCrcCal(unsigned char *pdata, unsigned int len);



static Ultrasonic_Temporary_Buffer g_ultrasonic_temporary_buffer = {0};
static Ultrasonic_Control_Struct   g_ultrasonic_control_struct   = {0};



/*
	变量初始化 main函数中
*/
void APP_Ultrasonic_ValueInit(void)
{
	APP_ULTRASONIC.InitOKFlag = FALSE;
	APP_ULTRASONIC.InitOverTimeCnt = 0;
	APP_ULTRASONIC.SendCmdOverTimeCnt = 0;
	APP_ULTRASONIC.InitDealAddr = 1;
	APP_ULTRASONIC.LocalAddr =  1;
	APP_ULTRASONIC.LedShakeFlag = 0;
	APP_ULTRASONIC.RevBuffCashe = 0;
	APP_ULTRASONIC.LedShakeTimCnt = 0;
}


/*
	超声波初始化
*/
void APP_Ultrasonic_ConfigInit(void)
{
  APP_ULTRASONIC.InitOverTimeCnt = 30;   //设置超声波初始化超时时间为30s
  //Main_StrPar.Init_LED2_ShakeFlag = TRUE;//启动LED2闪烁
  
  while(APP_ULTRASONIC.InitOverTimeCnt !=0 && APP_ULTRASONIC.InitOKFlag == FALSE)//30s内一直查询超声波模块的地址,要么成功查询到了,要么超时了才退出
  {
    APP_Ultrasonic_UsartDealForInit();
    APP_Ultrasonic_UartRxDealforInit();
  }
  
  //Main_StrPar.Init_LED2_ShakeFlag = FALSE;//关闭LED2闪烁
  LED2_OFF();
}


void APP_Ultrasonic_UsartDealForInit(void)
{
	switch(APP_ULTRASONIC.InitDealAddr)
	{
		case 0:
			break;
		case 1:
			//发送查询地址命令
			HAL_USART_SendStringN((u8 *)Ultrasonic_PollAddrCMD,sizeof(Ultrasonic_PollAddrCMD),USER_USART2);
			APP_ULTRASONIC.InitDealAddr ++;
			APP_ULTRASONIC.SendCmdOverTimeCnt = 20;//4s
			break;
		case 2:
			if(APP_ULTRASONIC.SendCmdOverTimeCnt == 0)
			{
				if(APP_ULTRASONIC.InitOKFlag == TRUE)
				{
					APP_ULTRASONIC.InitDealAddr = 0;
				}
				else
				{
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
        //u8 cnt;
  
	if(UART2.RxDataSucFlag == TRUE)
	{
		//printf("UART2.RxDataSucFlag==TRUE\r\n");
		//printf("--%d--\r\n",UART2.RxCnt);
		/*for(cnt=0;cnt< UART2.RxCnt;cnt++)
		{
			printf("%02X ",UART2.RxBuff[cnt]);
		}
		printf("\r\n");*/
		
		if(UART2.RxCnt >= 4)
		{
			APP_ULTRASONIC.LocalAddr = UART2.RxBuff[3];//本机地址
			APP_ULTRASONIC.InitOKFlag = TRUE;
		}
		
		UART2.RxDealStep=1;//重新开始接收
		UART2.RxDataSucFlag = FALSE;
	}
}


/*
	超声波定时查询车位状态，在定时任务处理函数中
*/

void APP_Ultrasonic_FixedPollState(void)
{
  uint8_t i = 0;
  
  UART2.TxLeng = 0;
  UART2.TxBuff[UART2.TxLeng++] = 0xFA;
  UART2.TxBuff[UART2.TxLeng++] = APP_ULTRASONIC.LocalAddr;
  UART2.TxBuff[UART2.TxLeng++] = Ultrasonic_CmdCrcCal(UART2.TxBuff,2);
  
#ifdef DEBUG_MCU_STATUS
printf("【MCU to Ultrasonic】:");
  for(i=0; i<3; i++)
  {
    printf("%02X ",UART2.TxBuff[i]);
  }
  printf("\r\n");
#endif
  
  HAL_USART_SendStringN(UART2.TxBuff,UART2.TxLeng,USER_USART2);
}


/*
	超声波工作模式下串口接收处理
*/
void APP_Ultrasonic_UartRxDealforWork(void)
{
	u8 s_tmp[18]={0};
	s_tmp[0]=0x0A;	//U包服务质量
	s_tmp[1]=0x03;	//U包帧头，数据长度

	u8 cnt;
        
	if(UART2.RxDataSucFlag==TRUE){
          
		// if(UART2.RxCnt >= 3)
		// {
			for (cnt = 0; cnt < UART2.RxCnt; cnt++)	//将传感器数据放入U包组合
			{
				s_tmp[cnt+2]=UART2.RxBuff[cnt];
			}			
#ifdef DEBUG_MCU_STATUS
            printf("【Receive Data From Ultrasonic】:");
			for(cnt=0;cnt< UART2.RxCnt;cnt++)
			{
				printf("%02X ",UART2.RxBuff[cnt]);
			}
			printf("\r\n");
#endif
			if(UART2.RxCnt<6){
				memcpy((void *)g_ultrasonic_temporary_buffer.buf,(const void *)(s_tmp),13);
				g_ultrasonic_temporary_buffer.len = 13;	//length+data
#ifdef DEBUG_MCU_TO_DDA
            printf("【MCU to DDA U Package Data】:");
			for(cnt=0;cnt< g_ultrasonic_temporary_buffer.len;cnt++)
			{
				printf("%02X ",g_ultrasonic_temporary_buffer.buf[cnt]);
			}
			printf("\r\n");
#endif
				start_send_U_package();                     
			}else if (UART2.RxCnt>=6)	//查询地址类命令
			{
				memcpy((void *)g_ultrasonic_temporary_buffer.buf,(const void *)(s_tmp),13);	//传感器上报6位应答地址值
				g_ultrasonic_temporary_buffer.len = 13;	//length+data
#ifdef DEBUG_MCU_TO_DDA
            printf("【MCU to DDA U Package Data】:");
			for(cnt=0;cnt< g_ultrasonic_temporary_buffer.len;cnt++)
			{
				printf("%02X ",g_ultrasonic_temporary_buffer.buf[cnt]);
			}
			printf("\r\n");
#endif				
				start_send_U_package();                     

			}
			//if(APP_ULTRASONIC.RevBuffCashe != UART2.RxBuff[1])
			// {
				// APP_ULTRASONIC.RevBuffCashe = UART2.RxBuff[1];
				
				// s_tmp[0] = 0x0A;
				// s_tmp[1] = UART2.RxBuff[1] >> 7;  //有车无车状态
                                
				// //判断闪烁标志
				// if(APP_ULTRASONIC.LedShakeFlag == 1)//没有下发快捷令这个标志是不会为1的
				// {
				// 	if(s_tmp[1] == 1)//有车
				// 	{
				// 		//关闭闪烁命令 
				// 		s_tmp[0] = 0xFA;
				// 		s_tmp[1] = 0xFB;
				// 		s_tmp[2] = 0x01;
				// 		HAL_USART_SendStringN(s_tmp,3,USER_USART2);
                                                
				// 		APP_ULTRASONIC.LedShakeFlag = 0;	
				// 		APP_ULTRASONIC.LedShakeTimCnt = 0;
				// 	}
				// }
				
				// s_tmp[2] = APP_ULTRASONIC.LocalAddr;   //地址值
				// s_tmp[3] = APP_ULTRASONIC.LedShakeFlag;//闪烁状态
                                
				// for(cnt=4;cnt<12;cnt++)
				// {
    //                               s_tmp[cnt] = 0x00;
				// }
				
				// s_tmp[12] = DEV_TypeClass;//0xC1:超声波类
                                
    //                             if(g_ultrasonic_control_struct.perform_status == ULTRASONIC_IDLE)
    //                             {
    //                               memcpy((void *)g_ultrasonic_temporary_buffer.buf,(const void *)s_tmp,13);
    //                               g_ultrasonic_temporary_buffer.len = 13;
                                  
    //                               start_send_U_package();
    //                             }
			// }
		// }
          UART2.RxDealStep=1;//重新开始接收
          UART2.RxDataSucFlag = FALSE;
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


void start_send_U_package(void)
{
  g_ultrasonic_control_struct.perform_status = ULTRASONIC_ACTIVE;
  g_ultrasonic_control_struct.timer = 0;
  
  PRO_spider_BuildCMDForPar(CMD_TYPE_TRFER,TRFER_CMD_SENDUPACK,g_ultrasonic_temporary_buffer.buf,g_ultrasonic_temporary_buffer.len);//发送U包
  APP_SPIDER.DealSendCmdToSpiderAddr = 1;
}

void halt_send_U_package(void)
{
  memset((void *)&g_ultrasonic_temporary_buffer,0,sizeof(Ultrasonic_Temporary_Buffer));
  memset((void *)&g_ultrasonic_control_struct,0,sizeof(Ultrasonic_Control_Struct));
}

void ultrasonic_delay(void)
{
  if(g_ultrasonic_control_struct.perform_status == ULTRASONIC_ACTIVE)
  {
    g_ultrasonic_control_struct.timer++;
    if(g_ultrasonic_control_struct.timer > ULTRASONIC_RESEND_MAX_TIME)
    {
      start_send_U_package();
    }
  }
}


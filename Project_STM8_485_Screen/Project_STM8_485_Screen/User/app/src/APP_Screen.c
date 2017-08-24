#include "APP_Screen.h"

Screen_Ack_Struct APP_Screen_Ack;
Screen_Struct	APP_Screen;
APP_LED_STRUCT APP_LED;

void APP_LED_ValueInit(void)
{
	APP_LED.vehicle_count = 0;
}

void APP_Screen_dealSendCmdToSpider(void){
	u8 i=0;
	APP_Screen_Ack.Data[0]=0x0A;//服务质量
	//APP_Screen_Ack.Data[1]=APP_Screen.Tx_Data[7];	//screen ack length 	
	APP_Screen_Ack.Data[1]=ACK_PKG_CLASS ;			//应答数据包类型
	APP_Screen_Ack.Data[2]=0x01;					//具体命令 01H
	APP_Screen_Ack.Data[3]=0x01;					//screen ack length 	
	APP_Screen_Ack.Data[4]=0x80;					//screen ack status
	APP_Screen_Ack.Data[5]=0x00;
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


//命令格式 :  const u8 LED_SHOWCMD[] = {0xC9, 0x05, 0x00, 0x08, 0x00, 0x99, 0x5D, 0x9C};
void APP_Deal_U_Package_From_GW(void){

	u16 CmdDataLength = 0;
	UART2.TxLeng= 0;

	UART2.TxBuff[UART2.TxLeng ++] = 0xC9;
	CmdDataLength ++;

	UART2.TxBuff[UART2.TxLeng ++] = Screen_Addr ;
	CmdDataLength ++;

	//固定命令代码
	UART2.TxBuff[UART2.TxLeng ++] = 0x00;
	CmdDataLength ++;
	UART2.TxBuff[UART2.TxLeng ++] = 0x08;
	CmdDataLength ++;

	//车位数，十进制表示
	UART2.TxBuff[UART2.TxLeng ++] = APP_Screen.Tx_Data[10];//ALG_HextoAlg(APP_Screen.Tx_Data[12]/ 100);  //车位数高位
	CmdDataLength ++;

	UART2.TxBuff[UART2.TxLeng ++] = APP_Screen.Tx_Data[11];//ALG_HextoAlg( APP_Screen.Tx_Data[13]% 100);  //车位数低位
	CmdDataLength ++;

	UART2.TxBuff[UART2.TxLeng ++] =  ALG_CmdCrcCal1(& UART2.TxBuff[0],CmdDataLength);

	UART2.TxBuff[UART2.TxLeng ++] = 0x9C;
	HAL_RS485_TxNbyte( (u8 *)UART2.TxBuff,UART2.TxLeng,USER_USART2);
}	

void APP_LedShow_init(void)
{
  	u8 cnt;
	u8 s_tmp[18];
	 //读取内部EEPROM内部的数据，如果还没有存入数值，则初始化为0，已存入则将读出的数据设置为当前显示
	 HAL_eeprom_ReadData(VEHICLE_EEPADDR,s_tmp,2);
	 APP_LED.vehicle_count = s_tmp[0] + (s_tmp[1]<<8);
	 	
		// APP_LED_ShowDataReady();
		HAL_RS485_TxNbyte( (u8 *)UART2.TxBuff,UART2.TxLeng,USER_USART2);

		s_tmp[0] = 0x01;
		s_tmp[1] = APP_LED.vehicle_count % 256;  //车位数低位
		s_tmp[2] = APP_LED.vehicle_count / 256;  //车位数高位
		
		for(cnt=0;cnt<13;cnt++)
		{
			s_tmp[cnt+3] = 0x00;
		}

		s_tmp[16] = DEV_TypeClass;
	
		PRO_spider_BuildCMDForPar(CMD_TYPE_TRFER,TRFER_CMD_SENDSTATUS,s_tmp,17);
		//APP_SPIDER.DealSendCmdToSpiderAddr = 1;
		HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);
	
}

/*
	设置引导牌显示信息
*/
void APP_LEDSetShow(void)
{
		u16 CmdDataLength = 0;
		UART2.TxLeng= 0;
   
		UART2.TxBuff[UART2.TxLeng ++] = 0xC9;
		CmdDataLength ++;

		UART2.TxBuff[UART2.TxLeng ++] = Screen_Addr;
		CmdDataLength ++;

		//固定命令代码
		UART2.TxBuff[UART2.TxLeng ++] = 0x02;
		CmdDataLength ++;
		UART2.TxBuff[UART2.TxLeng ++] = 0x0E;
		CmdDataLength ++;

		UART2.TxBuff[UART2.TxLeng ++] = 0x30;  //箭头位置 30显示在左端 31 显示在右端
		CmdDataLength ++;

		UART2.TxBuff[UART2.TxLeng ++] = 0x30;  //变色门槛 0~99 如39 39 则为99
		CmdDataLength ++;
		UART2.TxBuff[UART2.TxLeng ++] = 0x35;  
		CmdDataLength ++;

		UART2.TxBuff[UART2.TxLeng ++] = 0x30;  //箭头类型 30：向前的箭头 31：向右 32：向左 33：向前右 34向前左
		CmdDataLength ++;

		UART2.TxBuff[UART2.TxLeng ++] = 0x30;  //箭头速度
		CmdDataLength ++;

		UART2.TxBuff[UART2.TxLeng ++] = 0x41;  //显示区号
		CmdDataLength ++;

		UART2.TxBuff[UART2.TxLeng ++] = 0x30;  
		CmdDataLength ++;
		UART2.TxBuff[UART2.TxLeng ++] = 0x30;  
		CmdDataLength ++;
		  
		UART2.TxBuff[UART2.TxLeng ++] =  ALG_CmdCrcCal1(UART2.TxBuff,CmdDataLength);
		  
		UART2.TxBuff[UART2.TxLeng ++] = 0x9C;
		
		HAL_RS485_TxNbyte( (u8 *)UART2.TxBuff,UART2.TxLeng,USER_USART2);
}

//按键增加
void APP_Deal_Vehicle_By_Key(void){

	u16 CmdDataLength = 0;
	UART2.TxLeng= 0;

	UART2.TxBuff[UART2.TxLeng ++] = 0xC9;
	CmdDataLength ++;

	UART2.TxBuff[UART2.TxLeng ++] = Screen_Addr ;
	CmdDataLength ++;

	//固定命令代码
	UART2.TxBuff[UART2.TxLeng ++] = 0x00;
	CmdDataLength ++;
	UART2.TxBuff[UART2.TxLeng ++] = 0x08;
	CmdDataLength ++;

	//车位数，十进制表示
	UART2.TxBuff[UART2.TxLeng ++] = APP_LED.vehicle_count/100;//ALG_HextoAlg(APP_Screen.Tx_Data[12]/ 100);  //车位数高位
	CmdDataLength ++;

	UART2.TxBuff[UART2.TxLeng ++] = APP_LED.vehicle_count%100;//ALG_HextoAlg( APP_Screen.Tx_Data[13]% 100);  //车位数低位
	CmdDataLength ++;

	UART2.TxBuff[UART2.TxLeng ++] =  ALG_CmdCrcCal1(& UART2.TxBuff[0],CmdDataLength);

	UART2.TxBuff[UART2.TxLeng ++] = 0x9C;
	HAL_RS485_TxNbyte( (u8 *)UART2.TxBuff,UART2.TxLeng,USER_USART2);
}
// //准备引导牌数据    命令格式 :  const u8 LED_SHOWCMD[] = {0xC9, 0x05, 0x00, 0x08, 0x00, 0x99, 0x5D, 0x9C};
// void APP_LED_ShowDataReady(void)
// {
// 	u16 CmdDataLength = 0;
// 	UART2.TxLeng= 0;
   
//   UART2.TxBuff[UART2.TxLeng ++] = 0xC9;
//    CmdDataLength ++;
   
//    UART2.TxBuff[UART2.TxLeng ++] = Led_showAddr;
//    CmdDataLength ++;

//    //固定命令代码
//    UART2.TxBuff[UART2.TxLeng ++] = 0x00;
//    CmdDataLength ++;
//    UART2.TxBuff[UART2.TxLeng ++] = 0x08;
//    CmdDataLength ++;

//    //车位数，十进制表示
//    UART2.TxBuff[UART2.TxLeng ++] = ALG_HextoAlg(APP_LED.vehicle_count / 100);  //车位数高位
//    CmdDataLength ++;
   
//    UART2.TxBuff[UART2.TxLeng ++] = ALG_HextoAlg(APP_LED.vehicle_count % 100);  //车位数低位
//    CmdDataLength ++;
      
//    UART2.TxBuff[UART2.TxLeng ++] =  ALG_CmdCrcCal1(& UART2.TxBuff[0],CmdDataLength);
      
//    UART2.TxBuff[UART2.TxLeng ++] = 0x9C;
// }

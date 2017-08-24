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
	APP_Screen_Ack.Data[0]=0x0A;//��������
	//APP_Screen_Ack.Data[1]=APP_Screen.Tx_Data[7];	//screen ack length 	
	APP_Screen_Ack.Data[1]=ACK_PKG_CLASS ;			//Ӧ�����ݰ�����
	APP_Screen_Ack.Data[2]=0x01;					//�������� 01H
	APP_Screen_Ack.Data[3]=0x01;					//screen ack length 	
	APP_Screen_Ack.Data[4]=0x80;					//screen ack status
	APP_Screen_Ack.Data[5]=0x00;
	PRO_spider_BuildCMDForPar(CMD_TYPE_TRFER,TRFER_CMD_SENDUPACK,APP_Screen_Ack.Data,Screen_Ack_Lenght);//����U������
	HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,USER_USART3);	//mcu to spider
	APP_SPIDER.DealSendCmdToSpiderAddr = 1;//��־��������Ҫ���͵�spider
#ifdef DEBUG_MCU_TO_DDA
    printf("\r\nScreen_to_DDA ACK U Package:");
    for(i=0; i<APP_SPIDER.TxUsartLeng; i++)
    {
      printf("%02X ",APP_SPIDER.TxUsartData[i]);
    }
	printf("\r\n");
#endif
}


//�����ʽ :  const u8 LED_SHOWCMD[] = {0xC9, 0x05, 0x00, 0x08, 0x00, 0x99, 0x5D, 0x9C};
void APP_Deal_U_Package_From_GW(void){

	u16 CmdDataLength = 0;
	UART2.TxLeng= 0;

	UART2.TxBuff[UART2.TxLeng ++] = 0xC9;
	CmdDataLength ++;

	UART2.TxBuff[UART2.TxLeng ++] = Screen_Addr ;
	CmdDataLength ++;

	//�̶��������
	UART2.TxBuff[UART2.TxLeng ++] = 0x00;
	CmdDataLength ++;
	UART2.TxBuff[UART2.TxLeng ++] = 0x08;
	CmdDataLength ++;

	//��λ����ʮ���Ʊ�ʾ
	UART2.TxBuff[UART2.TxLeng ++] = APP_Screen.Tx_Data[10];//ALG_HextoAlg(APP_Screen.Tx_Data[12]/ 100);  //��λ����λ
	CmdDataLength ++;

	UART2.TxBuff[UART2.TxLeng ++] = APP_Screen.Tx_Data[11];//ALG_HextoAlg( APP_Screen.Tx_Data[13]% 100);  //��λ����λ
	CmdDataLength ++;

	UART2.TxBuff[UART2.TxLeng ++] =  ALG_CmdCrcCal1(& UART2.TxBuff[0],CmdDataLength);

	UART2.TxBuff[UART2.TxLeng ++] = 0x9C;
	HAL_RS485_TxNbyte( (u8 *)UART2.TxBuff,UART2.TxLeng,USER_USART2);
}	

void APP_LedShow_init(void)
{
  	u8 cnt;
	u8 s_tmp[18];
	 //��ȡ�ڲ�EEPROM�ڲ������ݣ������û�д�����ֵ�����ʼ��Ϊ0���Ѵ����򽫶�������������Ϊ��ǰ��ʾ
	 HAL_eeprom_ReadData(VEHICLE_EEPADDR,s_tmp,2);
	 APP_LED.vehicle_count = s_tmp[0] + (s_tmp[1]<<8);
	 	
		// APP_LED_ShowDataReady();
		HAL_RS485_TxNbyte( (u8 *)UART2.TxBuff,UART2.TxLeng,USER_USART2);

		s_tmp[0] = 0x01;
		s_tmp[1] = APP_LED.vehicle_count % 256;  //��λ����λ
		s_tmp[2] = APP_LED.vehicle_count / 256;  //��λ����λ
		
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
	������������ʾ��Ϣ
*/
void APP_LEDSetShow(void)
{
		u16 CmdDataLength = 0;
		UART2.TxLeng= 0;
   
		UART2.TxBuff[UART2.TxLeng ++] = 0xC9;
		CmdDataLength ++;

		UART2.TxBuff[UART2.TxLeng ++] = Screen_Addr;
		CmdDataLength ++;

		//�̶��������
		UART2.TxBuff[UART2.TxLeng ++] = 0x02;
		CmdDataLength ++;
		UART2.TxBuff[UART2.TxLeng ++] = 0x0E;
		CmdDataLength ++;

		UART2.TxBuff[UART2.TxLeng ++] = 0x30;  //��ͷλ�� 30��ʾ����� 31 ��ʾ���Ҷ�
		CmdDataLength ++;

		UART2.TxBuff[UART2.TxLeng ++] = 0x30;  //��ɫ�ż� 0~99 ��39 39 ��Ϊ99
		CmdDataLength ++;
		UART2.TxBuff[UART2.TxLeng ++] = 0x35;  
		CmdDataLength ++;

		UART2.TxBuff[UART2.TxLeng ++] = 0x30;  //��ͷ���� 30����ǰ�ļ�ͷ 31������ 32������ 33����ǰ�� 34��ǰ��
		CmdDataLength ++;

		UART2.TxBuff[UART2.TxLeng ++] = 0x30;  //��ͷ�ٶ�
		CmdDataLength ++;

		UART2.TxBuff[UART2.TxLeng ++] = 0x41;  //��ʾ����
		CmdDataLength ++;

		UART2.TxBuff[UART2.TxLeng ++] = 0x30;  
		CmdDataLength ++;
		UART2.TxBuff[UART2.TxLeng ++] = 0x30;  
		CmdDataLength ++;
		  
		UART2.TxBuff[UART2.TxLeng ++] =  ALG_CmdCrcCal1(UART2.TxBuff,CmdDataLength);
		  
		UART2.TxBuff[UART2.TxLeng ++] = 0x9C;
		
		HAL_RS485_TxNbyte( (u8 *)UART2.TxBuff,UART2.TxLeng,USER_USART2);
}

//��������
void APP_Deal_Vehicle_By_Key(void){

	u16 CmdDataLength = 0;
	UART2.TxLeng= 0;

	UART2.TxBuff[UART2.TxLeng ++] = 0xC9;
	CmdDataLength ++;

	UART2.TxBuff[UART2.TxLeng ++] = Screen_Addr ;
	CmdDataLength ++;

	//�̶��������
	UART2.TxBuff[UART2.TxLeng ++] = 0x00;
	CmdDataLength ++;
	UART2.TxBuff[UART2.TxLeng ++] = 0x08;
	CmdDataLength ++;

	//��λ����ʮ���Ʊ�ʾ
	UART2.TxBuff[UART2.TxLeng ++] = APP_LED.vehicle_count/100;//ALG_HextoAlg(APP_Screen.Tx_Data[12]/ 100);  //��λ����λ
	CmdDataLength ++;

	UART2.TxBuff[UART2.TxLeng ++] = APP_LED.vehicle_count%100;//ALG_HextoAlg( APP_Screen.Tx_Data[13]% 100);  //��λ����λ
	CmdDataLength ++;

	UART2.TxBuff[UART2.TxLeng ++] =  ALG_CmdCrcCal1(& UART2.TxBuff[0],CmdDataLength);

	UART2.TxBuff[UART2.TxLeng ++] = 0x9C;
	HAL_RS485_TxNbyte( (u8 *)UART2.TxBuff,UART2.TxLeng,USER_USART2);
}
// //׼������������    �����ʽ :  const u8 LED_SHOWCMD[] = {0xC9, 0x05, 0x00, 0x08, 0x00, 0x99, 0x5D, 0x9C};
// void APP_LED_ShowDataReady(void)
// {
// 	u16 CmdDataLength = 0;
// 	UART2.TxLeng= 0;
   
//   UART2.TxBuff[UART2.TxLeng ++] = 0xC9;
//    CmdDataLength ++;
   
//    UART2.TxBuff[UART2.TxLeng ++] = Led_showAddr;
//    CmdDataLength ++;

//    //�̶��������
//    UART2.TxBuff[UART2.TxLeng ++] = 0x00;
//    CmdDataLength ++;
//    UART2.TxBuff[UART2.TxLeng ++] = 0x08;
//    CmdDataLength ++;

//    //��λ����ʮ���Ʊ�ʾ
//    UART2.TxBuff[UART2.TxLeng ++] = ALG_HextoAlg(APP_LED.vehicle_count / 100);  //��λ����λ
//    CmdDataLength ++;
   
//    UART2.TxBuff[UART2.TxLeng ++] = ALG_HextoAlg(APP_LED.vehicle_count % 100);  //��λ����λ
//    CmdDataLength ++;
      
//    UART2.TxBuff[UART2.TxLeng ++] =  ALG_CmdCrcCal1(& UART2.TxBuff[0],CmdDataLength);
      
//    UART2.TxBuff[UART2.TxLeng ++] = 0x9C;
// }

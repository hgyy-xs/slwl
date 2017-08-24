#include "main.h"

KEY_STRUCT HalKey1_StrPar;
KEY_STRUCT HalKey2_StrPar;

/*
 * �� �� ��: HAL_KEY_config
 * ��    ��: �������ŵĳ�ʼ��
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */
void HAL_KEY_config(void){
  GPIO_Init(KEY_GPIO, KEY1_GPIO_PIN, GPIO_Mode_In_PU_No_IT);//�������롢���ж�
}

/*
 * �� �� ��: HAL_KEY_Value
 * ��    ��: �����ṹ�帳��ʼֵ
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */
void HAL_KEY_Value(void){
	HalKey1_StrPar.press_cnt =0;		//������´���
	HalKey1_StrPar.nopre_cnt =0;		//����ɿ�����
	HalKey1_StrPar.press_status =FALSE;	//�������״̬

	
	HalKey2_StrPar.press_cnt =0;		//������´���
	HalKey2_StrPar.nopre_cnt =0;		//����ɿ�����
	HalKey2_StrPar.press_status =FALSE;	//�������״̬
}

/*
 * �� �� ��: HAL_KEY_Scan
 * ��    ��: ����ɨ��
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */
void HAL_KEY_Scan(void){
	if( KEY1_INPUT()==0 ){
		HalKey1_StrPar.press_cnt++;
		HalKey1_StrPar.nopre_cnt=0;
	}
	else{
		HalKey1_StrPar.nopre_cnt++;
	}

	if( KEY2_INPUT()==0 ){
		HalKey2_StrPar.press_cnt++;
		HalKey2_StrPar.nopre_cnt=0;
	}
	else{
		HalKey2_StrPar.nopre_cnt++;
	}
}

/*
 * �� �� ��: HAL_KEY_deal
 * ��    ��: ��������
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */
void HAL_KEY_deal(void){
	//u8 t_buffer[17] = {0x0A,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
	u8 s_tmp[50] = {0};
	//�̰����� LED1��һ�� (��⵽200ms�ڰ�����������)
	if((HalKey1_StrPar.press_cnt>=KEY_PRESS_SCAN_CNT+100)&&(HalKey1_StrPar.press_cnt<2000)&&(HalKey1_StrPar.nopre_cnt<KEY_FLOAT_SCAN_CNT)){
		if(HalKey1_StrPar.press_status != 1){
			HalKey1_StrPar.press_status=1;
			HalKey1_StrPar.nopre_cnt=0;
			LED1_ON();
		}
		if(HalKey1_StrPar.press_cnt>=1500){
			LED1_OFF();
		}
	}
	//�������� LED1������(������2s����)
	else if((HalKey1_StrPar.press_cnt>=KEY_PRESS_SCAN_CNT+2000)&&(HalKey1_StrPar.press_cnt<5000)&&(HalKey1_StrPar.nopre_cnt<KEY_FLOAT_SCAN_CNT))
	{
		if(HalKey1_StrPar.press_status!=2)
		{
			HalKey1_StrPar.press_status=2;
			HalKey1_StrPar.nopre_cnt=0;
			LED1_ON();
		}
		if(HalKey1_StrPar.press_cnt>=5000)
		{
			LED1_OFF();
		}
	}//�ɿ����
	else if((HalKey1_StrPar.nopre_cnt>=KEY_FLOAT_SCAN_CNT)&&((HalKey1_StrPar.press_status==1) || (HalKey1_StrPar.press_status==2) ))
	{
		HalKey1_StrPar.press_cnt=0;
		HalKey1_StrPar.nopre_cnt=0;
        
		LED1_OFF();		
		//--------����	���ն��������� {0x59,0x03,0x00,0x03,0x11,0x01,0x11,0x47};���ն���������  {0x59,0x03,0x00,0x03,0x11,0x02,0x14,0x47};
		if(HalKey1_StrPar.press_status ==1)//�̰�KEY1 ���ն�����
		{
			printf("\r\nKEY1 s1.\n");
			APP_SPIDER.Fast_Order5Flag = TRUE;
			APP_SPIDER.Fast_Order6Flag = FALSE;
// 			if(APP_LED.vehicle_count == 0)
// 			{
// 				APP_LED.vehicle_count = 1;
// 			}
// 			APP_LED.vehicle_count--;
// #ifdef DEBUG_MCU_STATUS
// 			printf("\r\n�ճ�λ����%d\n",APP_LED.vehicle_count);
// #endif
			
			// s_tmp[0] = 0x02;
			// PRO_spider_BuildCMDForPar(CMD_TYPE_CTRL,CTRL_CMD_SWITCHWORKMODE,s_tmp,1);
			// HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);
			// HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,USER_USART1);
			// APP_SPIDER.DealSendCmdToSpiderAddr = 1;
		}
		else if(HalKey1_StrPar.press_status ==2)//����KEY1 һ�����ն�����
		{
			s_tmp[0] = 0x01;
            PRO_spider_BuildCMDForPar(CMD_TYPE_CTRL,CTRL_CMD_SWITCHWORKMODE,s_tmp,1);
			HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);
			HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,USER_USART1);
            APP_SPIDER.DealSendCmdToSpiderAddr = 1;
		}
		HalKey1_StrPar.press_status=0;	// �ɿ�
	}

	//KEY2����
	if((HalKey2_StrPar.press_cnt>=KEY_PRESS_SCAN_CNT+100)&&(HalKey2_StrPar.press_cnt<2000)&&(HalKey2_StrPar.nopre_cnt<KEY_FLOAT_SCAN_CNT)){
		if(HalKey2_StrPar.press_status != 1){
			HalKey2_StrPar.press_status=1;
			HalKey2_StrPar.nopre_cnt=0;
			LED2_ON();
		}
		if(HalKey2_StrPar.press_cnt>=1500){
			LED2_OFF();
		}
	}//�̰�����
	else if((HalKey2_StrPar.press_cnt>=KEY_PRESS_SCAN_CNT+2000)&&(HalKey2_StrPar.press_cnt<5000)&&(HalKey2_StrPar.nopre_cnt<KEY_FLOAT_SCAN_CNT))
	{
		if(HalKey2_StrPar.press_status!=2)
		{
			HalKey2_StrPar.press_status=2;
			HalKey2_StrPar.nopre_cnt=0;
			LED2_ON();
		}
		if(HalKey2_StrPar.press_cnt>=5000)
		{
			LED2_OFF();
		}
	}//�ɿ����
	else if((HalKey2_StrPar.nopre_cnt>=KEY_FLOAT_SCAN_CNT)&&((HalKey2_StrPar.press_status==1) || (HalKey2_StrPar.press_status==2) ))
	{
		HalKey2_StrPar.press_cnt=0;
		HalKey2_StrPar.nopre_cnt=0;
    	
		LED2_OFF();		
		//--------����	���ն��������� {0x59,0x03,0x00,0x03,0x11,0x01,0x11,0x47};���ն���������  {0x59,0x03,0x00,0x03,0x11,0x02,0x14,0x47};
		if(HalKey2_StrPar.press_status ==1)//�̰�KEY2 ��λ��ת��
		{			
			printf("\r\nKEY2 s1.\n");
			APP_SPIDER.Fast_Order5Flag = FALSE;
			APP_SPIDER.Fast_Order6Flag = TRUE;
// 			if(APP_LED.vehicle_count == 9)
// 			{
// 				APP_LED.vehicle_count = 8;
// 			}
// 			APP_LED.vehicle_count++;
// #ifdef DEBUG_MCU_STATUS
// 			printf("\r\n�ճ�λ����%d\n",APP_LED.vehicle_count);   
// #endif
		}
		else if(HalKey2_StrPar.press_status ==2)//����KEY2 һ�����ն�����
		{
			// s_tmp[0] = 0x02;
   //              PRO_spider_BuildCMDForPar(CMD_TYPE_CTRL,CTRL_CMD_SWITCHWORKMODE,s_tmp,1);
			// 	HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);
			// 	HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,USER_USART1);
   //              APP_SPIDER.DealSendCmdToSpiderAddr = 1;
		}
		HalKey2_StrPar.press_status=0;	//H3V4�������ģʽ
	}
}



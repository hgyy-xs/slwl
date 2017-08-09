#include "main.h"


SPIDER_STRUCT  APP_SPIDER;


//------�̻��Ĺ�������------
const static u8 SPIDER_WORK_DATA[42] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC8,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,
			                  0x2C,0x01,0x3C,0x0A, 0x34,0x08,0x07,0x3C, 0x00,0xB4,0x00,0x3C, 0x00,0x2C,0x01,0x3C,
				          0x00,0x5A,0x00,0x00, 0x00,0x0A,0x00,0x1E, 0x00,0x00}; //37to42

/*
 * �� �� ��: APP_spider_ValueInit
 * ��    ��: spider�ṹ�帳��ʼֵ
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */
void APP_spider_ValueInit(void){
	APP_SPIDER.InitDealAddr = 0;		//���spider��ʼ���¼�ID
	APP_SPIDER.InitOKFlag = FALSE;		//���spider��ʼ����ɱ�־λ
	APP_SPIDER.IEPINOK_Flag = FALSE;	//���spider��ʼ���ж��¼���־λ
	APP_SPIDER.SingleRegFlag = FALSE;	//���spider���ն�������־λ
	APP_SPIDER.MultRegFlag = FALSE;	//���spider���ն�������־λ
	APP_SPIDER.MF_Flag = TRUE;			//�����ȡģ����̬��־λ
	APP_SPIDER.WP_Flag = TRUE;			//�����ȡ����������־λ
	APP_SPIDER.WakeupTimeCnt=0;		//�������ʱ�����
	APP_SPIDER.WakeupDealAddr=0;		//��������¼�ID
	APP_SPIDER.FastCmdFlag=FALSE;		//���������־λ
	APP_SPIDER.FastCmdLEDShakeFlag =FALSE;	//��������LED��˸��־λ
	APP_SPIDER.Judge_Is_OnNetFlag = FALSE;	//������ɱ�־λ
	APP_SPIDER.Judge_ReadyOnNetFlag = FALSE;//׼��������־λ
	APP_SPIDER.Judge_SendOnNetCmdFlag = TRUE;//�������������־λ
}


void APP_spider_UsartDealForInit(void)
{
	u8 i = 0;
	u8 s_tmp[100] = {0};
        
        switch(APP_SPIDER.InitDealAddr){
	case 0:break;
	case 1:
		// ��ȡģ����̬
		if(HalTim4_StrPar.TIM_T200mSFlag==FALSE)break;
		HalTim4_StrPar.TIM_T200mSFlag=FALSE;
                
		PRO_spider_BuildCMDForNoPar(CMD_TYPE_QUERY,QUERY_CMD_GETMODULESHAPE);
                	printf("\r\n��MCU to spider��:");
				for(i=0; i<APP_SPIDER.TxUsartLeng; i++)
				{
					printf("%02X ",APP_SPIDER.TxUsartData[i]);
				}
				printf("\r\n");
		HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);// ģ����̬��ѯ
                
		APP_SPIDER.InitDealAddr++;
		APP_SPIDER.UartRevOkFlag=FALSE;
		APP_SPIDER.UartRevOverFlag=FALSE;
		APP_SPIDER.UartRevOverCnt=500;//100
		APP_SPIDER.MF_Flag=FALSE;
		break;
	case 2:
		if(APP_SPIDER.UartRevOkFlag==TRUE){
			APP_SPIDER.UartRevOkFlag=FALSE;
                        APP_SPIDER.UartRevOverCnt=0;
			if(APP_SPIDER.MF_Flag==TRUE){
				APP_SPIDER.InitDealAddr++;
			}
			else{
				APP_SPIDER.InitDealAddr=11; //  ��ת��ģ����̬����
			}
		}
		if(APP_SPIDER.UartRevOverFlag==TRUE){
			APP_SPIDER.UartRevOverFlag=FALSE;
			APP_SPIDER.InitDealAddr--;	
			HalTim4_StrPar.TIM_T200mSFlag=FALSE;
		}
		break;
	case 3:
		// ��ȡ��������
		if(HalTim4_StrPar.TIM_T200mSFlag==FALSE)break;
		HalTim4_StrPar.TIM_T200mSFlag=FALSE;
		s_tmp[0] = 42;
		s_tmp[1] = 0x00;
                
		PRO_spider_BuildCMDForPar(CMD_TYPE_QUERY,QUERY_CMD_GETWORKPARAMETER,s_tmp,2);
                	printf("\r\n��MCU to spider��:");
				for(i=0; i<APP_SPIDER.TxUsartLeng; i++)
				{
					printf("%02X ",APP_SPIDER.TxUsartData[i]);
				}
				printf("\r\n");
		HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);// ����������ѯ
                
		APP_SPIDER.InitDealAddr++;	
		APP_SPIDER.UartRevOkFlag=FALSE;
		APP_SPIDER.UartRevOverFlag=FALSE;
		APP_SPIDER.UartRevOverCnt=500;//100
		break;
	case 4:
		
        if(APP_SPIDER.UartRevOkFlag==TRUE){
			APP_SPIDER.UartRevOkFlag=FALSE;
			APP_SPIDER.InitDealAddr++;
            APP_SPIDER.UartRevOverCnt=0;
		}
		if(APP_SPIDER.UartRevOverFlag==TRUE){
			APP_SPIDER.UartRevOverFlag=FALSE;
			APP_SPIDER.InitDealAddr--;	
			HalTim4_StrPar.TIM_T200mSFlag=FALSE;
		}
		break;
     case 5:
		//�ȽϹ��������Ƿ�Ϊ�趨ֵ
		if(ALG_CmpsConst(&UART3.RxBuff[8],42,SPIDER_WORK_DATA,42)==TRUE)
		{
			APP_SPIDER.InitDealAddr= 17;
		}
		else
		{
			// �Ƚ�ʧ�ܣ����ù�������
			APP_SPIDER.InitDealAddr=13;
                        APP_SPIDER.UartRevOverCnt=0;
		}
        break;
		
  
    case 6:
		if(APP_SPIDER.UartRevOkFlag==TRUE){
			APP_SPIDER.UartRevOkFlag=FALSE;
            APP_SPIDER.UartRevOverCnt=0;
			if(APP_SPIDER.MF_Flag==TRUE){
				APP_SPIDER.InitDealAddr++;
			}
			else{
				APP_SPIDER.InitDealAddr=13; //  ��ת��ģ����̬����
			}
		}
		if(APP_SPIDER.UartRevOverFlag==TRUE){
			APP_SPIDER.UartRevOverFlag=FALSE;
			APP_SPIDER.InitDealAddr--;	
			HalTim4_StrPar.TIM_T200mSFlag=FALSE;
		}
		break;
	case 7:
		APP_SPIDER.InitOKFlag=TRUE;
		APP_SPIDER.InitDealAddr=0;
		break;
	case 11:
		// ����Ϊһ����̬
		s_tmp[0] = 0x00;
                
		PRO_spider_BuildCMDForPar(CMD_TYPE_CONFIG,CONFIG_CMD_SETMODULECONFIG,s_tmp,1);
		HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);// ����������ѯ
                
		APP_SPIDER.InitDealAddr++;	
		APP_SPIDER.UartRevOkFlag=FALSE;
		APP_SPIDER.UartRevOverFlag=FALSE;
		APP_SPIDER.UartRevOverCnt=500;//100
		break;
	case 12:
		
		if(APP_SPIDER.UartRevOkFlag==TRUE){
			APP_SPIDER.UartRevOkFlag=FALSE;
			APP_SPIDER.InitDealAddr=1;
            APP_SPIDER.UartRevOverCnt=0;
		}
		if(APP_SPIDER.UartRevOverFlag==TRUE){
			APP_SPIDER.UartRevOverFlag=FALSE;
			APP_SPIDER.InitDealAddr--;	
		}
		break;
	case 13:
		// д�����
		s_tmp[0] = 42;//37to42
		s_tmp[1] = 0x00;//0x05to0x00
                
		for(i=0;i<42;i++)//37to42
		{
                  s_tmp[i+2] = SPIDER_WORK_DATA[i];
		}
                
		PRO_spider_BuildCMDForPar(CMD_TYPE_CONFIG,CONFIG_CMD_SETWORKPARAMETER,(u8 *)s_tmp,44);//39to44
                	printf("\r\n��MCU to spider��:");
				for(i=0; i<APP_SPIDER.TxUsartLeng; i++)
				{
					printf("%02X ",APP_SPIDER.TxUsartData[i]);
				}
				printf("\r\n");
		HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);// ����������ѯ
                
                APP_SPIDER.InitDealAddr++;	
		APP_SPIDER.UartRevOkFlag=FALSE;
		APP_SPIDER.UartRevOverFlag=FALSE;
		APP_SPIDER.UartRevOverCnt=500;//100
		break;
	case 14:
		if(APP_SPIDER.UartRevOkFlag==TRUE){
			APP_SPIDER.UartRevOkFlag=FALSE;
			APP_SPIDER.InitDealAddr=3;
                        APP_SPIDER.UartRevOverCnt=0;
		}
		if(APP_SPIDER.UartRevOverFlag==TRUE){
			APP_SPIDER.UartRevOverFlag=FALSE;
			APP_SPIDER.InitDealAddr--;	
		}
		break;
	case 15:
		break;
	case 16:
		break;
	case 17:
		// ��ѯ��������
		//{0x59,0x04,0x00,0x04,0x06,0x01,0x01,0x07,0x47}
		s_tmp[0] = 0x01;
		s_tmp[1] = 0x01;
		
		PRO_spider_BuildCMDForPar(CMD_TYPE_QUERY,QUERY_CMD_GETNETFILE,s_tmp,2);
              //  printf("MCU TO DDA ");
              				printf("\r\n��MCU to spider��:");
				for(i=0; i<APP_SPIDER.TxUsartLeng; i++)
				{
					printf("%02X ",APP_SPIDER.TxUsartData[i]);
				}
				printf("\r\n");
		HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);// ����������ѯ
                
		APP_SPIDER.InitDealAddr++;	
		APP_SPIDER.UartRevOkFlag=FALSE;
		APP_SPIDER.UartRevOverFlag=FALSE;
		APP_SPIDER.UartRevOverCnt=500;//100
		break;
	case 18:
		if(APP_SPIDER.UartRevOkFlag==TRUE){
			APP_SPIDER.UartRevOkFlag=FALSE;
			APP_SPIDER.InitOKFlag=TRUE;
			APP_SPIDER.InitDealAddr=0;
	        APP_SPIDER.UartRevOverCnt=0;
		}
		if(APP_SPIDER.UartRevOverFlag==TRUE){
			APP_SPIDER.UartRevOverFlag=FALSE;
			APP_SPIDER.InitDealAddr--;	
		}
		break;
	default:break;
  } 
}



void APP_spider_IEPIN_deal(void)
{
 if(APP_SPIDER.IEPINOK_Flag == TRUE)
 {
   printf("\r\nincoming a event.\r\n");
   
   APP_SPIDER.IEPINOK_Flag = FALSE;
   
   PRO_spider_BuildCMDForNoPar(CMD_TYPE_QUERY,QUERY_CMD_GETMODULESTATUS);
//   HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);
   APP_SPIDER.DealSendCmdToSpiderAddr = 1;
   
   APP_SPIDER.UartRevOverCnt=500;//100
  }
}



//��ѯ�ж��¼�����
static void Spider_IEPIN_StatusDeal(unsigned char *data)
{
//-----------����ģʽ�л��¼�
  if(data[6] &(1 << 0)){
 	switch(data[7] & 0x0f){
 		case 0x01:		//һ�����ն�����ģʽ
 		printf("\r\n��spider status��:only networking mode\r\n");
		if(Main_StrPar.Init_LED_ShakeFlag == FALSE){
			Main_StrPar.Init_LED1_ShakeFlag = FALSE;
			Main_StrPar.Init_LED_AlternateShakeFlag = TRUE;	//LED������˸����
		}
			break;
		case 0x02:		//һ�����ն�����ģʽ
 		printf("\r\n��spider status��:multi-terminal networking mode\r\n");
		if(Main_StrPar.Init_LED_ShakeFlag == FALSE){
			Main_StrPar.Init_LED1_ShakeFlag = FALSE;
			Main_StrPar.Init_LED_AlternateShakeFlag = TRUE;	//LED������˸����
		}
			break;
		case 0x03:		//��ͨģʽ
		printf("\r\n��spider status��:normal mode\r\n");
		if(Main_StrPar.Init_LED_ShakeFlag == FALSE){				//�жϳ�ʼ���Ƿ����
			Main_StrPar.Init_LED_AlternateShakeFlag = FALSE;	//LED������˸�ر�
			Main_StrPar.Init_LED1_ShakeFlag = TRUE;				//LED1��������
		}
			break;
		case 0x04:		//����ģʽ
		printf("\r\n��spider status��:sleep mode\r\n");
			break;
		default:
			break;
 	}
  }
   //---------------------����������¼�����  
  if(data[6] &(1 << 5)){
	switch(data[9] >> 4){
		case 0x01:			//����һ�������
		  //��ѯ�����
		  PRO_spider_BuildCMDForNoPar(CMD_TYPE_QUERY,QUERY_CMD_GETDOWNSTAIRFALST);
//		  HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);
                  APP_SPIDER.DealSendCmdToSpiderAddr = 1;
                  
      	          APP_SPIDER.UartRevOverCnt=500;//100
			break;
		case 0x02: 		//����һ��ע����
			break;
		case 0x03:			//����һ��U��
			 PRO_spider_BuildCMDForNoPar(CMD_TYPE_QUERY,QUERY_CMD_GETDOWNUPACK);
//		 	 HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);
                         APP_SPIDER.DealSendCmdToSpiderAddr = 1;
                         
                         APP_SPIDER.UartRevOverCnt=500;//100
			break;
		case 0x04:			//����һ��M��
			 PRO_spider_BuildCMDForNoPar(CMD_TYPE_QUERY,QUERY_CMD_GETDOWNMPACK);
//		 	 HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);
   			 APP_SPIDER.DealSendCmdToSpiderAddr = 1;
                         
                         APP_SPIDER.UartRevOverCnt=500;//100
			break;
			
		default:
			break;
	}
  }

  if(data[6] &(1 << 1)) //�е��ն������¼�����
  {
  	switch(data[7] >> 4)
  	{
  		case 0x01:					// ���ն�������ʼ
  			
			break;
  		case 0x02:					//���ն�������������
			APP_SPIDER.Judge_ReadyOnNetFlag = TRUE;
                        HAL_LED_AllOff();
			APP_SPIDER.Judge_Is_OnNetFlag = TRUE;	//�Ѿ�����
			break;
		case 0x03:					 //���ն�������ʱ1
			if(APP_SPIDER.Judge_Is_OnNetFlag == TRUE)//�Ѿ�����
			{
				APP_SPIDER.Judge_ReadyOnNetFlag = TRUE;
                                HAL_LED_AllOff();
			}
			else
			{
				APP_SPIDER.Judge_ReadyOnNetFlag = TRUE;
                                HAL_LED_AllOff();
				APP_SPIDER.Judge_SendOnNetCmdFlag = TRUE;
			}
			break;
		case 0x04:					 //���ն�������ʱ2
			if(APP_SPIDER.Judge_Is_OnNetFlag == TRUE)		//�Ѿ�����
			{
				APP_SPIDER.Judge_ReadyOnNetFlag = TRUE;
                                HAL_LED_AllOff();
			}
			else
			{
				APP_SPIDER.Judge_ReadyOnNetFlag = TRUE;
                                HAL_LED_AllOff();
				APP_SPIDER.Judge_SendOnNetCmdFlag = TRUE;
			}
			break;
		default:
			break;
  	}
  }
 //--------------------�ж��ն������¼�����  
  if(data[6] &(1 << 2)){
    switch(data[8] & 0x0f){
    	case 0x02:				//���ն�������������
    		APP_SPIDER.Judge_ReadyOnNetFlag = TRUE;
                HAL_LED_AllOff();
			APP_SPIDER.Judge_Is_OnNetFlag = TRUE;	//�Ѿ�����
			break;
		case 0x03:				 //���ն�������ʱ1
			
			if(APP_SPIDER.Judge_Is_OnNetFlag == TRUE)		//�Ѿ�����
			{
				APP_SPIDER.Judge_ReadyOnNetFlag = TRUE;
                                HAL_LED_AllOff();
			}
			else
			{
				APP_SPIDER.Judge_ReadyOnNetFlag = TRUE;
                                HAL_LED_AllOff();
				APP_SPIDER.Judge_SendOnNetCmdFlag = TRUE;
			}
			break;
		case 0x04:				//���ն�������ʱ2
			if(APP_SPIDER.Judge_Is_OnNetFlag == TRUE)		//�Ѿ�����
			{
				APP_SPIDER.Judge_ReadyOnNetFlag = TRUE;
                                HAL_LED_AllOff();
			}
			else
			{
				APP_SPIDER.Judge_ReadyOnNetFlag = TRUE;
                                HAL_LED_AllOff();
				APP_SPIDER.Judge_SendOnNetCmdFlag = TRUE;
			}
			break;
		default:
			break;
    }  
  }
//----------------------------�����¼�����
  if(data[6] &(1 << 4)){
	switch(data[9] & 0x0f){
    	case 0x01:			//һ������U�����
			break;
    	case 0x02:			//һ������U����ʱ
			break;
		case 0x03:		//һ������M�����
			break;
		case 0x04:		//һ������M����ʱ
			break;
		default:
			break;
	  }
   }
}


/*
  spider��ʼ������
*/
void APP_spider_init(void){
	uint16_t i = 0;
	APP_SPIDER.StartDly = SPIDER_DLY_TIME;			  //��ֵSTM������ʱ
	APP_SPIDER.InitOKFlag = FALSE;					  //���STM��ʼ���ɹ���־λ
	APP_SPIDER.InitOverTimeCnt = MAIN_SPIDER_INIT_TIME;//��ֵSTM��ʼ��ʱ��
	HalTim4_StrPar.TIM_count = 0;						  //����TIM4�ļ���ֵ
	while(HalTim4_StrPar.TIM_count <= 500){
		if(HalTim4_StrPar.TIM_count > 300){
			SPIDER_TM_RESET_ENABLE();					//��0.3s��0.5s֮�临λSTM
			break;
		}
	}
	for(i = 0; i < 0xFF; i++);								//��ʱ
	SPIDER_TM_RESET_DISABLE();							//������λ
	for(i = 0; i < 0xFFFF; i++);							//��ʱ
	while((APP_SPIDER.InitOverTimeCnt != 0 ) && (APP_SPIDER.InitOKFlag == FALSE )){
      if(HalTim4_StrPar.TIM_T1SFlag == TRUE){
			HalTim4_StrPar.TIM_T1SFlag = FALSE;		//���1���־λ
			APP_SPIDER.InitOverTimeCnt--;				//����ʱ��ʱʱ��
			if(APP_SPIDER.StartDly != 0){
				APP_SPIDER.StartDly --;					//����ʱ����ʱ��
				printf("\r\n%d\r\n",APP_SPIDER.StartDly);
				if(APP_SPIDER.StartDly == 0){
					APP_SPIDER.InitDealAddr = 1;			//���ô���spider��ʼ���¼�IDΪ1
				}
			}
		}
		/* ��ʱδ��һ��:spider�¼�IDΪ0	������*/
		/* ������ʱ����:spider�¼�IDΪ1	��ȡģ����̬,��һ�ζ�ȡ����ʱ,֮���ȡ��ʱ200ms*/
		APP_spider_UsartDealForInit();
		APP_spider_uartDeal();
		APP_Spider_dealSendCmdToSpider();
		APP_spider_IEPIN_deal();
	}
	if(APP_SPIDER.InitOKFlag==TRUE){
		printf("\r\nSpider module init success!\r\n");
	}
	else{
		printf("\r\nSpider module init timeout!\r\n");
	}
}

void APP_spider_uartDeal(void){
	u8 i = 0;
	u8 crc1 = 0;
	u8 len1=3,len2=4,len3=5;

	// UART3  ���մ���
	if(UART3.RxDataSucFlag==TRUE){
        
		// ��������
		crc1= ALG_CmdCrcCal(&UART3.RxBuff[1],(UART3.RxLeng+2));//����У��ֵ
                
		if(UART3.RxBuff[UART3.RxLeng+3]==crc1){
                  
			APP_SPIDER.UartRevOkFlag=TRUE;
                        
            printf("DDA to MCU ");
            for(int cnt=0;cnt<UART3.RxLeng+5;cnt++)
			{
				printf("%02X ",UART3.RxBuff[cnt]);
			}
			printf("\r\n");

                        
			// �ж�Ӧ����
			if(UART3.RxBuff[len3]==0x80){
                          
				switch(UART3.RxBuff[len1]){
					case 0:break;
					case 1://������
						if(UART3.RxBuff[len2]==0x01){

						}
						else if(UART3.RxBuff[len2]==0x02){
							
						}
						break;
					case 2:  // ������
						
						break;
					case 3:  // ������
						if(UART3.RxBuff[len2]==0x01){

						}
						else if(UART3.RxBuff[len2]==0x02){

						}
						else if(UART3.RxBuff[len2]==0x03){

						}
						else if(UART3.RxBuff[len2]==0x04){

						}
						else if(UART3.RxBuff[len2]==0x11){

						}
						break;
					case 4: //  ��ѯ��
						if(UART3.RxBuff[len2]==0x01){
							// ģ��״̬
							Spider_IEPIN_StatusDeal(UART3.RxBuff);
						}
						else if(UART3.RxBuff[len2]==0x02){
							// ��������
						}
						else if(UART3.RxBuff[len2]==0x03){
							// ����汾��
						}
						else if(UART3.RxBuff[len2]==0x04){
							// ģ�����к�

						}
						else if(UART3.RxBuff[len2]==0x05){
							// ��������״̬

						}
						else if(UART3.RxBuff[len2]==0x06){
							// ��������
							APP_SPIDER.Judge_Is_OnNetFlag = UART3.RxBuff[9];//������־
						}
						else if(UART3.RxBuff[len2]==0x07){
							// ���߲���

						}
						else if(UART3.RxBuff[len2]==0x08){
							// ϵͳʱ��

						}
						else if(UART3.RxBuff[len2]==0x09){

						}
						else if(UART3.RxBuff[len2]==0x0A){
							if(UART3.RxBuff[len3+1]==0x00)// ģ����̬Ϊһ��
							{
								APP_SPIDER.MF_Flag=TRUE;
							}
						}
						else if(UART3.RxBuff[len2]==0x21){
							// ����״̬

						}
						else if(UART3.RxBuff[len2]==0x22){
							// �ϴ�U������
                                                  
						}
						else if(UART3.RxBuff[len2]==0x23){
							// �ϴ�M������
                                                  
						}
						else if(UART3.RxBuff[len2]==0x24){
							// �ϴ�P������

						}
						else if(UART3.RxBuff[len2]==0x31){
                                                  
							APP_SPIDER.FastCmdFlag=TRUE;//about LED.
                                                        
							//���п����
							switch(UART3.RxBuff[len3+1])
							{
								case 0:
									break;
								case 1:
									 break;
								case 2:
									break;
								case 3:
									break;
								case 4:
									break;
                                case 5:
									break;
								case 6:
									break;
								case 7:
									break;
								case 8:
									break;
								default:
									break;
							}							
						}
						else if(UART3.RxBuff[len2]==0x32){
							//����U��
                                                  
                                                printf("\r\nReceived U Package:");
                                                for(i=0; i<UART3.RxLeng+5; i++)
                                                {
                                                  printf("%02X ",UART3.RxBuff[i]);
												  APP_Screen.Tx_Data[i]=UART3.RxBuff[i];	//����U������
                                                }
                                                printf("\r\n");
                                                printf("DDA TO Screen U���ݳ��ȣ�%02x\n",APP_Screen.Tx_Data[7]);
												
                                                HAL_RS485_TxNbyte( (u8 *)(APP_Screen.Tx_Data+8),(u16)APP_Screen.Tx_Data[7],USER_USART2);	//485��������ת������������
                                            	//HAL_RS485_TxNbyte( (u8 *)(UART3.RxBuff+7),UART3.RxLeng-4,USER_USART2);	//485��������ת��
												// PRO_spider_BuildCMDForPar(CMD_TYPE_TRFER,TRFER_CMD_SENDUPACK,g_temporary_buffer_for_screen.buffer,3);//����U������
												// APP_SPIDER.DealSendCmdToSpiderAddr = 1;//��־��������Ҫ���͵�spider
												//APP_Screen_dealSendCmdToSpider();
                                            	
						}
						else if(UART3.RxBuff[len2]==0x33){
							//����M��
                                                printf("\r\nReceived M Package:");
                                                for(i=0; i<UART3.RxLeng+5; i++)
                                                {
                                                  printf("%02X ",UART3.RxBuff[i]);
												  APP_Screen.Tx_Data[i]=UART3.RxBuff[i];	//����M������
                                                }
                                                printf("\r\n");
                                                printf("DDA TO Screen M���ݳ��ȣ�%02x\n",APP_Screen.Tx_Data[7]);
												
                                                HAL_RS485_TxNbyte( (u8 *)(APP_Screen.Tx_Data+8),(u16)APP_Screen.Tx_Data[7],USER_USART2);	//485��������ת������������
                                            	// HAL_RS485_TxNbyte( (u8 *)(UART3.RxBuff+7),UART3.RxLeng-4,USER_USART2);	//485��������ת��
						}
						else if(UART3.RxBuff[len2]==0x34){
							//����P������
                                                  
						}
						break;
					case 5:  // ������
						if(UART3.RxBuff[len2]==0x01){
                                                  
						}
						else if(UART3.RxBuff[len2]==0x02){

						}
						else if(UART3.RxBuff[len2]==0x03){

						}
						else if(UART3.RxBuff[len2]==0x04){

						}
						else if(UART3.RxBuff[len2]==0x05){

						}
						else if(UART3.RxBuff[len2]==0x06){

						}
						else if(UART3.RxBuff[len2]==0x12){
                                                  
						}
						break;
					default:break;
				}
			}
		}
                
		// ���¿�ʼ����
		UART3.RxDataSucFlag=FALSE;
		UART3.RxDealStep=1;
	}
  
}



/*
	ע:�������뵽1s��ʱ������
*/
void APP_spider_FastCmdLEDshake(void)
{
	if(APP_SPIDER.FastCmdLEDShakeFlag==TRUE)
	{
		//LED6_XOR();
	}
	else
	{
		if(APP_SPIDER.FastCmdFlag==TRUE)
		{
			//LED6_OFF();
		}
		
	}
	APP_SPIDER.FastCmdFlag=FALSE;
}




/*
	��������Ӧ��
*/
void APP_Spider_dealSendCmdToSpider(void){
	u8 Addr = APP_SPIDER.DealSendCmdToSpiderAddr;
	u8 s_tmp[5];
	switch(Addr){
		case 0:		//�޲���
			break;
		case 1:
			if(HalTim4_StrPar.TIM_T200mSFlag==FALSE)break;
			HalTim4_StrPar.TIM_T200mSFlag=FALSE;
			HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);
				
				Addr++;
				APP_SPIDER.UartRevOkFlag=FALSE;
				APP_SPIDER.UartRevOverFlag=FALSE;
				APP_SPIDER.UartRevOverCnt=1000;//100
			break;
		case 2:
			if(APP_SPIDER.UartRevOkFlag==TRUE){
				APP_SPIDER.UartRevOkFlag=FALSE;
				Addr = 0;
		        APP_SPIDER.UartRevOverCnt=0;
			}
			if(APP_SPIDER.UartRevOverFlag==TRUE){
				APP_SPIDER.UartRevOverFlag=FALSE;
				Addr--;	
				HalTim4_StrPar.TIM_T200mSFlag=FALSE;
			}
			break;
                        
		//-----------�л�Spider�Ĺ���ģʽΪһ�����ն�/���ն�ģʽ
		case 5:
			if(HalTim4_StrPar.TIM_T200mSFlag==FALSE)break;		//200ms��־λΪ�٣��˳�switch
			HalTim4_StrPar.TIM_T200mSFlag=FALSE;	//�����־λ
			s_tmp[0] = 0x01;						//���ն�����
			PRO_spider_BuildCMDForPar(CMD_TYPE_CTRL,CTRL_CMD_SWITCHWORKMODE,s_tmp,1);					//��������
		  	HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);//��������
			Addr++;									//���÷������spiderģ����¼�IDΪ6
			APP_SPIDER.UartRevOkFlag=FALSE;		//���spider���ڽ�����ɱ�־λ
			APP_SPIDER.UartRevOverFlag=FALSE;		//���spider���ڽ��ճ�ʱ��־λ
			APP_SPIDER.UartRevOverCnt=500;			//����spider���ڽ��ճ�ʱʱ��Ϊ500s	
			break;
		case 6:
			if(APP_SPIDER.UartRevOkFlag==TRUE){
				APP_SPIDER.UartRevOkFlag=FALSE;
				Addr = 0;
		        APP_SPIDER.UartRevOverCnt=0;
				
			}
			if(APP_SPIDER.UartRevOverFlag==TRUE){
				APP_SPIDER.UartRevOverFlag=FALSE;
				Addr--;	
				HalTim4_StrPar.TIM_T200mSFlag=FALSE;
			}
			break;

		default :
			break;
		}
	 APP_SPIDER.DealSendCmdToSpiderAddr = Addr;
}

/*
	׼������
	�ж��Ƿ�������������һֱ��������ģʽ�������������򽫽���һ�����������۳�ʱ���������ɹ��������������ĳ���
*/
void APP_Spider_ReadyOnNet(void){
	while(APP_SPIDER.Judge_ReadyOnNetFlag == FALSE){	//�ж��Ƿ������������ģʽ
		if(APP_SPIDER.Judge_SendOnNetCmdFlag == TRUE){	//�ж��Ƿ�����������
			APP_SPIDER.Judge_SendOnNetCmdFlag = FALSE;	//ȡ��������������
			APP_SPIDER.DealSendCmdToSpiderAddr = 5;	//���÷������spiderģ����¼�IDΪ5
		}
		APP_Spider_dealSendCmdToSpider();				//�������spiderģ����¼�
		APP_spider_uartDeal();							//spider���ڽ������ݴ���
		APP_spider_IEPIN_deal();
	}
}




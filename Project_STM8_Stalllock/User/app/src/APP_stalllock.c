#include "main.h"


StallLock_STRUCT  APP_StallLock_StrPar;


static Ring_Buffer_For_StallLock		g_ring_buffer_for_stalllock      = {0};
static Temporary_Buffer_For_StallLock	g_temporary_buffer_for_stalllock = {0};
StallLock_Control_Struct				g_stalllock_control_struct       = {0};


/*
��λ�����ų�ʼ��
*/
void APP_stalllock_GPIOInit(void)
{
    //k1/k2/k3 �ų�ʼ��
    GPIO_Init(K_GPIO,K1_GPIO_PIN,GPIO_Mode_Out_PP_Low_Fast);//k1 ����͵�ƽ
    GPIO_Init(K_GPIO,K2_GPIO_PIN,GPIO_Mode_In_PU_No_IT);//k2/k3Ϊ��������
    GPIO_Init(K_GPIO,K3_GPIO_PIN,GPIO_Mode_In_PU_No_IT);	
    
    //�������ת���� ��ʼ��ȫ����
    GPIO_Init(MOT_GPIO,MOT_GPIO_PIN1,GPIO_Mode_Out_PP_Low_Fast);//��ת
    GPIO_Init(MOT_GPIO,MOT_GPIO_PIN2,GPIO_Mode_Out_PP_Low_Fast);//��ת
    
    //�����ź������  ����
    GPIO_Init(WARNING_LED_GPIO,WARNING_LED_GPIO_PIN,GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(WARNING_BEEP_GPIO,WARNING_BEEP_GPIO_PIN,GPIO_Mode_Out_PP_Low_Fast);
}

void APP_stalllock_ValueInit(void)
{
	APP_StallLock_StrPar.K2_value = 0;
	APP_StallLock_StrPar.K3_value = 0;

	APP_StallLock_StrPar.K2_CacheValue = 0;
	APP_StallLock_StrPar.K3_CacheValue = 0;

	APP_StallLock_StrPar.NoCmdPressFlag1 = FALSE;
    APP_StallLock_StrPar.NoCmdFlag2 = FALSE;
		
	APP_StallLock_StrPar.WarningFlag = FALSE;
	APP_StallLock_StrPar.WarningCount = 0;
	APP_StallLock_StrPar.WarningTimCnt = 0;
	APP_SPIDER.Fast_Order6Flag = FALSE;	

	APP_StallLock_StrPar.TimCnt = 10;

	APP_StallLock_StrPar.MontorTurnTimMaxcount = 0;
}



void APP_stalllock_StateDeal(void)
{
	u8 s_tmp[18] = {0};
	u8 cnt = 0;
        
//--------------����ʹ��
	if(APP_SPIDER.Fast_Order5Flag == TRUE)//�½�//���տ����5
	{
		if((APP_StallLock_StrPar.K2_value == 0 && APP_StallLock_StrPar.K3_value == 0)\
		|| (APP_StallLock_StrPar.K2_value == 8 && APP_StallLock_StrPar.K3_value == 0))//90��״̬  ���� ����0��С��90�� ��ʼ��ת
	 	{
			MOT_PIN1_LOW();
			MOT_PIN2_LOW();

	 		MOT_PIN1_LOW();
			MOT_PIN2_HIGH();
			APP_StallLock_StrPar.MontorTurnTimFlag = TRUE;
			
	 	}
		else if(APP_StallLock_StrPar.K2_value == 8 && APP_StallLock_StrPar.K3_value == 4)//����0�ȣ�ֹͣת��
		{
			APP_SPIDER.Fast_Order5Flag = FALSE;
			APP_StallLock_StrPar.MontorTurnTimFlag = FALSE;         
			MOT_PIN1_LOW();
			MOT_PIN2_LOW();
              //APP_StallLock_StrPar.MontorTurnTimMaxcount = MONTORTURNTIMMAXCNT;          
			if(Main_StrPar.SpidOrH3v4Flag != FALSE)
			{
				//�ϱ���ʱ��λ��״̬ ������׼��
				s_tmp[0] = 0x0A;
				for(cnt=0;cnt<12;cnt++)
				{
					s_tmp[cnt+1] = 0x00;
				}
				s_tmp[3] = HAL_adc_StrPar.WorkVoltgeGrade;
				s_tmp[12] = DEV_TypeClass;
				
				//PRO_spider_BuildCMDForPar(CMD_TYPE_TRFER,TRFER_CMD_SENDSTATUS,s_tmp,17);
				//APP_SPIDER.DealSendCmdToSpiderAddr = 1;

                if(push_data_into_ring_buffer(s_tmp,13) == 0)//�����ݷ��뻷������,���˷���0
                {
                  printf("\r\n��%s�����ζ�������\r\n",__FUNCTION__);
                }
			}
		}
	 }
	 else if(APP_SPIDER.Fast_Order6Flag == TRUE)		//����//�����6
	 {
	 	if((APP_StallLock_StrPar.K2_value == 8 && APP_StallLock_StrPar.K3_value == 4)\
			|| (APP_StallLock_StrPar.K2_value == 8 && APP_StallLock_StrPar.K3_value == 0))	//0��״̬ ���� ����0��С��90��    ��ʼ��ת
	 	{
			MOT_PIN1_LOW();
			MOT_PIN2_LOW();
			
	 		MOT_PIN1_HIGH();
			MOT_PIN2_LOW();
			APP_StallLock_StrPar.MontorTurnTimFlag = TRUE;  
			//APP_StallLock_StrPar.MontorTurnTimMaxcount = MONTORTURNTIMMAXCNT;
	 	}
		else if(APP_StallLock_StrPar.K2_value == 0 && APP_StallLock_StrPar.K3_value == 4)	//����90�ȣ���ʼ��ת
		{
			MOT_PIN1_LOW(); 
			MOT_PIN2_HIGH();
			APP_StallLock_StrPar.MontorTurnTimFlag = TRUE;  
			//APP_StallLock_StrPar.MontorTurnTimMaxcount = MONTORTURNTIMMAXCNT;
		}
		else if(APP_StallLock_StrPar.K2_value == 0 && APP_StallLock_StrPar.K3_value == 0)	//����90�ȣ�ֹͣת��
		{
		APP_StallLock_StrPar.MontorTurnTimFlag = FALSE;  
			APP_SPIDER.Fast_Order6Flag = FALSE;
			MOT_PIN1_LOW();
			MOT_PIN2_LOW();
			if(Main_StrPar.SpidOrH3v4Flag != FALSE)
			{
				//�ϱ���ʱ��λ��״̬
				s_tmp[0] = 0x0A;
				s_tmp[1] = 0x01;
				for(cnt=0;cnt<11;cnt++)
				{
					s_tmp[cnt+2] = 0x00;
				}
				s_tmp[3] = HAL_adc_StrPar.WorkVoltgeGrade;
				s_tmp[12] = DEV_TypeClass;
				
				//PRO_spider_BuildCMDForPar(CMD_TYPE_TRFER,TRFER_CMD_SENDSTATUS,s_tmp,17);
				//APP_SPIDER.DealSendCmdToSpiderAddr = 1;
                                
                                if(push_data_into_ring_buffer(s_tmp,13) == 0)
                                {
                                  printf("\r\n��%s�����ζ�������\r\n",__FUNCTION__);
                                }
			}
		}
	 }
//------------ǿ����90��״̬�£����³�λ��(����0~90��)
	else
	{
		if(APP_StallLock_StrPar.K2_CacheValue == 0 && APP_StallLock_StrPar.K3_CacheValue == 0 \
			&& APP_StallLock_StrPar.K2_value == 8 && APP_StallLock_StrPar.K3_value == 0  \
			&& APP_StallLock_StrPar.NoCmdPressFlag1 == FALSE)
		{
			//��ʼ��ת
			APP_StallLock_StrPar.NoCmdPressFlag1 = TRUE;

			APP_StallLock_StrPar.WarningCount ++;
			APP_StallLock_StrPar.WarningFlag = TRUE;
			APP_StallLock_StrPar.WarningTimCnt = WARNINGTIMECNT;
			//����,mcu��������źţ����ϱ������źţ��������һֱδ�����ÿ��һ��ʱ���ϱ�һ��
			WARNING_LED_MAKE();
			WARNING_BEEP_MAKE();

			APP_StallLock_StrPar.MontorTurnTimMaxcount = MONTORTURNTIMMAXCNT;
			if(Main_StrPar.SpidOrH3v4Flag != FALSE)
			{
				//�ϱ���ʱ��λ��״̬
				s_tmp[0] = 0x0A;
				s_tmp[1] = 0x01;
				s_tmp[2] = APP_StallLock_StrPar.WarningCount;
				for(cnt=0;cnt<10;cnt++)
				{
					s_tmp[cnt+3] = 0x00;
				}
				s_tmp[3] = HAL_adc_StrPar.WorkVoltgeGrade;
				s_tmp[12] = DEV_TypeClass;
				
				//PRO_spider_BuildCMDForPar(CMD_TYPE_TRFER,TRFER_CMD_SENDSTATUS,s_tmp,17);
				//APP_SPIDER.DealSendCmdToSpiderAddr = 1;
                                
                                if(push_data_into_ring_buffer(s_tmp,13) == 0)
                                {
                                  printf("\r\n��%s�����ζ�������\r\n",__FUNCTION__);
                                }
			}
		}
		
		if(APP_StallLock_StrPar.NoCmdPressFlag1 == TRUE)
		{
			if(APP_StallLock_StrPar.K2_value == 8 && APP_StallLock_StrPar.K3_value == 0)		//����0��С��90��
			{
				MOT_PIN1_LOW();
				MOT_PIN2_HIGH();
			}
			else if(APP_StallLock_StrPar.K2_value == 8 && APP_StallLock_StrPar.K3_value == 4)	//0��״̬��ֹͣת��������ת
			{
				MOT_PIN1_LOW();
				MOT_PIN2_LOW();
				
				MOT_PIN1_HIGH();
				MOT_PIN2_LOW();
				APP_StallLock_StrPar.NoCmdPressFlag1 = FALSE;
				APP_StallLock_StrPar.NoCmdFlag2 = TRUE;
			}
		}
		else if(APP_StallLock_StrPar.NoCmdFlag2 == TRUE)
		{
			printf("NoCmdPressFlag2 = true\r\n");
			printf("===%d====%d===\r\n",APP_StallLock_StrPar.K2_value,APP_StallLock_StrPar.K3_value);
                        
			if(APP_StallLock_StrPar.K2_value == 0 && APP_StallLock_StrPar.K3_value == 0)	//�ص�90��״̬��ֹͣת��
			{
				APP_StallLock_StrPar.NoCmdFlag2 = FALSE;
				//���������mcuֹͣ��������ź�
				WARNING_LED_REMOVE();
				WARNING_BEEP_REMOVE();
				if(Main_StrPar.SpidOrH3v4Flag != FALSE)
				{
					s_tmp[0] = 0x0A;
					s_tmp[1] = 0x01;
					for(cnt=1;cnt<12;cnt++)
					{
						s_tmp[cnt+1] = 0x00;
					}
					s_tmp[3] = HAL_adc_StrPar.WorkVoltgeGrade;
					s_tmp[12] = DEV_TypeClass;
					
					//PRO_spider_BuildCMDForPar(CMD_TYPE_TRFER,TRFER_CMD_SENDSTATUS,s_tmp,17);
					//APP_SPIDER.DealSendCmdToSpiderAddr = 1;
                                        
                                        if(push_data_into_ring_buffer(s_tmp,13) == 0)
                                        {
                                          printf("\r\n��%s�����ζ�������\r\n",__FUNCTION__);
                                        }
				}
				APP_StallLock_StrPar.WarningFlag = FALSE;
				APP_StallLock_StrPar.WarningCount = 0;

				MOT_PIN1_LOW();
				MOT_PIN2_LOW();
			}
		}
		//�жϾ����Ƿ���Ҫ����
		if(APP_StallLock_StrPar.WarningFlag == TRUE && APP_StallLock_StrPar.WarningTimCnt == 0)
		{
			if(Main_StrPar.SpidOrH3v4Flag != FALSE)
			{
				s_tmp[0] = 0x0A;
				s_tmp[1] = 0x01;
				s_tmp[2] = ++APP_StallLock_StrPar.WarningCount;
				for(cnt=0;cnt<10;cnt++)
				{
					s_tmp[cnt+3] = 0x00;
				}
				s_tmp[3] = HAL_adc_StrPar.WorkVoltgeGrade;
				s_tmp[12] = DEV_TypeClass;
				
				//PRO_spider_BuildCMDForPar(CMD_TYPE_TRFER,TRFER_CMD_SENDSTATUS,s_tmp,17);
				//APP_SPIDER.DealSendCmdToSpiderAddr = 1;
                                
                                if(push_data_into_ring_buffer(s_tmp,13) == 0)
                                {
                                  printf("\r\n��%s�����ζ�������\r\n",__FUNCTION__);
                                }
			}
			APP_StallLock_StrPar.WarningTimCnt = WARNINGTIMECNT;
		}
	}
}


static uint8_t pop_data_from_ring_buffer(void)
{
  if(g_ring_buffer_for_stalllock.numbers == 0)
  {
	return 0;
  }
//����һ�黷�����ݵ�����������
  memcpy((void *)g_temporary_buffer_for_stalllock.buffer,(const void *)g_ring_buffer_for_stalllock.buffer[g_ring_buffer_for_stalllock.rear],g_ring_buffer_for_stalllock.length[g_ring_buffer_for_stalllock.rear]);
//���Ƹ������ݵ����ݳ���
  g_temporary_buffer_for_stalllock.length = g_ring_buffer_for_stalllock.length[g_ring_buffer_for_stalllock.rear];
//��ո��黷������
  memset((void *)g_ring_buffer_for_stalllock.buffer[g_ring_buffer_for_stalllock.rear],0,THE_LENGTH_OF_FRAME);
//�����黷�����ݵ����ݳ������
  g_ring_buffer_for_stalllock.length[g_ring_buffer_for_stalllock.rear] = 0;
//��ȡ�������ݵ�ָ���1
  g_ring_buffer_for_stalllock.rear++;
//��֤��ȡ�������ݵ�ָ��ֵ��������Χ
  g_ring_buffer_for_stalllock.rear %= NODE_NUMBERS_FOR_STALLLOCK;//��֤g_ring_buffer_for_stalllock.rear��0~9֮��
//���������������ݵ�������1
  g_ring_buffer_for_stalllock.numbers--;

  return 1;
}


uint8_t push_data_into_ring_buffer(uint8_t *buf,uint8_t len)
{
  if((buf == NULL) || (g_ring_buffer_for_stalllock.numbers == NODE_NUMBERS_FOR_STALLLOCK))
  {
    return 0;
  }
  
  memcpy((void *)g_ring_buffer_for_stalllock.buffer[g_ring_buffer_for_stalllock.head],(const void *)buf,len);
  g_ring_buffer_for_stalllock.length[g_ring_buffer_for_stalllock.head] = len;
  
  g_ring_buffer_for_stalllock.head++;
  g_ring_buffer_for_stalllock.head %= NODE_NUMBERS_FOR_STALLLOCK;
  g_ring_buffer_for_stalllock.numbers++;
  
  return 1;
}


void main_stalllock(void)
{/*������������*/
    if((g_stalllock_control_struct.perform_status == STALLLOCK_IDLE) && (pop_data_from_ring_buffer() == 1)){//pop_data_from_ring_buffer���ƴ������뻺������һ������,����������
	//״̬����&&�����������������ݣ������������һ�����ݸ��Ƶ����������У���ո��黷������
		start_send_U_package();//����U��
	}
}


void start_send_U_package(void)
{
  g_stalllock_control_struct.perform_status = STALLLOCK_ACTIVE;//����״̬æ
  //
  g_stalllock_control_struct.timer = 0;
  /*�ϳ�����U��*/
  PRO_spider_BuildCMDForPar(CMD_TYPE_TRFER,TRFER_CMD_SENDUPACK,g_temporary_buffer_for_stalllock.buffer,g_temporary_buffer_for_stalllock.length);
	//�޸ķ���spider���ݵ�ָ��ֵ
  APP_SPIDER.DealSendCmdToSpiderAddr = 1;
}


void halt_send_U_package(void)
{
  //HAL_eeprom_WriteData(VEHICLE_EEPADDR,&g_temporary_buffer_for_screen.buffer[1],2);
  
  memset((void *)&g_temporary_buffer_for_stalllock,0,sizeof(Temporary_Buffer_For_StallLock));
  memset((void *)&g_stalllock_control_struct,0,sizeof(StallLock_Control_Struct));
}


uint8_t get_stalllock_perform_status(void)
{
  return g_stalllock_control_struct.perform_status;
}


void stalllock_delay(void)
{
  if(g_stalllock_control_struct.perform_status == STALLLOCK_ACTIVE)
  {
    g_stalllock_control_struct.timer++;
    if(g_stalllock_control_struct.timer > STALLLOCK_RESEND_MAX_TIME)
    {
      start_send_U_package();
    }
  }
}










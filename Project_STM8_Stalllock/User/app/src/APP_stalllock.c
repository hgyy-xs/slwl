#include "main.h"


StallLock_STRUCT  APP_StallLock_StrPar;


static Ring_Buffer_For_StallLock		g_ring_buffer_for_stalllock      = {0};
static Temporary_Buffer_For_StallLock	g_temporary_buffer_for_stalllock = {0};
StallLock_Control_Struct				g_stalllock_control_struct       = {0};


/*
车位锁引脚初始化
*/
void APP_stalllock_GPIOInit(void)
{
    //k1/k2/k3 脚初始化
    GPIO_Init(K_GPIO,K1_GPIO_PIN,GPIO_Mode_Out_PP_Low_Fast);//k1 输出低电平
    GPIO_Init(K_GPIO,K2_GPIO_PIN,GPIO_Mode_In_PU_No_IT);//k2/k3为输入引脚
    GPIO_Init(K_GPIO,K3_GPIO_PIN,GPIO_Mode_In_PU_No_IT);	
    
    //电机正反转引脚 初始化全拉低
    GPIO_Init(MOT_GPIO,MOT_GPIO_PIN1,GPIO_Mode_Out_PP_Low_Fast);//正转
    GPIO_Init(MOT_GPIO,MOT_GPIO_PIN2,GPIO_Mode_Out_PP_Low_Fast);//反转
    
    //报警信号输出脚  拉低
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
        
//--------------正常使用
	if(APP_SPIDER.Fast_Order5Flag == TRUE)//下降//接收快捷令5
	{
		if((APP_StallLock_StrPar.K2_value == 0 && APP_StallLock_StrPar.K3_value == 0)\
		|| (APP_StallLock_StrPar.K2_value == 8 && APP_StallLock_StrPar.K3_value == 0))//90度状态  或者 大于0度小于90度 开始反转
	 	{
			MOT_PIN1_LOW();
			MOT_PIN2_LOW();

	 		MOT_PIN1_LOW();
			MOT_PIN2_HIGH();
			APP_StallLock_StrPar.MontorTurnTimFlag = TRUE;
			
	 	}
		else if(APP_StallLock_StrPar.K2_value == 8 && APP_StallLock_StrPar.K3_value == 4)//到达0度，停止转动
		{
			APP_SPIDER.Fast_Order5Flag = FALSE;
			APP_StallLock_StrPar.MontorTurnTimFlag = FALSE;         
			MOT_PIN1_LOW();
			MOT_PIN2_LOW();
              //APP_StallLock_StrPar.MontorTurnTimMaxcount = MONTORTURNTIMMAXCNT;          
			if(Main_StrPar.SpidOrH3v4Flag != FALSE)
			{
				//上报此时车位锁状态 ，数据准备
				s_tmp[0] = 0x0A;
				for(cnt=0;cnt<12;cnt++)
				{
					s_tmp[cnt+1] = 0x00;
				}
				s_tmp[3] = HAL_adc_StrPar.WorkVoltgeGrade;
				s_tmp[12] = DEV_TypeClass;
				
				//PRO_spider_BuildCMDForPar(CMD_TYPE_TRFER,TRFER_CMD_SENDSTATUS,s_tmp,17);
				//APP_SPIDER.DealSendCmdToSpiderAddr = 1;

                if(push_data_into_ring_buffer(s_tmp,13) == 0)//将数据放入环形数据,满了返回0
                {
                  printf("\r\n【%s】环形队列已满\r\n",__FUNCTION__);
                }
			}
		}
	 }
	 else if(APP_SPIDER.Fast_Order6Flag == TRUE)		//上升//快捷令6
	 {
	 	if((APP_StallLock_StrPar.K2_value == 8 && APP_StallLock_StrPar.K3_value == 4)\
			|| (APP_StallLock_StrPar.K2_value == 8 && APP_StallLock_StrPar.K3_value == 0))	//0度状态 或者 大于0度小于90度    开始正转
	 	{
			MOT_PIN1_LOW();
			MOT_PIN2_LOW();
			
	 		MOT_PIN1_HIGH();
			MOT_PIN2_LOW();
			APP_StallLock_StrPar.MontorTurnTimFlag = TRUE;  
			//APP_StallLock_StrPar.MontorTurnTimMaxcount = MONTORTURNTIMMAXCNT;
	 	}
		else if(APP_StallLock_StrPar.K2_value == 0 && APP_StallLock_StrPar.K3_value == 4)	//大于90度，开始反转
		{
			MOT_PIN1_LOW(); 
			MOT_PIN2_HIGH();
			APP_StallLock_StrPar.MontorTurnTimFlag = TRUE;  
			//APP_StallLock_StrPar.MontorTurnTimMaxcount = MONTORTURNTIMMAXCNT;
		}
		else if(APP_StallLock_StrPar.K2_value == 0 && APP_StallLock_StrPar.K3_value == 0)	//到达90度，停止转动
		{
		APP_StallLock_StrPar.MontorTurnTimFlag = FALSE;  
			APP_SPIDER.Fast_Order6Flag = FALSE;
			MOT_PIN1_LOW();
			MOT_PIN2_LOW();
			if(Main_StrPar.SpidOrH3v4Flag != FALSE)
			{
				//上报此时车位锁状态
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
                                  printf("\r\n【%s】环形队列已满\r\n",__FUNCTION__);
                                }
			}
		}
	 }
//------------强行在90度状态下，按下车位锁(按下0~90度)
	else
	{
		if(APP_StallLock_StrPar.K2_CacheValue == 0 && APP_StallLock_StrPar.K3_CacheValue == 0 \
			&& APP_StallLock_StrPar.K2_value == 8 && APP_StallLock_StrPar.K3_value == 0  \
			&& APP_StallLock_StrPar.NoCmdPressFlag1 == FALSE)
		{
			//开始正转
			APP_StallLock_StrPar.NoCmdPressFlag1 = TRUE;

			APP_StallLock_StrPar.WarningCount ++;
			APP_StallLock_StrPar.WarningFlag = TRUE;
			APP_StallLock_StrPar.WarningTimCnt = WARNINGTIMECNT;
			//报警,mcu输出报警信号，并上报报警信号，警报如果一直未解除，每隔一段时间上报一次
			WARNING_LED_MAKE();
			WARNING_BEEP_MAKE();

			APP_StallLock_StrPar.MontorTurnTimMaxcount = MONTORTURNTIMMAXCNT;
			if(Main_StrPar.SpidOrH3v4Flag != FALSE)
			{
				//上报此时车位锁状态
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
                                  printf("\r\n【%s】环形队列已满\r\n",__FUNCTION__);
                                }
			}
		}
		
		if(APP_StallLock_StrPar.NoCmdPressFlag1 == TRUE)
		{
			if(APP_StallLock_StrPar.K2_value == 8 && APP_StallLock_StrPar.K3_value == 0)		//大于0度小于90度
			{
				MOT_PIN1_LOW();
				MOT_PIN2_HIGH();
			}
			else if(APP_StallLock_StrPar.K2_value == 8 && APP_StallLock_StrPar.K3_value == 4)	//0度状态，停止转动，并反转
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
                        
			if(APP_StallLock_StrPar.K2_value == 0 && APP_StallLock_StrPar.K3_value == 0)	//回到90度状态，停止转动
			{
				APP_StallLock_StrPar.NoCmdFlag2 = FALSE;
				//报警解除，mcu停止输出报警信号
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
                                          printf("\r\n【%s】环形队列已满\r\n",__FUNCTION__);
                                        }
				}
				APP_StallLock_StrPar.WarningFlag = FALSE;
				APP_StallLock_StrPar.WarningCount = 0;

				MOT_PIN1_LOW();
				MOT_PIN2_LOW();
			}
		}
		//判断警告是否需要继续
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
                                  printf("\r\n【%s】环形队列已满\r\n",__FUNCTION__);
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
//复制一组环形数据到传输数据中
  memcpy((void *)g_temporary_buffer_for_stalllock.buffer,(const void *)g_ring_buffer_for_stalllock.buffer[g_ring_buffer_for_stalllock.rear],g_ring_buffer_for_stalllock.length[g_ring_buffer_for_stalllock.rear]);
//复制该组数据的数据长度
  g_temporary_buffer_for_stalllock.length = g_ring_buffer_for_stalllock.length[g_ring_buffer_for_stalllock.rear];
//清空该组环形数据
  memset((void *)g_ring_buffer_for_stalllock.buffer[g_ring_buffer_for_stalllock.rear],0,THE_LENGTH_OF_FRAME);
//将该组环形数据的数据长度清空
  g_ring_buffer_for_stalllock.length[g_ring_buffer_for_stalllock.rear] = 0;
//读取环形数据的指向加1
  g_ring_buffer_for_stalllock.rear++;
//保证读取环形数据的指向值不超过范围
  g_ring_buffer_for_stalllock.rear %= NODE_NUMBERS_FOR_STALLLOCK;//保证g_ring_buffer_for_stalllock.rear在0~9之间
//环形数组中有数据的组数减1
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
{/*空闲且有数据*/
    if((g_stalllock_control_struct.perform_status == STALLLOCK_IDLE) && (pop_data_from_ring_buffer() == 1)){//pop_data_from_ring_buffer复制串口输入缓冲区的一条数据,到传输数据
	//状态空闲&&环形数组里面有数据，将环形数组的一组数据复制到传输数组中，清空该组环形数据
		start_send_U_package();//发送U包
	}
}


void start_send_U_package(void)
{
  g_stalllock_control_struct.perform_status = STALLLOCK_ACTIVE;//设置状态忙
  //
  g_stalllock_control_struct.timer = 0;
  /*合成上行U包*/
  PRO_spider_BuildCMDForPar(CMD_TYPE_TRFER,TRFER_CMD_SENDUPACK,g_temporary_buffer_for_stalllock.buffer,g_temporary_buffer_for_stalllock.length);
	//修改发送spider数据的指向值
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










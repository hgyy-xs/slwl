
#include "main.h"

ADC_STRUCT  HAL_adc_StrPar;


void HAL_adc_ValueInit(void)
{
	HAL_adc_StrPar.DataTmp = 0;
	HAL_adc_StrPar.Datacnt = 0;
	HAL_adc_StrPar.WorkVoltgeGrade = 0;

	HAL_adc_StrPar.CheckWorkVoltgeTimeCount = 0;
	HAL_adc_StrPar.CheckWorkVoltgeFlag = FALSE;
}


void HAL_ADC_Init(void)
{
  u8 idx = 0;
  
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);//ʹ��ADCʱ�� 
  
  ADC_DeInit(ADC1);
  
  GPIO_Init(GPIOA,GPIO_Pin_4,GPIO_Mode_In_FL_No_IT);//��������
  
  for (idx = 0;idx < 50;idx++);//adc�ϵ���Ҫһ��ʱ��
  
  ADC_VrefintCmd(ENABLE);//ʹ���ڲ��ο���ѹ
  
  //����ת��ģʽ
  ADC_Init(ADC1, ADC_ConversionMode_Continuous,ADC_Resolution_12Bit, ADC_Prescaler_2);
  
  // ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_192Cycles);
  ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels,ADC_SamplingTime_192Cycles);
  
  ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, ENABLE);//�ڲ��ο���ѹ
  //ADC_ChannelCmd(ADC1, ADC_Channel_2, ENABLE);//ͨ��2   PA4
  
  // disable SchmittTrigger for ADC_Channel_1, to save power 
  ADC_SchmittTriggerConfig(ADC1, ADC_Channel_2, DISABLE);//��ֹʩ���ش�����
}


/*
	��õ�ص�ѹ
*/
void APP_ADC_GetWorkVolGrade(void)
{
	u16 DataTmp;
        
	static u8 Flag = FALSE;
        
	if(Flag == FALSE)
	{
		Flag = TRUE;
		ADC_Cmd(ADC1,ENABLE);//��ADC
  		ADC_SoftwareStartConv(ADC1);//software trigger
	}
        
	DataTmp = ADC_GetConversionValue(ADC1);
        
	if(HAL_adc_StrPar.Datacnt != 0)
	{
		HAL_adc_StrPar.DataTmp += DataTmp;
	}
	
	HAL_adc_StrPar.Datacnt ++;
        
	if(HAL_adc_StrPar.Datacnt > 10)
	{
		HAL_adc_StrPar.AverDataTmp = HAL_adc_StrPar.DataTmp / 10;
                
		printf("AverDataTmp:%d\r\n",HAL_adc_StrPar.AverDataTmp);
                
		//�жϻ�õ�ƽ��ֵ���õ���ѹ�ȼ�
		if(HAL_adc_StrPar.AverDataTmp >= 1032 )
		{
			HAL_adc_StrPar.WorkVoltgeGrade = 4;
		}
		else if(HAL_adc_StrPar.AverDataTmp >= 990)
		{
			HAL_adc_StrPar.WorkVoltgeGrade = 3;
		}
		else if(HAL_adc_StrPar.AverDataTmp >=  830)
		{
			HAL_adc_StrPar.WorkVoltgeGrade = 2;
		}
		else if(HAL_adc_StrPar.AverDataTmp >= 760)
		{
			HAL_adc_StrPar.WorkVoltgeGrade = 1;
		}
		else//��ѹ����
		{
			HAL_adc_StrPar.WorkVoltgeGrade = 0;
		}
		
		HAL_adc_StrPar.Datacnt = 0;
		HAL_adc_StrPar.DataTmp = 0;
		HAL_adc_StrPar.AverDataTmp = 0;				//�����õ�ֵ
                
		HAL_adc_StrPar.CheckWorkVoltgeFlag = FALSE;		//ֹͣ���
		
		HAL_adc_StrPar.CheckWorkVoltgeTimeCount = ADC_CHECK_TIMCNT;//����ʱ���¿�ʼ,ÿ���һ,����0ʱȥ����ڲ��ο���ѹֵ
                
		ADC_Cmd(ADC1,DISABLE);//�ر�ADC
                
		Flag = FALSE;
	}
}

#include "main.h"
#include "stm8l15x_tim2.h"


Tim_STRUCT  HalTim4_StrPar,HalTim2_StrPar;


void HAL_TIM4_config(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
  
  // Time base configuration   1ms timer  (1/(16M/128)) * 125 = 1ms
  TIM4_TimeBaseInit(TIM4_Prescaler_128,125);
  
  TIM4_ITConfig(TIM4_IT_Update,ENABLE);
  
  TIM4_Cmd(ENABLE);
}


void HAL_TIM2_config(void){
	CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);	//��TIM2��ʱ��

	TIM2_TimeBaseInit(TIM2_Prescaler_2,TIM2_CounterMode_Up,79);//����TIM2���ϼ��� reload after 10us.

	TIM2_ARRPreloadConfig(ENABLE);		//ʹ��TIM2��ʱ���ɸ���ʱ��
  
	TIM2_ITConfig(TIM2_IT_Update,ENABLE);	//ʹ�����ϼ�����TIM2�ж�

	TIM2_Cmd(ENABLE);		//ʹ��TIM
}
/*
 * �� �� ��: HAL_TIM4_Value
 * ��    ��: TIM4�ṹ�帳��ʼֵ
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */
void HAL_TIM4_Value(void){
	HalTim4_StrPar.TIM_count = 0;			//�����ʱ������
	HalTim4_StrPar.TIM_T200mSFlag = FALSE;	//���200ms��־λ
	HalTim4_StrPar.TIM_T500mSFlag = FALSE;	//���500ms��־λ
	HalTim4_StrPar.TIM_T1SFlag = FALSE;	//���1s��־λ
	HalTim4_StrPar.TIM_T3SFlag = FALSE;	//���3s��־λ
	//��ʱ��2������ʼ��
	HalTim2_StrPar.TIM_count = 0;
}


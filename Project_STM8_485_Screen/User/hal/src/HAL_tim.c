#include "main.h"


Tim_STRUCT  HalTim4_StrPar;

/*
 * �� �� ��: DEVICE_init
 * ��    ��: ��ʱ��4��ʼ��
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */
void HAL_TIM4_config(void){
	CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);	//ʹ��TIM4ʱ��
	TIM4_TimeBaseInit(TIM4_Prescaler_128,125);					//���ö�ʱ��Ƶ�ʺͼ���ֵ
	TIM4_ITConfig(TIM4_IT_Update,ENABLE);						//ʹ�ܶ�ʱ���ж�
	TIM4_Cmd(ENABLE);											//ʹ�ܶ�ʱ��
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
}


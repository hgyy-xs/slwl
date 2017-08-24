

#ifndef _HAL_TIM_H_
#define _HAL_TIM_H_


//-----------------------------------------------ȫ�ֱ���
typedef struct 
{
	//---��ʱ��4�ļ����ͱ�־
  u16 TIM_count;
  bool TIM_T200mSFlag;
  bool TIM_T500mSFlag;
  u8 TIM_T1SFlag;
  u8 TIM_T3SFlag;

}Tim_STRUCT;

extern Tim_STRUCT  HalTim4_StrPar,HalTim2_StrPar;



void HAL_TIM4_config(void);
void HAL_TIM4_Value(void);

void HAL_TIM2_config(void);

#endif
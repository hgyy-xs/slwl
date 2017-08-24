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
	CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);	//打开TIM2的时钟

	TIM2_TimeBaseInit(TIM2_Prescaler_2,TIM2_CounterMode_Up,79);//设置TIM2向上计数 reload after 10us.

	TIM2_ARRPreloadConfig(ENABLE);		//使能TIM2定时器可更新时钟
  
	TIM2_ITConfig(TIM2_IT_Update,ENABLE);	//使能向上计数的TIM2中断

	TIM2_Cmd(ENABLE);		//使能TIM
}
/*
 * 函 数 名: HAL_TIM4_Value
 * 功    能: TIM4结构体赋初始值
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 其    他: 
 */
void HAL_TIM4_Value(void){
	HalTim4_StrPar.TIM_count = 0;			//清除定时器计数
	HalTim4_StrPar.TIM_T200mSFlag = FALSE;	//清除200ms标志位
	HalTim4_StrPar.TIM_T500mSFlag = FALSE;	//清除500ms标志位
	HalTim4_StrPar.TIM_T1SFlag = FALSE;	//清除1s标志位
	HalTim4_StrPar.TIM_T3SFlag = FALSE;	//清除3s标志位
	//定时器2变量初始化
	HalTim2_StrPar.TIM_count = 0;
}


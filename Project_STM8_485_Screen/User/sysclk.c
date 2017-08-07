#include "main.h"

/*
 * �� �� ��: SYSCLK_HSI_Init
 * ��    ��: ��ʼ��ϵͳʱ��ΪHSI
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */
void SYSCLK_HSI_Init(void)
{
  CLK_HSICmd(ENABLE);
  CLK_LSICmd(ENABLE);
  
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
  
  CLK_MainRegulatorCmd(ENABLE);
  
  CLK_HaltConfig(CLK_Halt_FastWakeup, DISABLE);
  CLK_HaltConfig(CLK_Halt_SlowWakeup, ENABLE);
  
  while(PWR_GetFlagStatus(PWR_FLAG_VREFINTF) != SET);
  
  PWR_FastWakeUpCmd(ENABLE);
  PWR_UltraLowPowerCmd(ENABLE);
}

/*
 * �� �� ��: SYSCLK_HSE_Init
 * ��    ��: ��ʼ��ϵͳʱ��ΪHSE
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */
void SYSCLK_HSE_Init(void){
	CLK->ECKCR |= (1<<0);				//��HSEʱ��
	while(CLK->ECKCR & (1<<1) == 0);	//ѭ��LSI����δ׼����״̬
	while(CLK->SWCR & (1<<0) == 1);	//ѭ��ʱ��δ�л���״̬
	CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);	//���÷�Ƶϵ��
	CLK->SWR = 0X04;					//����Ŀ��ϵͳʱ��ΪHSE
	CLK->SWCR |= (1<<1);				//��ʼ�л�ʱ��
	while(CLK->SWCR & (1<<0) == 1);	//ѭ��ʱ��δ�л���״̬
	CLK->ICKCR &= ~(1<<0);				//�ر�HSIʱ��
}


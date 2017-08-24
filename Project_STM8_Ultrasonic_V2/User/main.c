#include "main.h"

Main_STRUCT  Main_StrPar;

/*
 * �� �� ��: DEVICE_init
 * ��    ��: ��Χ�豸��ʼ��
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */    
static void DEVICE_init(void){
	SYSCLK_HSE_Init();		//ϵͳʱ�ӳ�ʼ�� 16MHz HSE
	HAL_TIM4_config();		//��ʱ��4���� 1ms�ж�
	HAL_LED_config();		//LED��ʼ�� PD7 PD6    �͵�ƽ��Ч
	HAL_USART_config();		//���ڳ�ʼ�� USART1:debug PC2 PC3	USART2:485 PE3 PE4	  USART3:STM PE6 PE7
	HAL_KEY_config();		//KEY��ʼ�� PA7	�͵�ƽ��Ч
	HAL_GPIO_config();		//STM��λ PC4	STM�ж� PE1
	HAL_eeprom_Init();
	//IWDG_config();			//ʹ�ܶ������Ź�
	enableInterrupts();		//ʹ�������ж�
	HAL_LED_AllOff();		//�ر�����LED
}

/*
 * �� �� ��: WORK_VALUE_init
 * ��    ��: �ṹ�帳ֵ��ʼ��
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */
static void WORK_VALUE_init(void){
	Main_StrPar.Init_LED_ShakeFlag = TRUE;
	Main_StrPar.Init_LED_AlternateShakeFlag = FALSE;
	Main_StrPar.Init_LED1_ShakeFlag=FALSE;
	
	HAL_KEY_Value();		//�����ṹ�帳ֵ
	HAL_TIM4_Value();		//��ʱ��4�ṹ�帳ֵ 
	APP_spider_ValueInit(); //spider�ṹ�帳ֵ
	APP_Ultrasonic_ValueInit();
	// APP_Camera_ValueInit();

	//APP_LED_ValueInit();	//�������ճ�λ����ʼ��
}

/*
 * �� �� ��: main
 * ��    ��: ��λ��Ƶ������
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */

void main(void){
	DEVICE_init();					//�����ʼ�� 
	WORK_VALUE_init();				//�ṹ�帳ֵ��ʼ��
	APP_nor_mode_SysNormalMode();	//ϵͳ����ģʽ
}


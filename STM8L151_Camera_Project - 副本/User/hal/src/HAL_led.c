#include "main.h"

/*
 * �� �� ��: HAL_LED_config
 * ��    ��: LED���ų�ʼ��
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */
void HAL_LED_config(void){
	GPIO_Init(LED_GPIO, LED_GPIO_PIN, GPIO_Mode_Out_PP_High_Fast);//�ߵ�ƽ�����������
}

/*
 * �� �� ��: HAL_LED_AllOff
 * ��    ��: �ر����е�LED
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */
void HAL_LED_AllOff(void){
	LED_OFF();
}


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
	GPIO_Init(LED_GPIO, LED1_GPIO_PIN, GPIO_Mode_Out_PP_High_Fast);//�ߵ�ƽ�����������
	GPIO_Init(LED_GPIO, LED2_GPIO_PIN, GPIO_Mode_Out_PP_High_Fast);//�ߵ�ƽ�����������
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
	LED1_OFF();
	LED2_OFF();
}
/*
 * �� �� ��: HAL_LED_AlternateFlash
 * ��    ��: LED������˸
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */
void HAL_LED_AlternateFlash(void){
	static u8 flag=FALSE;
	if(flag==FALSE){	
		flag=TRUE;
		LED1_ON();
		LED2_OFF();
	}
	else if(flag==TRUE)
		{
			flag=FALSE;
			LED2_ON();
			LED1_OFF();
		}
}


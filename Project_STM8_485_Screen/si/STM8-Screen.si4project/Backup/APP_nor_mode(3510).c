#include "main.h"

/*
 * �� �� ��: APP_nor_mode_RealTask
 * ��    ��: ����������
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */
static void APP_nor_mode_RealTask(void){
	feed_IWDG();				//ι��
	HAL_KEY_Scan();			//����ɨ��
	HAL_KEY_deal();			//��������
	APP_spider_uartDeal();		//spider���ڴ���
	APP_Spider_dealSendCmdToSpider();	//�������spider����
	APP_spider_IEPIN_deal();	//����spider���ж��¼�
	APP_Camera_uartDeal();		//spider���ڴ���
	APP_Camera_dealSendCmdToSpider();	//�������spider����
}

/*
 * �� �� ��: APP_nor_mode_FixedTask
 * ��    ��: ϵͳ��ʱ��������
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */
void APP_nor_mode_FixedTask(void){
	/**************** ��ʱ1ms��Ҫִ�е����� ****************/

	/*************** ��ʱ200ms��Ҫִ�е����� ***************/

	/*************** ��ʱ500ms��Ҫִ�е����� ***************/

	/**************** ��ʱ1s��Ҫִ�е����� *****************/
	
}

/*
 * �� �� ��: APP_nor_mode_SysNormalMode
 * ��    ��: ϵͳ��������ģʽ
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */
void APP_nor_mode_SysNormalMode(void){
		//while(1){
		//if(APP_SN_Init()){
			//break;
			//printf("have SN");
		//}
		//else{
		//	printf("havn`t SN");
		//}
	//}
	APP_spider_init();			//spiderģ���ʼ��
	Main_StrPar.Init_LED_ShakeFlag = FALSE;	//��ʼ�����
	APP_Spider_ReadyOnNet();	//
	printf("\r\nall init success\r\n");
	while(1){
		APP_nor_mode_RealTask();	//ʵʱ������
		APP_nor_mode_FixedTask();
	}
}


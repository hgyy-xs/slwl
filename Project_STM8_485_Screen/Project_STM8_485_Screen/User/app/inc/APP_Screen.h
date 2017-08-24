#ifndef __APP_SCREEN_H
#define __APP_SCREEN_H

#include "main.h"
#include <string.h>

//--------STM8L151C8�ڲ�EEPROM�洢�ճ�λ���ĵ�ַ
#define  VEHICLE_EEPADDR 0x01010

#define Screen_Ack_Lenght 13
#define Screen_Tx_Lenght 140

#define ACK_PKG_CLASS 1<<3	//Ӧ�����ݰ����� 
#define Screen_Addr 0x05 	//��������ַ

typedef struct{
	u8	Data[Screen_Ack_Lenght];		//����
	u16	Status;							//״̬
}Screen_Ack_Struct;						//Screen to DDA receipt 

typedef struct{
	u8	Tx_Data[Screen_Tx_Lenght];		//ת������
	u16	Status;						//״̬
}Screen_Struct;						//to Screen

typedef struct
{
	 u16 vehicle_count;	//���೵λ����

}APP_LED_STRUCT;

extern APP_LED_STRUCT APP_LED;
extern Screen_Ack_Struct APP_Screen_Ack;
extern Screen_Struct	APP_Screen;

void APP_LED_ValueInit(void);
void APP_LedShow_init(void);
void APP_LED_ShowDataReady(void);
void APP_LEDSetShow(void);
void APP_Screen_dealSendCmdToSpider(void);
void APP_Deal_U_Package_From_GW(void);
void APP_Deal_Vehicle_By_Key(void);
// void APP_LEDSetShow(void);
#endif


#ifndef _APP_LED_H_
#define _APP_LED_H_


//--------�����Ƶ�ַ
#define Led_showAddr   0X05

//--------STM8L151C8�ڲ�EEPROM�洢�ճ�λ���ĵ�ַ
#define  VEHICLE_EEPADDR 0x01010

typedef struct
{
	 u16 vehicle_count;	//���೵λ����

}APP_LED_STRUCT;

extern APP_LED_STRUCT APP_LED;


void APP_LED_ValueInit(void);
void APP_LedShow_init(void);
void APP_LED_ShowDataReady(void);
void APP_LEDSetShow(void);

#endif


































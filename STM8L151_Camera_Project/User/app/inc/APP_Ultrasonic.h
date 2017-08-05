

#ifndef _APP_ULTRASONIC_H_
#define _APP_ULTRASONIC_H_





//-------------------------------����
typedef struct{
  u8  InitOKFlag;				// ������ ��ʼ���ɹ���־
  u8  InitOverTimeCnt;		// ������ ��ʼ����ʱ����

  u8 SendCmdOverTimeCnt;		//���������������ʱ

  u8 InitDealAddr;			//��ʼ������
  u16 UartRevOverCnt;        // ��ʱ����
   u8 RevBuffCashe;	//�����ϴν�������

  u8 LocalAddr;			//���������ص�ַ
  u8 LedShakeFlag;			//ָʾ����˸��־ 0:����˸  1: ��˸
 
}ULTRAS_STRUCT;

extern ULTRAS_STRUCT  APP_ULTRASONIC;



void APP_Ultrasonic_ValueInit(void);
void APP_Ultrasonic_ConfigInit(void);
void APP_Ultrasonic_UsartDealForInit(void);
void APP_Ultrasonic_UartRxDealforInit(void);
void APP_Ultrasonic_FixedPollState(void);
void APP_Ultrasonic_UartRxDealforWork(void);



#endif




































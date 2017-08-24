#ifndef _APP_ULTRASONIC_H_
#define _APP_ULTRASONIC_H_


#include <string.h>


#define ULTRASONIC_MAX_BUFFER_LENGTH 50
#define ULTRASONIC_RESEND_MAX_TIME   30000//30s.


typedef enum
{
  ULTRASONIC_IDLE,
  ULTRASONIC_ACTIVE
}Ultrasonic_Perform_Status_enum;


//--------------------------����
typedef struct{
  u8  InitOKFlag;		// ������ ��ʼ���ɹ���־
  u8  InitOverTimeCnt;		// ������ ��ʼ����ʱ����
  
  u16 SendCmdOverTimeCnt;	//���������������ʱ
  
  u8 InitDealAddr;		//��ʼ������
  u16 UartRevOverCnt;           // ��ʱ����
  u8 RevBuffCashe;	        //�����ϴν�������
  
  u8 LocalAddr;			//���������ص�ַ
  u8 LedShakeFlag;		//ָʾ����˸��־ 0:����˸  1: ��˸
  u16 LedShakeTimCnt;		//ָʾ����˸��ʱ
}ULTRAS_STRUCT;


typedef struct
{
  uint8_t buf[ULTRASONIC_MAX_BUFFER_LENGTH];
  uint8_t len;
}Ultrasonic_Temporary_Buffer;


typedef struct
{
  uint8_t perform_status;
  volatile uint32_t timer;
}Ultrasonic_Control_Struct;


extern ULTRAS_STRUCT  APP_ULTRASONIC;


void APP_Ultrasonic_ValueInit(void);
void APP_Ultrasonic_ConfigInit(void);
void APP_Ultrasonic_UsartDealForInit(void);
void APP_Ultrasonic_UartRxDealforInit(void);
void APP_Ultrasonic_FixedPollState(void);
void APP_Ultrasonic_UartRxDealforWork(void);


void start_send_U_package(void);
void halt_send_U_package(void);
void ultrasonic_delay(void);



#endif


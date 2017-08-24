#ifndef _APP_STALLLOCK_H_
#define _APP_STALLLOCK_H_


#include <string.h>


#define WARNINGTIMECNT  10		//60s�������ϱ�


#define K_GPIO GPIOB


#define K1_GPIO_PIN GPIO_Pin_4
#define K2_GPIO_PIN GPIO_Pin_3
#define K3_GPIO_PIN GPIO_Pin_2


//���ģ�������� K1
#define K1_OFF()  GPIO_ResetBits(K_GPIO,K1_GPIO_PIN)
#define K1_ON() GPIO_SetBits(K_GPIO,K1_GPIO_PIN)


//��ȡK2��K3��ֵ
#define K2() GPIO_ReadInputDataBit(K_GPIO,K2_GPIO_PIN)
#define K3() GPIO_ReadInputDataBit(K_GPIO,K3_GPIO_PIN)


//----�������ת
/*
	pin1          pin2
��ת	0		0
��ת	0		1
��ת	1		0
������  1		1
*/
#define MOT_GPIO GPIOD
#define MOT_GPIO_PIN1 GPIO_Pin_0
#define MOT_GPIO_PIN2 GPIO_Pin_3


#define MOT_PIN1_HIGH()	GPIO_SetBits(MOT_GPIO,MOT_GPIO_PIN1)
#define MOT_PIN1_LOW()	GPIO_ResetBits(MOT_GPIO,MOT_GPIO_PIN1)
#define MOT_PIN2_HIGH()	GPIO_SetBits(MOT_GPIO,MOT_GPIO_PIN2)
#define MOT_PIN2_LOW()	GPIO_ResetBits(MOT_GPIO,MOT_GPIO_PIN2)


#define WARNING_LED_GPIO GPIOD
#define WARNING_LED_GPIO_PIN GPIO_Pin_1
#define WARNING_BEEP_GPIO GPIOD
#define WARNING_BEEP_GPIO_PIN GPIO_Pin_2


#define WARNING_LED_REMOVE()  		GPIO_ResetBits(WARNING_LED_GPIO,WARNING_LED_GPIO_PIN)
#define WARNING_LED_MAKE() 	              GPIO_SetBits(WARNING_LED_GPIO,WARNING_LED_GPIO_PIN)


#define WARNING_BEEP_MAKE()  		GPIO_SetBits(WARNING_BEEP_GPIO,WARNING_BEEP_GPIO_PIN)
#define WARNING_BEEP_REMOVE() 	       GPIO_ResetBits(WARNING_BEEP_GPIO,WARNING_BEEP_GPIO_PIN)
#define WARNING_BEEP_XOR()                 GPIO_ToggleBits(WARNING_BEEP_GPIO,WARNING_BEEP_GPIO_PIN);


//----���ת�����ʱ�����
#define MONTORTURNTIMMAXCNT 8


typedef struct
{
  u8 K2_value;		//��ù��ģ��K2��ֵ
  u8 K3_value;		//��ù��ģ��K3��ֵ

  u8 K2_CacheValue;	//������ģ��K2��ֵ
  u8 K3_CacheValue;	//������ģ��K3��ֵ

	u8 MontorTurnTimFlag;	//���ת��������־λ
  u8 MontorTurnTimMaxcount;//���ÿ��ת��ʱ�����

  u8 WarningFlag;	//�����־
  u8 WarningCount;	//�������
  u8 WarningTimCnt;	//����ʱ�����

  u8 NoCmdPressFlag1;	//û�����ǿ�а��³�λ�� �½���־
  u8 NoCmdFlag2;	//û�����ǿ�а��³�λ��  ������־
  u8 TimCnt;		//��ʱ���k2��k3ֵ
}StallLock_STRUCT;




#define NODE_NUMBERS_FOR_STALLLOCK	10
#define THE_LENGTH_OF_FRAME			30
#define STALLLOCK_RESEND_MAX_TIME		30000//30s.



typedef enum
{
  STALLLOCK_IDLE = 0,	//����
  STALLLOCK_ACTIVE
}StallLock_Perform_Status_enum;



typedef struct
{
  uint8_t buffer[THE_LENGTH_OF_FRAME];
  uint8_t length;
}Temporary_Buffer_For_StallLock;



typedef struct
{
  uint8_t  perform_status;
  volatile uint32_t timer;
}StallLock_Control_Struct;



typedef struct
{
  uint8_t buffer[NODE_NUMBERS_FOR_STALLLOCK][THE_LENGTH_OF_FRAME];	//��Ӧ�Ļ�������
  uint8_t length[NODE_NUMBERS_FOR_STALLLOCK];						//��Ӧ�������ݵĳ���
  uint8_t head;														//д�뻷������ָ��
  uint8_t rear;														//������������ָ��
  uint8_t numbers;													//������������
}Ring_Buffer_For_StallLock;


uint8_t push_data_into_ring_buffer(uint8_t *buf,uint8_t len);
void main_stalllock(void);
void start_send_U_package(void);
void halt_send_U_package(void);
uint8_t get_stalllock_perform_status(void);
void stalllock_delay(void);




extern StallLock_STRUCT  APP_StallLock_StrPar;
extern StallLock_Control_Struct       g_stalllock_control_struct;


void APP_stalllock_GPIOInit(void);
void APP_stalllock_StateDeal(void);


#endif


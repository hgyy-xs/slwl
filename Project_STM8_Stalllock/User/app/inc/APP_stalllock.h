#ifndef _APP_STALLLOCK_H_
#define _APP_STALLLOCK_H_


#include <string.h>


#define WARNINGTIMECNT  10		//60s，周期上报


#define K_GPIO GPIOB


#define K1_GPIO_PIN GPIO_Pin_4
#define K2_GPIO_PIN GPIO_Pin_3
#define K3_GPIO_PIN GPIO_Pin_2


//光电模块驱动脚 K1
#define K1_OFF()  GPIO_ResetBits(K_GPIO,K1_GPIO_PIN)
#define K1_ON() GPIO_SetBits(K_GPIO,K1_GPIO_PIN)


//读取K2和K3的值
#define K2() GPIO_ReadInputDataBit(K_GPIO,K2_GPIO_PIN)
#define K3() GPIO_ReadInputDataBit(K_GPIO,K3_GPIO_PIN)


//----马达正反转
/*
	pin1          pin2
不转	0		0
反转	0		1
正转	1		0
不允许  1		1
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


//----电机转动最大时间计数
#define MONTORTURNTIMMAXCNT 8


typedef struct
{
  u8 K2_value;		//获得光电模块K2的值
  u8 K3_value;		//获得光电模块K3的值

  u8 K2_CacheValue;	//缓存光电模块K2的值
  u8 K3_CacheValue;	//缓存光电模块K3的值

	u8 MontorTurnTimFlag;	//电机转动计数标志位
  u8 MontorTurnTimMaxcount;//电机每次转动时间计数

  u8 WarningFlag;	//警告标志
  u8 WarningCount;	//警告计数
  u8 WarningTimCnt;	//警告时间计数

  u8 NoCmdPressFlag1;	//没有命令，强行按下车位锁 下降标志
  u8 NoCmdFlag2;	//没有命令，强行按下车位锁  上升标志
  u8 TimCnt;		//定时输出k2和k3值
}StallLock_STRUCT;




#define NODE_NUMBERS_FOR_STALLLOCK	10
#define THE_LENGTH_OF_FRAME			30
#define STALLLOCK_RESEND_MAX_TIME		30000//30s.



typedef enum
{
  STALLLOCK_IDLE = 0,	//闲置
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
  uint8_t buffer[NODE_NUMBERS_FOR_STALLLOCK][THE_LENGTH_OF_FRAME];	//对应的环形数据
  uint8_t length[NODE_NUMBERS_FOR_STALLLOCK];						//对应环形数据的长度
  uint8_t head;														//写入环形数据指向
  uint8_t rear;														//读出环形数据指向
  uint8_t numbers;													//环形数据总数
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


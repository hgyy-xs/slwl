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


//--------------------------变量
typedef struct{
  u8  InitOKFlag;		// 超声波 初始化成功标志
  u8  InitOverTimeCnt;		// 超声波 初始化超时计数
  
  u16 SendCmdOverTimeCnt;	//向超声波发送命令计时
  
  u8 InitDealAddr;		//初始化处理
  u16 UartRevOverCnt;           // 超时计数
  u8 RevBuffCashe;	        //缓存上次接收数据
  
  u8 LocalAddr;			//超声波本地地址
  u8 LedShakeFlag;		//指示灯闪烁标志 0:不闪烁  1: 闪烁
  u16 LedShakeTimCnt;		//指示灯闪烁计时
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


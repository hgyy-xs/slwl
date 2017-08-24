

#ifndef _APP_ULTRASONIC_H_
#define _APP_ULTRASONIC_H_





//-------------------------------变量
typedef struct{
  u8  InitOKFlag;				// 超声波 初始化成功标志
  u8  InitOverTimeCnt;		// 超声波 初始化超时计数

  u8 SendCmdOverTimeCnt;		//向超声波发送命令计时

  u8 InitDealAddr;			//初始化处理
  u16 UartRevOverCnt;        // 超时计数
   u8 RevBuffCashe;	//缓存上次接收数据

  u8 LocalAddr;			//超声波本地地址
  u8 LedShakeFlag;			//指示灯闪烁标志 0:不闪烁  1: 闪烁
 
}ULTRAS_STRUCT;

extern ULTRAS_STRUCT  APP_ULTRASONIC;



void APP_Ultrasonic_ValueInit(void);
void APP_Ultrasonic_ConfigInit(void);
void APP_Ultrasonic_UsartDealForInit(void);
void APP_Ultrasonic_UartRxDealforInit(void);
void APP_Ultrasonic_FixedPollState(void);
void APP_Ultrasonic_UartRxDealforWork(void);



#endif




































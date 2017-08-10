#ifndef _APP_SPIDER_H_
#define _APP_SPIDER_H_


#define SPIDER_USART     USER_USART3


//-------------------------------------------------------ҵ�����
#define DEV_TYPE         0x00
//-------------------------------------------------------���ķ������
#define DEV_Power_DC     0x00    
#define DEV_Power_BTY    0x0F
//-------------------------------------------------------ҵ������
#define DEV_TypeClass       0xD1    // ����������

//-------------------------------------------------------STU������λ
#define DEV_NonSensitiveByte1 0x00  // 0/1 : ����/������
#define DEV_NonSensitiveByte2 0x00  // 0/1 : ����/������



#define SPIDER_DLY_TIME 	        5  //spider������ʱ
#define MAIN_SPIDER_INIT_TIME	10// SPIDERģ���ܳ�ʼ��ʱ��35S



//---------------------SPIDER�ն�ģ����ΧӲ����λ����
#define  APP_SPIDER_HARDWARE_DISABLE()    SPIDER_TM_RESET_DISABLE()
#define  APP_SPIDER_HARDWARE_ENABLE()	SPIDER_TM_RESET_ENABLE()

// extern const static u8 SPIDER_WORK_DATA[42]; //37to42

//-------------------------------����
typedef struct{
  u8  InitOKFlag;				// spider ��ʼ���ɹ���־  TRUE : �ɹ�    FALSE :ʧ��
  u8  InitOverTimeCnt;		// spider��ʼ����ʱ����
  u8  StartDly;				// ϵͳ������ʱ��3s.
  bool IEPINOK_Flag;		//�ж��¼���־
  bool MF_Flag;  //��ȡģ����̬��־λ
  bool WP_Flag;  //��ȡ����������־λ
  bool SingleRegFlag;   //���ն�������־
  bool MultRegFlag;   //���ն�������־
  u8 InitDealAddr;
  bool UartRevOkFlag;        // ok
  bool UartRevOverFlag;        // ��ʱ��־
  u16  UartRevOverCnt;        // ��ʱ��־
  u8 SendToSpiderCmdCnt;		//���͸�Spider�������
  u8 DealSendCmdToSpiderAddr;//������Spider���������ַ
  u8 WakeupDealAddr;		//  ���Ѵ����ַ
  u16 WakeupTimeCnt;		// ���Ѷ�ʱ����

  //----------uart
	u16	TxUsartLeng;
	u8  TxUsartData[256];

	// ������־
	u8 FastCmdFlag;
	u8 FastCmdLEDShakeFlag;

	u8 Judge_Is_OnNetFlag;		//�Ƿ�������־  TRUE :�Ѿ�����    FALSE :û������ 
	u8 Judge_ReadyOnNetFlag;	//�Ƿ������������ģʽ    false:��������   true : ���ٽ���
	u8 Judge_SendOnNetCmdFlag;	//�ж��Ƿ������������־��true : ����  false : ֹͣ����
}SPIDER_STRUCT;


extern SPIDER_STRUCT  APP_SPIDER;


void APP_spider_UsartDealForInit(void);
void APP_spider_ValueInit(void);
void APP_spider_IEPIN_deal(void);
void APP_spider_init(void);
void APP_spider_uartDeal(void);
void APP_spider_FastCmdLEDshake(void);
void APP_Spider_dealSendCmdToSpider(void);
void APP_Spider_ReadyOnNet(void);


#endif
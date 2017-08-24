#ifndef _APP_SPIDER_H_
#define _APP_SPIDER_H_


#define SPIDER_USART     USER_USART3


#define SPIDER_DLY_TIME           5  //spider������ʱ
#define MAIN_SPIDER_INIT_TIME 10// SPIDERģ���ܳ�ʼ��ʱ��35S

//-------------------------------------------------------ҵ�����
#define DEV_TYPE         0x00
//-------------------------------------------------------���ķ������
#define DEV_Power_DC     0x00    
#define DEV_Power_BTY    0x0F
//-------------------------------------------------------ҵ������
#define DEV_TypeClass       0xC5    // ��λ��

//-------------------------------------------------------STU������λ
#define DEV_NonSensitiveByte1 0x00  // 0/1 : ����/������
#define DEV_NonSensitiveByte2 0x00  // 0/1 : ����/������

//-------------------------------------------------------�쳣�������   2 BYTE  0x12C == 300 
#define DEV_Anomaly_Detection_cycleByte1    0x2C
#define DEV_Anomaly_Detection_cycleByte2    0x01
//-------------------------------------------------------���¸澯��ֵ   1 BYTE
#define DEV_HigthTem_Warning          0x3C
//-------------------------------------------------------���¸澯��ֵ   1 BYTE
#define DEV_LowTem_Warning            0x0A
//-------------------------------------------------------��ѹ�澯��ֵ    2 BYTE
#define DEV_LowPress_Warning1         0x34
#define DEV_LowPress_Warning2         0x08
//-------------------------------------------------------һ��RF���书�ʵȼ�
#define DEV_RFTransmittedPower_Grade      0x07
//-------------------------------------------------------һ�����ն���������ʱ��
#define DEV_SFOnlyNetworking_TimeAllow1   0x3C
#define DEV_SFOnlyNetworking_TimeAllow2   0x00
//-------------------------------------------------------һ�����ն���������ʱ��
#define DEV_SFMultipleNetworking_TimeAllow1 0xB4
#define DEV_SFMultipleNetworking_TimeAllow2 0x00
//-------------------------------------------------------һ�����ն�������ʱʱ��
#define DEV_SFOnlyNetworking_TimeOut1     0x3C
#define DEV_SFOnlyNetworking_TimeOut2     0x00
//-------------------------------------------------------һ�����ն�������ʱʱ��
#define DEV_SFMultipleNetworking_TimeOut1   0x2C
#define DEV_SFMultipleNetworking_TimeOut2   0x01
//-------------------------------------------------------��������ʱ�� 2 BYTE    2 Min
#define DEV_Continuous_WorkingTimeByte1   0x1E
#define DEV_Continuous_WorkingTimeByte2   0x00 
//-------------------------------------------------------�Զ�����ʱ�� 4 BYTE
#define DEV_Auto_WakeUpTime1          0x1E
#define DEV_Auto_WakeUpTime2          0x00
#define DEV_Auto_WakeUpTime3          0x00
#define DEV_Auto_WakeUpTime4          0x00
//-------------------------------------------------------����U����ʱʱ��
#define DEV_SendUPacketOverTim1       0x0A
#define DEV_SendUPacketOverTim2       0x00
//-------------------------------------------------------����M����ʱʱ��
#define DEV_SendMPacketOverTim1       0x0A
#define DEV_SendMPacketOverTim2       0x00
//-------------------------------------------------------Ƶ���Զ�����ʹ��
#define DEV_AutoSearch              0x00
//

//---------------------SPIDER�ն�ģ����ΧӲ����λ����
#define  APP_SPIDER_HARDWARE_DISABLE()    SPIDER_TM_RESET_DISABLE()
#define  APP_SPIDER_HARDWARE_ENABLE()	SPIDER_TM_RESET_ENABLE()

extern   u8 SPIDER_WORK_DATA[42]; //37to42

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
	
	u8 Fast_Order5Flag;		//�����5
	u8 Fast_Order6Flag;		//�����6

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
/***********************************************************************************************
**************************************PRO_spider.h************************************
***********************************************************************************************/
#ifndef _PRO_SPIDER__H_
#define _PRO_SPIDER__H_


// spider���ͷ�ͽ�����
#define SPIDER_CMD_START 0X59
#define SPIDER_CMD_END   0X47


//------------------------- ������
#define  CMD_TYPE_CONFIG    0X01 
	#define CONFIG_CMD_SETWORKPARAMETER			0X01  	// ���ù�������
	#define CONFIG_CMD_SETMODULECONFIG  0X02	// ����ģ����̬
//------------------------ ������	
#define  CMD_TYPE_CTRL		0X03 
	#define CTRL_CMD_RESTART_MOD			0X01	//��λģ��
	#define CTRL_CMD_RESTART_RF			0X02	//��λģ�����߲���
	#define CTRL_CMD_RESETDEFAULT  	0X03	//�ָ�����
	#define CTRL_CMD_LOGOUTNETFILE	0X04	//ע����������
	#define CTRL_CMD_CLOSESLEEP       0X10	//�ر����߹���
	#define CTRL_CMD_SWITCHWORKMODE   0X11	//�л�����ģʽ

//------------------------ ��ѯ��	
#define  CMD_TYPE_QUERY		0X04 
	#define QUERY_CMD_GETMODULESTATUS		0X01	//��ѯģ��״̬
	#define QUERY_CMD_GETWORKPARAMETER		0X02	//��ѯ��������
	#define QUERY_CMD_GETNETFILE				0X06	//��ѯ��������
	#define QUERY_CMD_GETMODULESHAPE			0X0A	//��ѯģ����̬

	#define QUERY_CMD_GETDOWNSTAIRFALST  0X31	//��ѯ���п����
	#define QUERY_CMD_GETDOWNUPACK		0X32	// ��ѯ����U��
	#define QUERY_CMD_GETDOWNMPACK		0X33	// ��ѯ����M��
	
#define  CMD_TYPE_TRFER		0X05 // ������
	#define TRFER_CMD_SENDSTATUS     		0X01	//��������״̬
	#define TRFER_CMD_SENDUPACK			0X02	//�������� U��
	#define TRFER_CMD_SENDMPACK			0X03	//�������� M ��
	#define TRFER_CMD_STARTPPACK			0X04	//�������� P ��������
	
	
	#define TRFER_CMD_SENDPPACK			0X05	//�������� P ��
	#define TRFER_CMD_STOPPPACK			0X06	//�ر����� P ��������


void PRO_spider_BuildCMDForNoPar(u8 cmd,u8 cmd1);
void PRO_spider_BuildCMDForPar(u8 cmd,u8 cmd1,u8* pdata,u16 dataLength);
void PRO_spider_BuildMParkForPar(u8 *SendDate,u8 serve,u8* pdata,u16 dataLength);


#endif


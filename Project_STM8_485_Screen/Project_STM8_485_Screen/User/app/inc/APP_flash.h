#ifndef _APP_FLASH_H_
#define _APP_FLASH_H_

#define EQUIPMENTCHAR1	0x47	//�豸��Ŀ�ַ�1"G"
#define EQUIPMENTCHAR2	0x43	//�豸��Ŀ�ַ�2"C"
#define SNSTARTADD	0x13E0	//SN���ݵĿ�ʼ��ַ
#define SNLENG		5		//SN�����ݳ���
#define SNFLAGADD	0x13E7	//SN���ݱ�־λ�ĵ�ַ
#define SNFLAGDATA	0x79	//SN�ı�־λ����

#define SNHAVEADD	0X13E5

typedef struct 
{
	u8 SNdata[SNLENG];
	u8 SNflag;
}SN_STRUCT;

extern SN_STRUCT APP_SN_StrPar;		   


void APP_SN_Write(void);
void APP_SN_Init(void);



#endif
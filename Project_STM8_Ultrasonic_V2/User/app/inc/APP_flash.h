#ifndef _APP_FLASH_H_
#define _APP_FLASH_H_

#define EQUIPMENTCHAR1	0x55	//�豸��Ŀ�ַ�1"U"
#define EQUIPMENTCHAR2	0x4C	//�豸��Ŀ�ַ�2"L"

#define SNFLAGADD	0x13E7	//SN���ݱ�־λ�ĵ�ַ
#define SNFLAGDATA	0x79	//SN�ı�־λ����

#define SNHAVEADD	0X13E5



/*---------------------�ն˲�Ʒ���к�V1.0.0----------------------*/
/*
N��ַ��Χ��0x17F0~0x17F5
SN��ʽ�� ҵ������(1byte)+���(4byte)+У����(1byte)
ҵ�����ͣ� 
		������ 		C1
		��λ��Ƶ	C8
		��λ��		C5
		������		D1
		�շ���		C3
		�ش�		C4
		��բ		C7
��ţ�4byte �� 0x00 0x00 0x00 0x01 ��ʼ,������ 0x99 0x99 0x99 0x99
	  �����9��λ����ʮ������£����� 0x09 ��λ 0x10
	  ע����Ų�Ϊ0
У���룺 У���� = ҵ������^��� + 0x47
*/
#define SN_START_ADD	0x17F0	//SN���ݵĿ�ʼ��ַ
#define SN_LENG		6			//SN�����ݳ���
#define SN_TYPE		0xC1		//ҵ������
#define SN_CRC_ADD		0x17F5	//У�����ַ




typedef struct 
{
	u8 SNdata[SN_LENG];
	u8 SNflag;
}SN_STRUCT;

extern SN_STRUCT APP_SN_StrPar;		   

u8 APP_SN_CRC(u8 *pdata,u8 sn_type);
void APP_SN_Write(void);
void APP_SN_Init(void);



#endif
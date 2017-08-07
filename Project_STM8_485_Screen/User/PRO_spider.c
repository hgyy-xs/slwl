#include "main.h"



/*
	��������(����޲���)
	cmd:�������
	cmd1:��������
*/
void PRO_spider_BuildCMDForNoPar(u8 cmd,u8 cmd1)
{
	u16 cnt;
	APP_SPIDER.TxUsartLeng =0 ;
        
	APP_SPIDER.TxUsartData[APP_SPIDER.TxUsartLeng++]= SPIDER_CMD_START;// ͷ
	APP_SPIDER.TxUsartData[APP_SPIDER.TxUsartLeng++]= 0x02;// ���ȵ��ֽ�
	APP_SPIDER.TxUsartData[APP_SPIDER.TxUsartLeng++]= 0x00;
	APP_SPIDER.TxUsartData[APP_SPIDER.TxUsartLeng++]= cmd;// �������
	APP_SPIDER.TxUsartData[APP_SPIDER.TxUsartLeng++]= cmd1;// ��������
	cnt = APP_SPIDER.TxUsartLeng;
	APP_SPIDER.TxUsartData[APP_SPIDER.TxUsartLeng++]= ALG_CmdCrcCal(&APP_SPIDER.TxUsartData[1],cnt-1);
	APP_SPIDER.TxUsartData[APP_SPIDER.TxUsartLeng++]= SPIDER_CMD_END;
}



/*
	��������(����в���)
	cmd:�������
	cmd1:��������
	pdata:����ָ��
	dataLength:�����ĳ���
*/
void PRO_spider_BuildCMDForPar(u8 cmd,u8 cmd1,u8* pdata,u16 dataLength)
{
	u16 cnt;
	
	APP_SPIDER.TxUsartLeng =0 ;
	APP_SPIDER.TxUsartData[APP_SPIDER.TxUsartLeng++]= SPIDER_CMD_START;// ͷ(��ʼ��)
	APP_SPIDER.TxUsartLeng+=2; // ��������2���ֽ�
        
	APP_SPIDER.TxUsartData[APP_SPIDER.TxUsartLeng++]= cmd;// �������
	APP_SPIDER.TxUsartData[APP_SPIDER.TxUsartLeng++]= cmd1;// ��������
        
	for(cnt=0;cnt<dataLength;cnt++)
	{
		APP_SPIDER.TxUsartData[APP_SPIDER.TxUsartLeng++]=*(pdata+cnt);
	}
	//  ���¸�ֵ�����ֽ�
	APP_SPIDER.TxUsartData[1]= (APP_SPIDER.TxUsartLeng-3)&0xFF;
	APP_SPIDER.TxUsartData[2]= ((APP_SPIDER.TxUsartLeng-3)>>8)&0xFF;
        
	cnt = APP_SPIDER.TxUsartLeng;
	APP_SPIDER.TxUsartData[APP_SPIDER.TxUsartLeng++]= ALG_CmdCrcCal(&APP_SPIDER.TxUsartData[1],cnt-1);
	APP_SPIDER.TxUsartData[APP_SPIDER.TxUsartLeng++]= SPIDER_CMD_END;
}
/*
 * �� �� ��: main
 * ��    ��: ������
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��: 
 */
 
void PRO_spider_BuildMParkForPar(u8 *SendDate,u8 serve,u8* pdata,u16 dataLength){
	int i = 0;
	if(dataLength > 128)	dataLength = 128;
	SendDate[0] = serve;
	for(i = 0;i < dataLength; i++){
		SendDate[i+1] = pdata[i];
	}
	if(dataLength%16 != 0)	dataLength = dataLength/16+16;
	PRO_spider_BuildCMDForPar(CMD_TYPE_TRFER, TRFER_CMD_SENDMPACK, SendDate, dataLength+1);
}



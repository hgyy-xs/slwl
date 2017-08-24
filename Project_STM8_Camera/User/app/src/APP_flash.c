
#include "main.h"

void APP_SN_Write(void){
	u8 SNData[SNLENG] = {0};
	u8 SNFlag = SNFLAGDATA;
	u8 Code = 0;
	u8 s_tmp[7] = {0};
	HAL_eeprom_WriteData(SNFLAGADD, &SNFlag, 1);		//写入SN修改标志位
	HAL_eeprom_ReadData(SNSTARTADD, SNData, SNLENG);	//读取SN原始数据
	Code = ALG_SN_Code(SNData);							//获取SN加密字
	ALG_SN_Encrypt(SNData, Code);						//对SN设备号加密
	HAL_eeprom_WriteData(SNSTARTADD, SNData, SNLENG);	//写入SN加密后的数据
	s_tmp[0] = 5;
	s_tmp[1] = 0;
	s_tmp[2] = SNData[0];
	s_tmp[3] = SNData[1];
	s_tmp[4] = SNData[2];
	s_tmp[5] = SNData[3];
	s_tmp[6] = SNData[4];
	PRO_spider_BuildCMDForPar(CMD_TYPE_CONFIG,CONFIG_CMD_SETWORKPARAMETER,s_tmp,7);
	HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);         
} 

u8  APP_SN_Init(void){
	u8 SNFlag = 0;
	u8 SNData[SNLENG] = {0};
	HAL_eeprom_ReadData(SNHAVEADD, &SNFlag, 1);
	if(SNFlag != 0xF1){
		return 0;
	}
	HAL_eeprom_ReadData(SNFLAGADD, &SNFlag, 1);	//读取SN修改标志位
	if(SNFlag == 0){		//判断SN数据是否加密
		APP_SN_Write();	//对SN数据进行加密
	}
	HAL_eeprom_ReadData(SNSTARTADD, SNData, SNLENG);	//读取SN数据
	return 1;
}

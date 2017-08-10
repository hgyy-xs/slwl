#include "main.h"

SN_STRUCT APP_SN_StrPar;	

void APP_SN_Write(void){
	// u8 SNData[SNLENG] = {0};
	u8 SNFlag = SNFLAGDATA;
	u8 Code = 0;
	// u8 s_tmp[7] = {0};
	HAL_eeprom_WriteData(SNFLAGADD, &SNFlag, 1);					//写入SN加密修改标志位
	//HAL_eeprom_ReadData(SNSTARTADD, SNData, SNLENG);				//读取SN原始数据
	Code = ALG_SN_Code(APP_SN_StrPar.SNdata);						//获取SN加密字
	ALG_SN_Encrypt(APP_SN_StrPar.SNdata, Code);						//对SN设备号加密
	HAL_eeprom_WriteData(SNSTARTADD, APP_SN_StrPar.SNdata, SNLENG);	//写入SN加密后的数据
#ifdef DEBUG
	printf("\r\n【MCU status】:encrypted SN: %02X_%02X_%02X_%02X_%02X\r\n",APP_SN_StrPar.SNdata[0],
		APP_SN_StrPar.SNdata[1],APP_SN_StrPar.SNdata[2],APP_SN_StrPar.SNdata[3],APP_SN_StrPar.SNdata[4]);
#endif
	// s_tmp[0] = 5;
	// s_tmp[1] = 0;
	// s_tmp[2] = SNData[0];
	// s_tmp[3] = SNData[1];
	// s_tmp[4] = SNData[2];
	// s_tmp[5] = SNData[3];
	// s_tmp[6] = SNData[4];
	//PRO_spider_BuildCMDForPar(CMD_TYPE_CONFIG,CONFIG_CMD_SETWORKPARAMETER,s_tmp,7);
	//HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);         
} 

void APP_SN_Init(void){
	HAL_eeprom_ReadData(SNSTARTADD, APP_SN_StrPar.SNdata, SNLENG);	//读取SN原始数据
	HAL_eeprom_ReadData(SNFLAGADD, &APP_SN_StrPar.SNflag, 1);		//读取SN加密标志位数据
	if(APP_SN_StrPar.SNflag == SNFLAGDATA){//SN号已加密
		if(APP_SN_StrPar.SNdata[0]||APP_SN_StrPar.SNdata[1]||APP_SN_StrPar.SNdata[2]||APP_SN_StrPar.SNdata[3]||APP_SN_StrPar.SNdata[4]){
			u8 Code = 0;
			Code = ALG_SN_Code(APP_SN_StrPar.SNdata);	//获取SN加密字
			if((APP_SN_StrPar.SNdata[3] == EQUIPMENTCHAR2^Code) && (APP_SN_StrPar.SNdata[4] == EQUIPMENTCHAR1^Code) ){	//判断加密是否正确
#ifdef DEBUG
				printf("\r\n【MCU status】:SN: %02X_%02X_%02X_%02X_%02X\r\n",APP_SN_StrPar.SNdata[0],
				APP_SN_StrPar.SNdata[1],APP_SN_StrPar.SNdata[2],APP_SN_StrPar.SNdata[3],APP_SN_StrPar.SNdata[4]);
#endif
				// for (int i = 0; i < 5; ++i)	//写入工作参数区
				// {
				// 	SPIDER_WORK_DATA[i]=APP_SN_StrPar.SNdata[i];
				// }
			}else{	//加密不正确
				while(1)
				{
					printf("\r\n【%s】SN falut !\r\n",__FUNCTION__ );
					feed_IWDG();				//喂狗	
				}
			}
		}
		else{
			//while(1);//添加LED闪烁指示，和喂狗过程
			while(1)
			{
				printf("\r\n【%s】SN falut !\r\n",__FUNCTION__ );
				feed_IWDG();				//喂狗	
			}
		}
	}
	else{
		if(APP_SN_StrPar.SNdata[4] == EQUIPMENTCHAR1 && APP_SN_StrPar.SNdata[3] == EQUIPMENTCHAR2){//SN号未加密
#ifdef DEBUG
	printf("\r\n【MCU status】:The is SN unencrypted\r\n");
#endif
#ifdef DEBUG
	printf("\r\n【MCU status】:unencrypted SN: %02X_%02X_%02X_%02X_%02X\r\n",APP_SN_StrPar.SNdata[0],
		APP_SN_StrPar.SNdata[1],APP_SN_StrPar.SNdata[2],APP_SN_StrPar.SNdata[3],APP_SN_StrPar.SNdata[4]);
#endif
			APP_SN_Write();	//对SN数据进行加密
		}
		else if(APP_SN_StrPar.SNdata[4] == 0x00 && APP_SN_StrPar.SNdata[3] == 0x00){//无SN号
#ifdef DEBUG
	printf("\r\n【MCU status】:The is no SN\r\n");
#endif
			//while(1);//添加LED闪烁指示，和喂狗过程
		}
		else{//SN有误
#ifdef DEBUG
	printf("\r\n【MCU status】:SN error\r\n");
#endif
			//while(1);//添加LED闪烁指示，和喂狗过程
		}
	}

// 	u8 SNFlag = 0;
// 	u8 SNData[SNLENG] = {0};
// 	HAL_eeprom_ReadData(SNHAVEADD, &SNFlag, 1);
// 	if(SNFlag != 0xF1){
// #ifdef DEBUG
// 		printf("\r\nnot have SN, init failure\r\n");
// #endif
// 		return 0;
// 	}
// 	HAL_eeprom_ReadData(SNFLAGADD, &SNFlag, 1);	//读取SN修改标志位
// 	if(SNFlag == 0){		//判断SN数据是否加密
// #ifdef DEBUG
// 	printf("\r\nstart init SN...\r\n");
// #endif
// 	APP_SN_Write();	//对SN数据进行加密
// #ifdef DEBUG
// 	printf("\r\nSN init succeed\r\n");
// #endif
// 	}
// 	HAL_eeprom_ReadData(SNSTARTADD, SNData, SNLENG);	//读取SN数据
// #ifdef DEBUG
// 	printf("\r\nSN: %X-%X-%X-%X-%X\r\n",SNData[0],SNData[1],SNData[2],SNData[3],SNData[4]);
// #endif
	// return 1;
}

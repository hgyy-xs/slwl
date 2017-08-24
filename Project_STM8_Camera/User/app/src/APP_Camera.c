#include "APP_Camera.h"

CAMERA_STRUCT APP_CAMERA;

const char BlueCol[]	= " 0, 0, 255, 0 ";
const char YellowCol[]	= " 255, 255, 0, 0 ";
const char WhiteCol[]	= " 255, 255, 255, 0 ";
const char BlackCol[]	= " 0, 0, 0, 0 ";
const char GreenCol[]	= " 0, 255, 0, 0 ";

void APP_Camera_ValueInit(void){
	int i = 0;
	APP_CAMERA.CameraID = CAMERADEFAULTID;		//摄像头ID
	APP_CAMERA.LocaTion = 0;		//停车位
	APP_CAMERA.PlateColor = 0;		//车牌颜色
	APP_CAMERA.PlateLeng = 0;		//车牌数据长度
	for(i = 0; i < PLATEMAXLENG; i++){
		APP_CAMERA.PlateNumber[i] = 0;	//车牌号
	}
	APP_CAMERA.SpaceState = 0;		//停车状态
	APP_CAMERA.DataSucFlag = FALSE;

	
}
void LocatNumberDate(void){
	APP_CAMERA.LocaTion = UART2.RxBuff[16] - 0x30;
}
void StateNumberDate(void){
	APP_CAMERA.SpaceState = UART2.RxBuff[UART2.RxLeng - 3] - 0X30;
}

void CarPlateColor(void){
	unsigned char platelenstart = 0,platelen = 0;
	char temp[25] = {0};
	if(UART2.RxBuff[35] == 0x6E){
		APP_CAMERA.PlateColor = 0;
	}
	else{
		platelenstart = 36;
		while(UART2.RxBuff[platelenstart] != 0x5D){
			temp[platelen++] = UART2.RxBuff[platelenstart++];
		}
		if(strcmp(temp,BlueCol) == 0){
			APP_CAMERA.PlateColor = 1;
		}
		else if(strcmp(temp,YellowCol) == 0){
			APP_CAMERA.PlateColor = 2;
		}
		else if(strcmp(temp,WhiteCol) == 0){
			APP_CAMERA.PlateColor = 3;
		}
		else if(strcmp(temp,BlackCol) == 0){
			APP_CAMERA.PlateColor = 4;
		}
		else if(strcmp(temp,GreenCol) == 0){
			APP_CAMERA.PlateColor = 5;
		}
		else{
			APP_CAMERA.PlateColor = 0;
		}
	}
}
void CarPlateNumber(void){
	unsigned char platelenstart = 0,platelen = 0;
	int i = 0;
	char temp[PLATEMAXLENG] = {0};
	for(i = 0; i < PLATEMAXLENG; i++){
		APP_CAMERA.PlateNumber[i] = 0;
	}
	APP_CAMERA.PlateLeng = 0;
	if(UART2.RxBuff[UART2.RxLeng - 57] == 0x22){
		platelenstart = UART2.RxLeng - 58;
		while(UART2.RxBuff[platelenstart] != 0x22){
			temp[platelen++] = UART2.RxBuff[platelenstart--];
		}
		for(i = 0; i < platelen; i++){
			APP_CAMERA.PlateNumber[i] = temp[platelen-i-1];
		}
		APP_CAMERA.PlateLeng = platelen;
	}
}

void APP_Camera_uartDeal(void){
	// UART3  接收处理
	if(UART2.RxDataSucFlag==TRUE){
		LocatNumberDate();
		StateNumberDate();
		CarPlateColor();
		CarPlateNumber();
		
		// 重新开始接收
		UART2.RxDataSucFlag=FALSE;
		UART2.RxDealStep=1;	
		APP_CAMERA.DataSucFlag = TRUE;
	}
}
void APP_Camera_dealSendCmdToSpider(void){
	int i = 0;
	static u8 pkg_cnt=0
	u8 Camera_MPark[32] = {0};
	u8 SendDate[40] = {0};
	if(APP_CAMERA.DataSucFlag==TRUE){

		pkg_cnt++;
		//Camera_MPark[APP_CAMERA.PlateLeng + 6] = (u8)APP_CAMERA.CameraID;
		//Camera_MPark[APP_CAMERA.PlateLeng + 7] = (u8)(APP_CAMERA.CameraID>>8);
		if(APP_CAMERA.PlateLeng > 9){	//超过16字节，改发32字节M包
			//Camera_MPark[31] = 0xC8;
			Camera_MPark[0] = PKG_CLASS  //包类型
			Camera_MPark[1] = pkg_cnt	//包计数
			Camera_MPark[2] = APP_CAMERA.PlateLeng+4;	//数据长度为 车牌数据长度+4
			Camera_MPark[3] = APP_CAMERA.SpaceState;	//停车状态	
			Camera_MPark[4] = APP_CAMERA.LocaTion;	//停车位
			Camera_MPark[5] = APP_CAMERA.PlateColor;
			Camera_MPark[6] = APP_CAMERA.PlateLeng;
			for(i = 0; i < APP_CAMERA.PlateLeng; i++){
				Camera_MPark[i+7] = APP_CAMERA.PlateNumber[i];
			}
			for(i = APP_CAMERA.PlateLeng+7; i <32 ; ++i)	//多余位填充0
				{
					Camera_MPark[i]=0x00;	
				}	
			PRO_spider_BuildMParkForPar(SendDate, 0x01,Camera_MPark,32);	//服务质量为3，最大数据长度为3*16byte
			HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);//发送命令
		}
		else{	//发16字节M包
				Camera_MPark[0] = UP_PKG_CLASS//主动上报包类型
				Camera_MPark[1] = pkg_cnt	//包计数
				Camera_MPark[2] = APP_CAMERA.PlateLeng+4;	//数据长度为 车牌数据长度+4
				Camera_MPark[3] = APP_CAMERA.SpaceState;	//停车状态	
				Camera_MPark[4] = APP_CAMERA.LocaTion;	//停车位
				Camera_MPark[5] = APP_CAMERA.PlateColor;
				Camera_MPark[6] = APP_CAMERA.PlateLeng;
				for(i = 0; i < APP_CAMERA.PlateLeng; i++){
					Camera_MPark[i+7] = APP_CAMERA.PlateNumber[i];
				}
				for(i = APP_CAMERA.PlateLeng+7; i <16 ; ++i)	//多余位填充0
					{
						Camera_MPark[i]=0x00;	
					}	
			//Camera_MPark[15] = 0xC8;
			PRO_spider_BuildMParkForPar(SendDate, 0x01,Camera_MPark,16);	//最大数据长度2*16
			HAL_USART_SendStringN((u8 *)APP_SPIDER.TxUsartData,APP_SPIDER.TxUsartLeng,SPIDER_USART);//发送命令
		}
		APP_CAMERA.DataSucFlag=FALSE;
	}
}


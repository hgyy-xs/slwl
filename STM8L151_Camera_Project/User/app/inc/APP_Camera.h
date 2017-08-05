#ifndef __APP_CAMERA_H
#define __APP_CAMERA_H


#include "main.h"
#include <string.h>

#define PLATEMAXLENG 16
#define CAMERADEFAULTID 20

typedef struct{
	u8	LocaTion;			//ͣ��λ
	u8	PlateColor;			//������ɫ
	u8	PlateLeng;			//�������ݳ���
 	u8	PlateNumber[PLATEMAXLENG];	//���ƺ���
	u8	SpaceState;			//ͣ��״̬
	u16	CameraID;			//����ͷID
	u8 DataSucFlag;		//���ݱ���ɹ���־λ
}CAMERA_STRUCT;

extern CAMERA_STRUCT APP_CAMERA;

void APP_Camera_ValueInit(void);
void APP_Camera_uartDeal(void);
void APP_Camera_dealSendCmdToSpider(void);

#endif

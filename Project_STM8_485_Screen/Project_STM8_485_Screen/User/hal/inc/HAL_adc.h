

#ifndef _HAL_ADC_H_
#define _HAL_ADC_H_


typedef struct 
{
	u16 DataTmp;	//�ɼ�������
	u8 Datacnt;		//�ɼ����ݼ�����Ϊ��ɼ�����ƽ��ֵ
	u16 AverDataTmp;	//ƽ��ֵ
	u8 WorkGrade;			//������ѹ�ȼ�

}ADC_STRUCT;

extern ADC_STRUCT  HAL_adc_StrPar;




void HAL_adc_ValueInit(void);
void HAL_ADC_Init(void);

void APP_ADC_GetWorkVolGrade(void);




#endif


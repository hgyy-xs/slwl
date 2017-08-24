

#ifndef _HAL_ADC_H_
#define _HAL_ADC_H_

//adc检测计时
#define ADC_CHECK_TIMCNT 5


typedef struct 
{
	u16 DataTmp;	//采集到数据
	u8 Datacnt;		//采集数据计数，为求采集数据平均值
	u16 AverDataTmp;	//平均值
	u8 WorkVoltgeGrade;			//工作电压等级

	u8 CheckWorkVoltgeTimeCount;		//检测电压计时 
	u8 CheckWorkVoltgeFlag;			//检测工作电压标志

}ADC_STRUCT;

extern ADC_STRUCT  HAL_adc_StrPar;




void HAL_adc_ValueInit(void);
void HAL_ADC_Init(void);

void APP_ADC_GetWorkVolGrade(void);




#endif


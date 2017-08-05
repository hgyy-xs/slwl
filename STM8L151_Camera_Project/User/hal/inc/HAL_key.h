

#ifndef _HAL_KEY_H_
#define _HAL_KEY_H_



#define KEY_GPIO		GPIOB
#define KEY_GPIO_PIN	       GPIO_Pin_1

#define KEY_INPUT()	       GPIO_ReadInputDataBit(KEY_GPIO ,KEY_GPIO_PIN)

#define KEY_PRESS_SCAN_CNT 30		// 按下扫描计数
#define KEY_FLOAT_SCAN_CNT 5		// 浮空扫描计数

typedef struct
{
	u16 press_cnt;
	u16 nopre_cnt;
	u8  press_status;
}KEY_STRUCT;

extern KEY_STRUCT HalKey_StrPar;

void HAL_KEY_config(void);
void HAL_KEY_Value(void);
void HAL_KEY_Scan(void);
void HAL_KEY_deal(void);



#endif







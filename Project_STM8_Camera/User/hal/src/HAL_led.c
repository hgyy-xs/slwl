#include "main.h"

/*
 * 函 数 名: HAL_LED_config
 * 功    能: LED引脚初始化
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 其    他: 
 */
void HAL_LED_config(void){
	GPIO_Init(LED_GPIO, LED_GPIO_PIN, GPIO_Mode_Out_PP_High_Fast);//高电平快速推挽输出
}

/*
 * 函 数 名: HAL_LED_AllOff
 * 功    能: 关闭所有的LED
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 其    他: 
 */
void HAL_LED_AllOff(void){
	LED1_OFF();
	LED2_OFF();
}


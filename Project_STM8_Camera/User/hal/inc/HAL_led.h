#ifndef _HAL_LED_H_
#define _HAL_LED_H_

#define LED_GPIO       GPIOD

#define LED1_GPIO_PIN	 GPIO_Pin_7
#define LED2_GPIO_PIN	 GPIO_Pin_6


//LED操作宏
#define LED1_ON()     GPIO_ResetBits(LED_GPIO,LED1_GPIO_PIN)
#define LED1_OFF()    GPIO_SetBits(LED_GPIO,LED1_GPIO_PIN)
#define LED1_XOR()    GPIO_ToggleBits(LED_GPIO,LED1_GPIO_PIN);//切换状态

#define LED2_ON()     GPIO_ResetBits(LED_GPIO,LED2_GPIO_PIN)
#define LED2_OFF()    GPIO_SetBits(LED_GPIO,LED2_GPIO_PIN)
#define LED2_XOR()    GPIO_ToggleBits(LED_GPIO,LED2_GPIO_PIN);//切换状态

void HAL_LED_config(void);
void HAL_LED_AllOff(void);

#endif






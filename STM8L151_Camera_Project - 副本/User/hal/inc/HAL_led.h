#ifndef _HAL_LED_H_
#define _HAL_LED_H_

#define LED_GPIO       GPIOB

#define LED_GPIO_PIN	 GPIO_Pin_0

//LED²Ù×÷ºê
#define LED_ON()     GPIO_ResetBits(LED_GPIO,LED_GPIO_PIN)
#define LED_OFF()    GPIO_SetBits(LED_GPIO,LED_GPIO_PIN)
#define LED_XOR()    GPIO_ToggleBits(LED_GPIO,LED_GPIO_PIN);//ÇÐ»»×´Ì¬

void HAL_LED_config(void);
void HAL_LED_AllOff(void);

#endif






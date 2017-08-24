#ifndef _MAIN_H
#define _MAIN_H


//-----头文件----
#include "stm8l15x.h"
#include "stm8l15x_it.h"
#include "stm8l15x_pwr.h"
#include <stdio.h>


#include "HAL_usart.h"
#include "HAL_gpio.h"
#include "HAL_tim.h"
#include "HAL_key.h"
#include "HAL_led.h"
#include "HAL_eeprom.h"

#include "APP_nor_mode.h"
#include "APP_spider.h"
#include "APP_iwdg.h"
#include "APP_Camera.h"
#include "APP_flash.h"

#include "sysclk.h"

#include "FUN_algorithm.h"
#include "PRO_spider.h"

//--ST库头文件--
#include "stm8l15x_gpio.h"
#include "stm8l15x_usart.h"
#include "stm8l15x_spi.h"



//调试开关
//#define  INIT_DEBUG

#define _BV(bit) (1 << (bit))


#define USART_MAX_DATA_SIZE		200


#define SPIDER_CMD_TIME  800
#define SPIDER_CMD1_TIME 800


#endif
#include "main.h"

/*
 * 函 数 名: APP_nor_mode_RealTask
 * 功    能: 真正的任务
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 其    他: 
 */
static void APP_nor_mode_RealTask(void){
	feed_IWDG();				//喂狗
	HAL_KEY_Scan();			//按键扫描
	HAL_KEY_deal();			//按键处理
	APP_spider_uartDeal();		//spider串口处理
	APP_Spider_dealSendCmdToSpider();	//发送命令到spider处理
	APP_spider_IEPIN_deal();	//处理spider的中断事件
	APP_Camera_uartDeal();		//spider串口处理
	APP_Camera_dealSendCmdToSpider();	//发送命令到spider处理
}

/*
 * 函 数 名: APP_nor_mode_FixedTask
 * 功    能: 系统定时工作任务
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 其    他: 
 */
void APP_nor_mode_FixedTask(void){
	/**************** 定时1ms需要执行的任务 ****************/

	/*************** 定时200ms需要执行的任务 ***************/

	/*************** 定时500ms需要执行的任务 ***************/

	/**************** 定时1s需要执行的任务 *****************/
	
}

/*
 * 函 数 名: APP_nor_mode_SysNormalMode
 * 功    能: 系统正常工作模式
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 * 其    他: 
 */
void APP_nor_mode_SysNormalMode(void){
		//while(1){
		//if(APP_SN_Init()){
			//break;
			//printf("have SN");
		//}
		//else{
		//	printf("havn`t SN");
		//}
	//}
	APP_spider_init();			//spider模块初始化
	Main_StrPar.Init_LED_ShakeFlag = FALSE;	//初始化完成
	APP_Spider_ReadyOnNet();	//
	printf("\r\nall init success\r\n");
	while(1){
		APP_nor_mode_RealTask();	//实时的任务
		APP_nor_mode_FixedTask();
	}
}


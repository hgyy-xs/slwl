#include "main.h"


struct UART_STRUCT               UART2,UART3;
static USART_RX_Control_Struct   g_usart_rx_control_struct = {0};


static void UART2_ValueInit(void)
{
	UART2.RxLeng = 0;
	UART2.RxOverFlag = FALSE;
	UART2.RxDataFlag = FALSE;
	UART2.RxTimeOutCnt = 0;
	UART2.TxLeng = 0;
	UART2.RxDealStep = 1;
	UART2.RxCnt = 0;
	UART2.RxDataSucFlag= FALSE;
}


static void UART3_ValueInit(void)
{
	UART3.RxLeng = 0;
	UART3.RxOverFlag = FALSE;
	UART3.RxDataFlag = FALSE;
	UART3.RxTimeOutCnt = 0;
	UART3.TxLeng = 0;
	UART3.RxDealStep = 1;
	UART3.RxCnt = 0;
}


void HAL_USART_config(void){
  //调试串口
  GPIO_Init(GPIOC,GPIO_Pin_3,GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOC,GPIO_Pin_2,GPIO_Mode_In_FL_No_IT);
  
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1,ENABLE);
  
  USART_DeInit(USART1);
  
  USART_Init(USART1,(uint32_t)115200,USART_WordLength_8b,USART_StopBits_1,\
             USART_Parity_No,(USART_Mode_TypeDef)(USART_Mode_Tx|USART_Mode_Rx));
  
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
  
  USART_Cmd(USART1,ENABLE);
  
  
  
  //485通信串口
  GPIO_Init(GPIOE, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOE, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOE, GPIO_Pin_3, GPIO_Mode_In_FL_No_IT);
  
  CLK_PeripheralClockConfig(CLK_Peripheral_USART2,ENABLE);
  
  USART_DeInit(USART2);
  
  USART_Init(USART2,(u32)9600,USART_WordLength_8b,USART_StopBits_1,\
            USART_Parity_No,(USART_Mode_TypeDef)(USART_Mode_Tx|USART_Mode_Rx));
  
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
  
  USART_Cmd(USART2,ENABLE);
  
  CTRL485_RX_ENABLE();
  
  
  
  //spider通信串口
  GPIO_Init(GPIOE, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_In_FL_No_IT);
  
  CLK_PeripheralClockConfig(CLK_Peripheral_USART3,ENABLE);
  
  USART_DeInit(USART3);
  
  USART_Init(USART3,(u32)115200,USART_WordLength_8b,USART_StopBits_1,\
            USART_Parity_No,(USART_Mode_TypeDef)(USART_Mode_Tx|USART_Mode_Rx));
  
  USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
  
  USART_Cmd(USART3,ENABLE);
  
  
  
  UART2_ValueInit();
  UART3_ValueInit();
}


void usart_delay(void)
{
  if(g_usart_rx_control_struct.rx_timer < 0xFFFF)
  {
    g_usart_rx_control_struct.rx_timer++;
  }
}


void received_data_from_rs485(uint8_t data)
{
  if(g_usart_rx_control_struct.rx_len >= USART_RECEIVED_MAX_LENGTH)
  {
    g_usart_rx_control_struct.rx_len = 0;
  }
  
  g_usart_rx_control_struct.rx_buf[g_usart_rx_control_struct.rx_len++] = data;
  g_usart_rx_control_struct.rx_timer = 0;
}


void HAL_USART_SendByte(u8 data,u8 usart_num)
{
  if(USER_USART1 == usart_num)
  {
    USART_SendData8(USART1,data);
    while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
  }
  else if(USER_USART2 == usart_num)
  {
    USART_SendData8(USART2,data);
    while (USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
  }
  else if(USER_USART3 == usart_num)
  {
    USART_SendData8(USART3,data);
    while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
  }
}



void HAL_USART_SendString(u8 *p,u8 usart_num)
{
  u8 sendbuf = 0;
  
  while (p[sendbuf] != '\0')  
  {
    HAL_USART_SendByte(p[sendbuf],usart_num);
    sendbuf++;
  }
}



void HAL_USART_SendStringN(u8 *p, u16 num,u8 usart_num)
{
  u16 cout = 0;
  
  for(cout = 0; cout < num; cout++)
  {
    HAL_USART_SendByte(p[cout],usart_num);
  }
}



void HAL_RS485_TxNbyte(u8 *dat,u16 num,u8 usart_num)
{
    CTRL485_TX_ENABLE();
    HAL_USART_SendStringN(dat,num,usart_num);
    CTRL485_RX_ENABLE();
}


int putchar(int c)
{
  while ((USART1->SR&0x80)==0x00); 
  HAL_USART_SendByte((u8)c,USER_USART1);
  return (c);  
}



void HAL_UART2_IERecieve(u8 dat){
	if(UART2.RxDealStep>1){
		UART2.RxDataFlag = TRUE;
		UART2.RxTimeOutCnt=0;
	}
	
	switch(UART2.RxDealStep){
		case 0:
			break;
		case 1:  //获取有效的字符'{'
			if(dat == 0x7B){
				UART2.RxCnt = 0;	//清空数据长度
				UART2.RxBuff[UART2.RxCnt++] = dat;//保存数据并累加数据长度
				UART2.RxDealStep++;	//跳转下一个字符
				UART2.RxDataSucFlag = FALSE;	//清空接收完成标志位
			}
			break;
		case 2:  //存放数据
			UART2.RxBuff[UART2.RxCnt++] = dat;
			if(dat == 0x7D){
				UART2.RxBuff[UART2.RxCnt] = '\0';
				UART2.RxLeng = UART2.RxCnt;
				UART2.RxDataSucFlag=TRUE;	//完整数据标志
				UART2.RxDealStep=0;			//停止装载数据
				UART2.RxDataFlag = FALSE;
			}
			break;
		default:
			break;
		}
}


void HAL_UART3_IERecieve(u8 dat){
	if(UART3.RxDealStep>1){
		UART3.RxDataFlag = TRUE;
		UART3.RxTimeOutCnt=0;
	}
	switch(UART3.RxDealStep){
		case 0:
			break;
		case 1:  // 获取数据开头
			if(dat==0x59){
				UART3.RxCnt=0;
				UART3.RxBuff[UART3.RxCnt++]=dat;
				UART3.RxDealStep++;
				UART3.RxDataSucFlag=FALSE;
			}
			break;
		case 2:  // 数据长度高字节
				UART3.RxBuff[UART3.RxCnt++]=dat;
                UART3.RxLeng=dat;
				UART3.RxDealStep++;
				break;
		case 3:  // 数据长度低字节
				UART3.RxBuff[UART3.RxCnt++]=dat;
                UART3.RxLeng+=(u16)dat<<8;
				UART3.RxDealStep++;
				// 判断数据长度是否超出存储数据大小
				if(UART3.RxLeng>50){
					UART3.RxDealStep=1; 
				}
				break;
		case 4:
				UART3.RxBuff[UART3.RxCnt++]=dat;
				if(UART3.RxCnt<(UART3.RxLeng+3+1+1)){
						
				}
				else if(UART3.RxCnt==(UART3.RxLeng+3+1+1)){
					UART3.RxDataSucFlag=TRUE;  // 完整数据标志
					UART3.RxDealStep=0; // 停止装载数据
					UART3.RxDataFlag = FALSE;
				}
				else{
					UART3.RxDealStep=1; 
				}
				break;
		default:
			break;
		}
}



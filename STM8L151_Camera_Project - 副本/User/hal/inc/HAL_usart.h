


#ifndef _HAL_USART_H_
#define _HAL_USART_H_


#include "stm8l15x.h"
#include <stdio.h>
#include <string.h>


#define USER_USART1		0X01
#define USER_USART2		0X02
#define USER_USART3		0X03

// �����ʽ
#define USART_CMD_START	0X59		// ��ͷ"Y"
#define USART_CMD_END		0X47		// ��β"G"

//-----------------------------------------cmd define internal----------------------------
#define USART_CMD_NULL	0
#define USART_CMD_1		1
#define USART_CMD_2		2
#define USART_CMD_3		3
#define USART_CMD_4		4
#define USART_CMD_5		5
#define USART_CMD_6		6
#define USART_CMD_7		7
#define USART_CMD_8		8
#define USART_CMD_9		9
#define USART_CMD_10		10
#define USART_CMD_11		11
#define USART_CMD_12		12
#define USART_CMD_13		13
#define USART_CMD_14		14
#define USART_CMD_15		15
#define USART_CMD_16		16
#define USART_CMD_17		17
#define USART_CMD_18		18
#define USART_CMD_19		19
#define USART_CMD_20		20
#define USART_CMD_21		21
#define USART_CMD_22		22
#define USART_CMD_23		23
#define USART_CMD_24		24
#define USART_CMD_25		25
#define USART_CMD_26		26
#define USART_CMD_27		27
#define USART_CMD_28		28
#define USART_CMD_29		29
#define USART_CMD_30		30
#define USART_CMD_31		31
#define USART_CMD_32		32
#define USART_CMD_33		33
#define USART_CMD_34		34
#define USART_CMD_35		35
#define USART_CMD_36		36
#define USART_CMD_37		37
#define USART_CMD_38		38
#define USART_CMD_39		39
#define USART_CMD_40		40


//-----------------------------��������С
#define TXBUFF_MAXSIZE            50
#define RXBUFF_MAXSIZE            140


#define USART_RECEIVED_MAX_LENGTH 200
#define USART_MSECOND_50          50


// ----------485��PE5����-------------
#define CTRL485_GPIO	            GPIOE
#define CTRL485_PIN_NO	     5
#define CTRL485_GPIO_PIN	     GPIO_Pin_5

#define CTRL485_TX_ENABLE()       CTRL485_GPIO->ODR|=_BV(CTRL485_PIN_NO)   // ��������
#define CTRL485_RX_ENABLE()       CTRL485_GPIO->ODR&= ~_BV(CTRL485_PIN_NO) // ��������

#define CTRL485_TX_DISABLE()      CTRL485_RX_ENABLE()
#define CTRL485_RX_DISABLE()      CTRL485_TX_ENABLE()


//----------------------------VAR
struct UART_STRUCT
{
    u16	TxLeng;       // �������ݳ���
    u8	TxIndex;      // ������������
    u8	TxBuff[TXBUFF_MAXSIZE];  // ���ͻ���  
    
    u8	RxDealStep;   // ���մ�����
    u8	RxDataFlag;    // �������ݱ�־
    u16	RxLeng;       // �������ݳ���
    u8	RxCnt;		// ���ռ���
    u8	RxIndex;      // ������������
    u8	RxOverFlag;   // ���ս�����־
    u8	RxTimeOutCnt; // ���ճ�ʱ����
    u8	RxBuff[RXBUFF_MAXSIZE];  // ���ջ���
    
    u8	RxDataSucFlag; // ���ճɹ���־
    u8	RxDataTimCnt;	//��������ʱ�����
};

typedef struct
{
  uint8_t  rx_buf[USART_RECEIVED_MAX_LENGTH];
  uint16_t rx_len;
  volatile uint16_t rx_timer;
}USART_RX_Control_Struct;


extern struct UART_STRUCT UART2,UART3;


void HAL_USART_config(void);

void HAL_USART_SendByte(u8 data,u8 usart_num);
void HAL_USART_SendString(u8 *p,u8 usart_num);
void HAL_USART_SendStringN(u8 *p, u16 num,u8 usart_num);
void HAL_RS485_TxNbyte(u8 *dat,u16 num,u8 usart_num);

int putchar(int c);
void HAL_UART2_IERecieve(u8 dat);
void HAL_UART3_IERecieve(u8 dat);


void usart_delay(void);
void received_data_from_rs485(uint8_t data);

#endif
#ifndef _USART1_H
#define _USART1_H
 
#include "ht32.h"
#include <stdio.h>

#define DEBUG_USART1				1
#define USART_REC_LEN  				200  	//�����������ֽ��� 200 

#if	DEBUG_USART1 
//����1 USART - 1
#define COM1_CLK(CK)                (CK.Bit.USART1)
#define COM1_PORT                   (HT_USART1)
#define COM1_IRQn                   (USART1_IRQn)
#define COM1_IRQHandler             (USART1_IRQHandler)
 

#define COM1_TX_GPIO_ID             (GPIO_PA)
#define COM1_TX_AFIO_PIN            (AFIO_PIN_4)//TX
#define COM1_TX_AFIO_MODE           (AFIO_FUN_USART_UART) 

#define COM1_RX_GPIO_ID             (GPIO_PA)
#define COM1_RX_AFIO_PIN            (AFIO_PIN_5)//RX
#define COM1_RX_AFIO_MODE           (AFIO_FUN_USART_UART)
 
#endif
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

void COM1_Configuration(void);
void Usart_Sendbyte(HT_USART_TypeDef* USARTx, u8 Data);
void Usart_SendArray(HT_USART_TypeDef* USARTx, u8 *array,u8 num);
void Usart_SendStr(HT_USART_TypeDef* USARTx, uint8_t *str);
 
#endif

#ifndef __ESP8266_H
#define __ESP8266_H
/* Includes ---------------------------------------------------------------------- */
#include "ht32.h"



#define COM0_REC_LEN  				200  	//定义最大接收字节数 200 
//串口0 USART - 0
#define COM0_CLK(CK)                (CK.Bit.USART0)
#define COM0_PORT                   (HT_USART0)
#define COM0_IRQn                   (USART0_IRQn)
#define COM0_IRQHandler             (USART0_IRQHandler)
 
#define COM0_TX_GPIO_ID             (GPIO_PB)
#define COM0_TX_AFIO_PIN            (AFIO_PIN_0)
#define COM0_TX_AFIO_MODE           (AFIO_FUN_USART_UART)
 
#define COM0_RX_GPIO_ID             (GPIO_PB)
#define COM0_RX_AFIO_PIN            (AFIO_PIN_1)
#define COM0_RX_AFIO_MODE           (AFIO_FUN_USART_UART)


/* Exported variables --------------------------------------------------------------- */
extern u8  COM0_RX_BUF[COM0_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 COM0_RX_STA;         		//接收状态标记	

/* Exported function prototypes ------------------------------------------------------------*/
void ESP8266_Init();
void COM0_SendStr(HT_USART_TypeDef* USARTx, uint8_t *str);

#endif /* __ESP8266_H */

#ifndef _IT_INCLUDE_H
#define _IT_INCLUDE_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "usart1.h"
#include "esp8266.h"

/* Private define ------------------------------------------------------------------------------------------*/
#define USART_TIMEOUT_ON          ((u32)0x00000080)
#define USART_BUFSIZE   128


/* Exported variables --------------------------------------------------------------------------------------*/
extern u8 URTxBuf[USART_BUFSIZE];
extern u8 URRxBuf[USART_BUFSIZE];
extern vu32 URTxReadIndex;
extern vu32 URTxWriteIndex;
extern vu32 URRxWriteIndex;

/* Exported function prototypes -----------------------------------------------------------------------*/
extern void TimingDelay_Decrement(void);
#endif

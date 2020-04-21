#ifndef _MAIN_INCLUDE__H
#define _MAIN_INCLUDE__H

/* Includes --------------------------------------------------------------------------------*/
#include "ht32.h"
#include "it_include.h"
#include <string.h>
#include "usart1.h"
#include "systick.h"
#include "led.h"
#include "sht3x_iic.h"
#include "sgp30_iic.h"
#include "bh1750_iic.h"
#include "bmp180_iic.h"
#include "oled_spi.h"
#include "nrf24_spi.h"

/* Private function prototypes -------------------------------------------------------------*/


/* Private variables -----------------------------------------------------------------------*/
const char HelloString[] = "Hello, this is USART Tx/Rx interrupt example. Please enter 15 characters...\r\n";

/* Exported variables ----------------------------------------------------------------------*/

/* Exported function prototypes ------------------------------------------------------------*/
void NVIC_Configuration(void);
void APP_Configuration(void);

#endif

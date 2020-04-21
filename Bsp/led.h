#ifndef _LED_H
#define _LED_H
 
#include "ht32f5xxxx_01.h"


/** @addtogroup LED
  * @{
  */
#define USE_LED_APP					1
#define LEDn                        (3)

#define LED1_GPIO_ID                (GPIO_PC)
#define LED1_GPIO_PIN               (GPIO_PIN_14)
#define LED1_AFIO_MODE              (AFIO_FUN_GPIO)

#define LED2_GPIO_ID                (GPIO_PC)
#define LED2_GPIO_PIN               (GPIO_PIN_15)
#define LED2_AFIO_MODE              (AFIO_FUN_GPIO)

#define LED3_GPIO_ID                (GPIO_PC)
#define LED3_GPIO_PIN               (GPIO_PIN_1)
#define LED3_AFIO_MODE              (AFIO_FUN_GPIO)


typedef enum
{
  HT_LED1 = 0,
  HT_LED2 = 1,
  HT_LED3 = 2
} LED_TypeDef;
 
/* Private function prototypes -----------------------------------------------------------------------------*/
static void _HT32F_ClockConfig(u32 GpioId);
 
void HT32F_LEDInit(LED_TypeDef HT_LEDn);
void HT32F_LEDOn(LED_TypeDef HT_LEDn);
void HT32F_LEDOff(LED_TypeDef HT_LEDn);
void HT32F_LEDToggle(LED_TypeDef HT_LEDn); 
#endif


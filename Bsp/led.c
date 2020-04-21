#include "led.h"


HT_GPIO_TypeDef* const GPIO_PORT[GPIO_PORT_NUM] = {
    HT_GPIOA,
    HT_GPIOB,
    #if (LIBCFG_GPIOC)
    HT_GPIOC,
    #else
    0,
    #endif
  };

const u8 LED_GPIO_ID[LEDn] = {LED1_GPIO_ID, LED2_GPIO_ID, LED3_GPIO_ID};
const u16 LED_GPIO_PIN[LEDn] = {LED1_GPIO_PIN, LED2_GPIO_PIN, LED3_GPIO_PIN};
const AFIO_MODE_Enum LED_AFIO_MODE[LEDn] = {LED1_AFIO_MODE, LED2_AFIO_MODE, LED3_AFIO_MODE};


void HT32F_LEDInit(LED_TypeDef HT_LEDn)
{
  u32 bGpioId = LED_GPIO_ID[HT_LEDn];
  u32 uGPIOPin = LED_GPIO_PIN[HT_LEDn];
  AFIO_MODE_Enum uGPIOMode = LED_AFIO_MODE[HT_LEDn];
  HT_GPIO_TypeDef* GPIOx = GPIO_PORT[bGpioId];

  /* Enable the GPIO_LED Clock                                                                              */
  /* Enable the AFIO Clock                                                                                  */
  _HT32F_ClockConfig(bGpioId);

  /* Configure the AFIO mode                                                                                */
  AFIO_GPxConfig(bGpioId, uGPIOPin, uGPIOMode);

  /* LED is off by default                                                                                  */
  HT32F_LEDOff(HT_LEDn);

  /* Configure the GPIO pin                                                                                 */
  GPIO_PullResistorConfig(GPIOx, uGPIOPin, GPIO_PR_DISABLE);
  GPIO_DriveConfig(GPIOx, uGPIOPin, GPIO_DV_8MA);
  GPIO_DirectionConfig(GPIOx, uGPIOPin, GPIO_DIR_OUT);
}

void HT32F_LEDOn(LED_TypeDef HT_LEDn)
{
  GPIO_PORT[LED_GPIO_ID[HT_LEDn]]->RR = LED_GPIO_PIN[HT_LEDn];
}


void HT32F_LEDOff(LED_TypeDef HT_LEDn)
{
  GPIO_PORT[LED_GPIO_ID[HT_LEDn]]->SRR = LED_GPIO_PIN[HT_LEDn];
}


void HT32F_LEDToggle(LED_TypeDef HT_LEDn)
{
  GPIO_PORT[LED_GPIO_ID[HT_LEDn]]->DOUTR ^= LED_GPIO_PIN[HT_LEDn];
}

/*********************************************************************************************************//**
 * @brief Enable GPIO and AFIO Clock.
 * @param  GpioId: Specifies the GPIO ID.
 * @retval None
 ************************************************************************************************************/
static void _HT32F_ClockConfig(u32 GpioId)
{
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  u8 RegCK[GPIO_PORT_NUM] = {0};
  RegCK[GpioId] = 1;

  CKCUClock.Bit.PA         = RegCK[0];
  CKCUClock.Bit.PB         = RegCK[1];

  CKCUClock.Bit.PC         = RegCK[2];


  CKCUClock.Bit.AFIO       = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
}

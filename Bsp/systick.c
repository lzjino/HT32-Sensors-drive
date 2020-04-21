#include "systick.h"
#include "ht32_board.h"

/* Private variables ---------------------------------------------------------------------------------------*/
static vu32 DelayTime;


/*********************************************************************************************************//**
  * @brief  Insert delay_us time.
  * @param  nTime: Insert Delay time.
  * @retval None
  ***********************************************************************************************************/
void delay_us(u32 nTime)
{
	/* SYSTICK configuration */
  SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK);       // Default : CK_SYS/8
  SYSTICK_SetReloadValue(SystemCoreClock / 8 / 1000000); // (CK_SYS/8/1000000) = 1us on chip
  SYSTICK_IntConfig(ENABLE);                          // Enable SYSTICK Interrupt

  /* Enable the SysTick Counter */
  SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
  SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);

  DelayTime = nTime;

  while(DelayTime != 0);

  /* Disable SysTick Counter */
  SYSTICK_CounterCmd(SYSTICK_COUNTER_DISABLE);
  /* Clear SysTick Counter */
  SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
}
/*********************************************************************************************************//**
  * @brief  Insert delay_ms time.
  * @param  nTime: Insert Delay time.
  * @retval None
  ***********************************************************************************************************/

void delay_ms(u32 nTime)
{
	/* SYSTICK configuration */
  SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK);       // Default : CK_SYS/8
  SYSTICK_SetReloadValue(SystemCoreClock / 8 / 1000); // (CK_SYS/8/1000) = 1ms on chip
  SYSTICK_IntConfig(ENABLE);                          // Enable SYSTICK Interrupt

  /* Enable the SysTick Counter */
  SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
  SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);

  DelayTime = nTime;

  while(DelayTime != 0);

  /* Disable SysTick Counter */
  SYSTICK_CounterCmd(SYSTICK_COUNTER_DISABLE);
  /* Clear SysTick Counter */
  SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
}

void TimingDelay_Decrement(void)
{
  if(DelayTime != 0)
  {
    DelayTime--;
  }
}

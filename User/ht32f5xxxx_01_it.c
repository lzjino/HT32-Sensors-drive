//�����жϷ�����USART \SysTick
/* Includes ------------------------------------------------------------------------------------------------*/
#include "it_include.h"



/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief   This function handles NMI exception.
 * @retval  None
 ************************************************************************************************************/
void NMI_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Hard Fault exception.
 * @retval  None
 ************************************************************************************************************/
void HardFault_Handler(void)
{
  #if 1

  static vu32 gIsContinue = 0;
  /*--------------------------------------------------------------------------------------------------------*/
  /* For development FW, MCU run into the while loop when the hardfault occurred.                           */
  /* 1. Stack Checking                                                                                      */
  /*    When a hard fault exception occurs, MCU push following register into the stack (main or process     */
  /*    stack). Confirm R13(SP) value in the Register Window and typing it to the Memory Windows, you can   */
  /*    check following register, especially the PC value (indicate the last instruction before hard fault).*/
  /*    SP + 0x00    0x04    0x08    0x0C    0x10    0x14    0x18    0x1C                                   */
  /*           R0      R1      R2      R3     R12      LR      PC    xPSR                                   */
  while (gIsContinue == 0)
  {
  }
  /* 2. Step Out to Find the Clue                                                                           */
  /*    Change the variable "gIsContinue" to any other value than zero in a Local or Watch Window, then     */
  /*    step out the HardFault_Handler to reach the first instruction after the instruction which caused    */
  /*    the hard fault.                                                                                     */
  /*--------------------------------------------------------------------------------------------------------*/

  #else

  /*--------------------------------------------------------------------------------------------------------*/
  /* For production FW, you shall consider to reboot the system when hardfault occurred.                    */
  /*--------------------------------------------------------------------------------------------------------*/
  NVIC_SystemReset();

  #endif
}

/*********************************************************************************************************//**
 * @brief   This function handles SVCall exception.
 * @retval  None
 ************************************************************************************************************/
void SVC_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles PendSVC exception.
 * @retval  None
 ************************************************************************************************************/
void PendSV_Handler(void)
{
}



/*********************************************************************************************************//**
 * @brief   This function handles SysTick Handler.
 * @retval  None
 ************************************************************************************************************/
void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}

/* USART1 definitions                                                                                         */
u8 URTxBuf[USART_BUFSIZE] = {0x41, 0x42, 0x43, 0x44, 0x45};
u8 URRxBuf[USART_BUFSIZE];
vu32 URTxReadIndex = 0;
vu32 URTxWriteIndex = 0;
vu32 URRxWriteIndex = 0;


/*********************************************************************************************************//**
 * @brief   This function handles USARTx interrupt.
 * @retval  None
 ************************************************************************************************************/
void COM1_IRQHandler(void)
{
	/* Rx, move data from USART1 to buffer                      */
	u8 Res;
	if(USART_GetFlagStatus(COM1_PORT, USART_FLAG_RXDR) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(COM1_PORT);	//��ȡ���յ�������
		
		URRxBuf[URRxWriteIndex] = USART_ReceiveData(COM1_PORT);
		URRxWriteIndex = (URRxWriteIndex + 1) % USART_BUFSIZE;
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
     } 

}                                                                                        



/*********************************************************************************************************//**
 * @brief   This function handles USARTx interrupt.
 * @retval  None
 ************************************************************************************************************/
void COM0_IRQHandler(void)
{
	/* Rx, move data from USART0 to buffer                      */
	u8 Res;
	if(USART_GetFlagStatus(COM0_PORT, USART_FLAG_RXDR) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(COM0_PORT);	//��ȡ���յ�������
		
		if((COM0_RX_STA&0x8000)==0)//����δ���
		{
			if(COM0_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)COM0_RX_STA=0;//���մ���,���¿�ʼ
				else COM0_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)COM0_RX_STA|=0x4000;
				else
					{
					COM0_RX_BUF[COM0_RX_STA&0X3FFF]=Res ;
					COM0_RX_STA++;
					if(COM0_RX_STA>(COM0_REC_LEN-1))COM0_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     }

}                                                                                        


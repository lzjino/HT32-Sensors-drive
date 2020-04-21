#include "usart1.h"

u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���

static void USART_CKCU_Config()
{
	CKCU_PeripClockConfig_TypeDef CCLOCK;
	
	CCLOCK.Bit.AFIO = 1;
	COM1_CLK(CCLOCK) = 1;
	
	CKCU_PeripClockConfig(CCLOCK, ENABLE);
}
 
static void USART_AFIO_Config()
{
	AFIO_GPxConfig(COM1_TX_GPIO_ID, COM1_TX_AFIO_PIN, AFIO_FUN_USART_UART);
	AFIO_GPxConfig(COM1_RX_GPIO_ID, COM1_RX_AFIO_PIN, AFIO_FUN_USART_UART);
}
 
static void USARTx_Config()
{
	USART_InitTypeDef USART_InitStructure;
	
	/*USART��ʼ��
	*�����ʣ�115200
	*�ֽڳ��ȣ�8λ
	*ֹͣλ��1λ
	*У��λ����			
	*ģʽ������ģʽ
	**/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure.USART_Parity = USART_PARITY_NO;
	USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
	USART_Init(COM1_PORT  , &USART_InitStructure);
	
	/* ����USART�жϱ�־ */
	USART_IntConfig(COM1_PORT, USART_INT_RXDR, ENABLE);
	/* ʹ��USART���ա����� */
	USART_TxCmd(COM1_PORT, ENABLE);
	USART_RxCmd(COM1_PORT, ENABLE);
	/* ��ʼ���ж� */
	NVIC_EnableIRQ(COM1_IRQn);
	
}
 
void COM1_Configuration()
{
	USART_CKCU_Config();
	USART_AFIO_Config();
	USARTx_Config();
}

 
// ����һ���ֽ�
void Usart_Sendbyte(HT_USART_TypeDef* USARTx, u8 Data)
{
	USART_SendData(COM1_PORT, Data);
	// �ȴ��������ݼĴ������
	while (USART_GetFlagStatus(COM1_PORT, USART_FLAG_TXDE) == RESET);		
}
 
//����ÿ��Ԫ�����ݳ���Ϊ8λ������
void Usart_SendArray(HT_USART_TypeDef* USARTx, u8 *array,u8 num)
{
	u8 i;
	for( i = 0;i < num;i++)
	{
		Usart_Sendbyte(USARTx,*array);
		array++;
	}
}
 
// �����ַ���
void Usart_SendStr(HT_USART_TypeDef* USARTx, uint8_t *str)
{
	uint8_t i;
	for(i = 0;str[i] != '\0';i++)
	{
		Usart_Sendbyte(USARTx,str[i]);
	}
}
 
// �ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(COM1_PORT, (uint8_t) ch);
		
	/* �ȴ�������� */
	while (USART_GetFlagStatus(COM1_PORT, USART_FLAG_TXDE) == RESET);		
	
	return (ch);
}
 
// �ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
	/* �ȴ������������� */
	while (USART_GetFlagStatus(COM1_PORT, USART_FLAG_RXDNE) == RESET);
 
	return (int)USART_ReceiveData(COM1_PORT);
}

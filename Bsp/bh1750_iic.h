#ifndef _BH1750_IIC_H
#define _BH1750_IIC_H

/* Include --------------------------------------------------------------------------*/
#include "ht32.h"
#include "systick.h"
#include "usart1.h" 

/* Definition -----------------------------------------------------------------------*/
#define USE_BH1750_APP 					1

// PB7 - SCL
// PB8 - SDA
/* ����IIC��SCL���ӵ�GPIO�˿� */
#define BH_SCL_CLK(CK)						CK.Bit.PC						/* ����ʱ�ӿ���λ */
#define BH_SCL_GPIO_ID					GPIO_PB					/* GPIO�˿� */
#define BH_SCL_PIN						GPIO_PIN_3				/* ���ӵ�SCLʱ���ߵ�GPIO */
#define BH_SCL_AFIO_MODE				AFIO_FUN_GPIO
#define BH_SCL_PORT						HT_GPIOC
 
/* ����IIC��SDA���ӵ�GPIO�˿� */
#define BH_SDA_CLK(CK)						CK.Bit.PB						/* ����ʱ�ӿ���λ */

#define BH_SDA_GPIO_ID					GPIO_PB					/* GPIO�˿� */
#define BH_SDA_PIN						GPIO_PIN_6				/* ���ӵ�SDAʱ���ߵ�GPIO */
#define BH_SDA_AFIO_MODE				AFIO_FUN_GPIO
#define BH_SDA_PORT						HT_GPIOB

#define BH_ADDR_GPIO_ID					GPIO_PB					/* GPIO�˿� */
#define BH_ADDR_PIN						GPIO_PIN_6				/* ���ӵ�ADDRʱ���ߵ�GPIO */
#define BH_ADDR_AFIO_MODE				AFIO_FUN_GPIO
#define BH_ADDR_PORT					HT_GPIOB


#define BH_SDA_IN()    					{ GPIO_DirectionConfig(BH_SDA_PORT, BH_SDA_PIN, GPIO_DIR_IN);  \
											GPIO_InputConfig(BH_SDA_PORT, BH_SDA_PIN, ENABLE);       }	

#define BH_SDA_OUT()   					{ GPIO_DirectionConfig(BH_SDA_PORT, BH_SDA_PIN, GPIO_DIR_OUT);}			
	 
#define BH_SCL_H()  					GPIO_WriteOutBits(BH_SCL_PORT, BH_SCL_PIN, SET) 	//SCL���H
#define BH_SCL_L() 						GPIO_WriteOutBits(BH_SCL_PORT, BH_SCL_PIN, RESET)	//SCL���L
#define BH_SDA_H()  					GPIO_WriteOutBits(BH_SDA_PORT, BH_SDA_PIN, SET) 	//SDA���H
#define BH_SDA_L()  					GPIO_WriteOutBits(BH_SDA_PORT, BH_SDA_PIN, RESET)	//SDA���L
#define BH_READ_SDA()  					GPIO_ReadInBit(BH_SDA_PORT, BH_SDA_PIN)  //SDA���� 

#define BH1750_read  1           
#define BH1750_write 0 
#define	SlaveAddress   0x46				//����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
										//ALT  ADDRESS���Žӵ�ʱ��ַΪ0x46���ӵ�Դʱ��ַΪ0xB8
/* Exported functions ---------------------------------------------------------------*/

//IIC��������
void BH_IIC_Init(void);               			 	//��ʼ��IIC��IO��				 
void BH_IIC_Start(void);							//����IIC��ʼ�ź�
void BH_IIC_Stop(void);	  						//����IICֹͣ�ź�
void BH_IIC_Send_Byte(u8 txd);						//IIC����һ���ֽ�
u8 BH_IIC_Read_Byte(unsigned char ack);			//IIC��ȡһ���ֽ�
u8 BH_IIC_Wait_Ack(void); 							//IIC�ȴ�ACK�ź�
void BH_IIC_Ack(void);								//IIC����ACK�ź�
void BH_IIC_NAck(void);							//IIC������ACK�ź�

//BH1750��������       
void BH1750_Init(void);				  
void Single_Write_BH1750(u8 addr);
u16 Multiple_read_BH1750(void);
float BH1750_Read_Coversion(void);
#endif

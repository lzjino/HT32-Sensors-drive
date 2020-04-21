#ifndef __SGP30_IIC_H__
#define __SGP30_IIC_H__

#include "systick.h"
#include "usart1.h" 

#define USE_SGP30_APP					1
// PC12 - SCL
// PC13 - SDA
/* ����IIC��SCL���ӵ�GPIO�˿� */
#define SGP_ClK(CK)						CK.Bit.PC						/* ����ʱ�ӿ���λ */
#define SGP_SCL_GPIO_ID					GPIO_PC					/* GPIO�˿� */
#define SGP_SCL_PIN						GPIO_PIN_12				/* ���ӵ�SCLʱ���ߵ�GPIO */
#define SGP_SCL_AFIO_MODE				AFIO_FUN_GPIO
#define SGP_SCL_PORT					HT_GPIOC
 
/* ����IIC��SDA���ӵ�GPIO�˿� */
#define SGP_SDA_GPIO_ID					GPIO_PC					/* GPIO�˿� */
#define SGP_SDA_PIN						GPIO_PIN_13				/* ���ӵ�SCLʱ���ߵ�GPIO */
#define SGP_SDA_AFIO_MODE				AFIO_FUN_GPIO
#define SGP_SDA_PORT					HT_GPIOC


#define SGP_SDA_IN()    				{ GPIO_DirectionConfig(SGP_SDA_PORT, SGP_SDA_PIN, GPIO_DIR_IN);  \
											GPIO_InputConfig(SGP_SDA_PORT, SGP_SDA_PIN, ENABLE);       }	

#define SGP_SDA_OUT()   				{ GPIO_DirectionConfig(SGP_SDA_PORT, SGP_SDA_PIN, GPIO_DIR_OUT);}			
	 
#define SGP_SCL_H()  					GPIO_WriteOutBits(SGP_SCL_PORT, SGP_SCL_PIN, SET) 	//SCL���H
#define SGP_SCL_L() 					GPIO_WriteOutBits(SGP_SCL_PORT, SGP_SCL_PIN, RESET)	//SCL���L
#define SGP_SDA_H()  					GPIO_WriteOutBits(SGP_SDA_PORT, SGP_SDA_PIN, SET) 	//SDA���H
#define SGP_SDA_L()  					GPIO_WriteOutBits(SGP_SDA_PORT, SGP_SDA_PIN, RESET)	//SDA���L
#define SGP_READ_SDA()  				GPIO_ReadInBit(SGP_SDA_PORT, SGP_SDA_PIN)  //SDA���� 

#define SGP30_read  0xb1           
#define SGP30_write 0xb0 

//IIC���в�������

void SGP_IIC_Init(void);               			 //��ʼ��IIC��IO��				 
void SGP_IIC_Start(void);							//����IIC��ʼ�ź�
void SGP_IIC_Stop(void);	  						//����IICֹͣ�ź�
void SGP_IIC_Send_Byte(u8 txd);						//IIC����һ���ֽ�
u8 SGP_IIC_Read_Byte(unsigned char ack);			//IIC��ȡһ���ֽ�
u8 SGP_IIC_Wait_Ack(void); 							//IIC�ȴ�ACK�ź�
void SGP_IIC_Ack(void);							//IIC����ACK�ź�
void SGP_IIC_NAck(void);							//IIC������ACK�ź�

       
void SGP30_Init(void);				  
void SGP30_ad_write(u8 a, u8 b);
u32 SGP30_ad_read(void);


#endif


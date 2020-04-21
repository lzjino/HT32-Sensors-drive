#ifndef __SGP30_IIC_H__
#define __SGP30_IIC_H__

#include "systick.h"
#include "usart1.h" 

#define USE_SGP30_APP					1
// PC12 - SCL
// PC13 - SDA
/* 定义IIC的SCL连接的GPIO端口 */
#define SGP_ClK(CK)						CK.Bit.PC						/* 外设时钟控制位 */
#define SGP_SCL_GPIO_ID					GPIO_PC					/* GPIO端口 */
#define SGP_SCL_PIN						GPIO_PIN_12				/* 连接到SCL时钟线的GPIO */
#define SGP_SCL_AFIO_MODE				AFIO_FUN_GPIO
#define SGP_SCL_PORT					HT_GPIOC
 
/* 定义IIC的SDA连接的GPIO端口 */
#define SGP_SDA_GPIO_ID					GPIO_PC					/* GPIO端口 */
#define SGP_SDA_PIN						GPIO_PIN_13				/* 连接到SCL时钟线的GPIO */
#define SGP_SDA_AFIO_MODE				AFIO_FUN_GPIO
#define SGP_SDA_PORT					HT_GPIOC


#define SGP_SDA_IN()    				{ GPIO_DirectionConfig(SGP_SDA_PORT, SGP_SDA_PIN, GPIO_DIR_IN);  \
											GPIO_InputConfig(SGP_SDA_PORT, SGP_SDA_PIN, ENABLE);       }	

#define SGP_SDA_OUT()   				{ GPIO_DirectionConfig(SGP_SDA_PORT, SGP_SDA_PIN, GPIO_DIR_OUT);}			
	 
#define SGP_SCL_H()  					GPIO_WriteOutBits(SGP_SCL_PORT, SGP_SCL_PIN, SET) 	//SCL输出H
#define SGP_SCL_L() 					GPIO_WriteOutBits(SGP_SCL_PORT, SGP_SCL_PIN, RESET)	//SCL输出L
#define SGP_SDA_H()  					GPIO_WriteOutBits(SGP_SDA_PORT, SGP_SDA_PIN, SET) 	//SDA输出H
#define SGP_SDA_L()  					GPIO_WriteOutBits(SGP_SDA_PORT, SGP_SDA_PIN, RESET)	//SDA输出L
#define SGP_READ_SDA()  				GPIO_ReadInBit(SGP_SDA_PORT, SGP_SDA_PIN)  //SDA输入 

#define SGP30_read  0xb1           
#define SGP30_write 0xb0 

//IIC所有操作函数

void SGP_IIC_Init(void);               			 //初始化IIC的IO口				 
void SGP_IIC_Start(void);							//发送IIC开始信号
void SGP_IIC_Stop(void);	  						//发送IIC停止信号
void SGP_IIC_Send_Byte(u8 txd);						//IIC发送一个字节
u8 SGP_IIC_Read_Byte(unsigned char ack);			//IIC读取一个字节
u8 SGP_IIC_Wait_Ack(void); 							//IIC等待ACK信号
void SGP_IIC_Ack(void);							//IIC发送ACK信号
void SGP_IIC_NAck(void);							//IIC不发送ACK信号

       
void SGP30_Init(void);				  
void SGP30_ad_write(u8 a, u8 b);
u32 SGP30_ad_read(void);


#endif


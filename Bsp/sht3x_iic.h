#ifndef __SHT3X_IIC_H
#define __SHT3X_IIC_H

#include "ht32.h"
#include "systick.h"

#define USE_SHT31_APP					1

// PA8 - SCL
// PA9 - SDA
/* 定义IIC的SCL连接的GPIO端口 */
#define SHT_CLK(CK)						CK.Bit.PA						/* 外设时钟控制位 */
#define SHT_SCL_GPIO_ID					GPIO_PA					/* GPIO端口 */
#define SHT_SCL_PIN						GPIO_PIN_8				/* 连接到SCL时钟线的GPIO */
#define SHT_SCL_AFIO_MODE				AFIO_FUN_GPIO
#define SHT_SCL_PORT					HT_GPIOA
 
/* 定义IIC的SDA连接的GPIO端口 */
#define SHT_SDA_GPIO_ID					GPIO_PA					/* GPIO端口 */
#define SHT_SDA_PIN						GPIO_PIN_9				/* 连接到SCL时钟线的GPIO */
#define SHT_SDA_AFIO_MODE				AFIO_FUN_GPIO
#define SHT_SDA_PORT					HT_GPIOA


#define SHT_SDA_IN()    				{ GPIO_DirectionConfig(SHT_SDA_PORT, SHT_SDA_PIN, GPIO_DIR_IN);  \
											GPIO_InputConfig(SHT_SDA_PORT, SHT_SDA_PIN, ENABLE);       }	

#define SHT_SDA_OUT()   				{ GPIO_DirectionConfig(SHT_SDA_PORT, SHT_SDA_PIN, GPIO_DIR_OUT);}			
	 
#define SHT_SCL_H()  					GPIO_WriteOutBits(SHT_SCL_PORT, SHT_SCL_PIN, SET) 	//SCL输出H
#define SHT_SCL_L() 					GPIO_WriteOutBits(SHT_SCL_PORT, SHT_SCL_PIN, RESET)	//SCL输出L
#define SHT_SDA_H()  					GPIO_WriteOutBits(SHT_SDA_PORT, SHT_SDA_PIN, SET) 	//SDA输出H
#define SHT_SDA_L()  					GPIO_WriteOutBits(SHT_SDA_PORT, SHT_SDA_PIN, RESET)	//SDA输出L
#define SHT_READ_SDA()  				GPIO_ReadInBit(SHT_SDA_PORT, SHT_SDA_PIN)  //SDA输入 

#define Sht3x_write 0
#define Sht3x_read  1

//IIC所有操作函数

void SHT_IIC_Init(void);               			 //初始化IIC的IO口				 
void SHT_IIC_Start(void);							//发送IIC开始信号
void SHT_IIC_Stop(void);	  						//发送IIC停止信号
void SHT_IIC_Send_Byte(u8 txd);						//IIC发送一个字节
u8 SHT_IIC_Read_Byte(unsigned char ack);			//IIC读取一个字节
u8 SHT_IIC_Wait_Ack(void); 							//IIC等待ACK信号
void SHT_IIC_Ack(void);							//IIC发送ACK信号
void SHT_IIC_NAck(void);			

extern double SHT_Temp;
extern double SHT_Humi;

extern void Sht3x_Init(void);
extern void Sth3x_ReadResult(u8 addr);
u8 sht30_crc8_check(u8 *check_data,u8 num_of_data,u8 CrcData);

#endif

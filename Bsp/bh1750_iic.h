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
/* 定义IIC的SCL连接的GPIO端口 */
#define BH_SCL_CLK(CK)						CK.Bit.PC						/* 外设时钟控制位 */
#define BH_SCL_GPIO_ID					GPIO_PB					/* GPIO端口 */
#define BH_SCL_PIN						GPIO_PIN_3				/* 连接到SCL时钟线的GPIO */
#define BH_SCL_AFIO_MODE				AFIO_FUN_GPIO
#define BH_SCL_PORT						HT_GPIOC
 
/* 定义IIC的SDA连接的GPIO端口 */
#define BH_SDA_CLK(CK)						CK.Bit.PB						/* 外设时钟控制位 */

#define BH_SDA_GPIO_ID					GPIO_PB					/* GPIO端口 */
#define BH_SDA_PIN						GPIO_PIN_6				/* 连接到SDA时钟线的GPIO */
#define BH_SDA_AFIO_MODE				AFIO_FUN_GPIO
#define BH_SDA_PORT						HT_GPIOB

#define BH_ADDR_GPIO_ID					GPIO_PB					/* GPIO端口 */
#define BH_ADDR_PIN						GPIO_PIN_6				/* 连接到ADDR时钟线的GPIO */
#define BH_ADDR_AFIO_MODE				AFIO_FUN_GPIO
#define BH_ADDR_PORT					HT_GPIOB


#define BH_SDA_IN()    					{ GPIO_DirectionConfig(BH_SDA_PORT, BH_SDA_PIN, GPIO_DIR_IN);  \
											GPIO_InputConfig(BH_SDA_PORT, BH_SDA_PIN, ENABLE);       }	

#define BH_SDA_OUT()   					{ GPIO_DirectionConfig(BH_SDA_PORT, BH_SDA_PIN, GPIO_DIR_OUT);}			
	 
#define BH_SCL_H()  					GPIO_WriteOutBits(BH_SCL_PORT, BH_SCL_PIN, SET) 	//SCL输出H
#define BH_SCL_L() 						GPIO_WriteOutBits(BH_SCL_PORT, BH_SCL_PIN, RESET)	//SCL输出L
#define BH_SDA_H()  					GPIO_WriteOutBits(BH_SDA_PORT, BH_SDA_PIN, SET) 	//SDA输出H
#define BH_SDA_L()  					GPIO_WriteOutBits(BH_SDA_PORT, BH_SDA_PIN, RESET)	//SDA输出L
#define BH_READ_SDA()  					GPIO_ReadInBit(BH_SDA_PORT, BH_SDA_PIN)  //SDA输入 

#define BH1750_read  1           
#define BH1750_write 0 
#define	SlaveAddress   0x46				//定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
										//ALT  ADDRESS引脚接地时地址为0x46，接电源时地址为0xB8
/* Exported functions ---------------------------------------------------------------*/

//IIC操作函数
void BH_IIC_Init(void);               			 	//初始化IIC的IO口				 
void BH_IIC_Start(void);							//发送IIC开始信号
void BH_IIC_Stop(void);	  						//发送IIC停止信号
void BH_IIC_Send_Byte(u8 txd);						//IIC发送一个字节
u8 BH_IIC_Read_Byte(unsigned char ack);			//IIC读取一个字节
u8 BH_IIC_Wait_Ack(void); 							//IIC等待ACK信号
void BH_IIC_Ack(void);								//IIC发送ACK信号
void BH_IIC_NAck(void);							//IIC不发送ACK信号

//BH1750操作函数       
void BH1750_Init(void);				  
void Single_Write_BH1750(u8 addr);
u16 Multiple_read_BH1750(void);
float BH1750_Read_Coversion(void);
#endif

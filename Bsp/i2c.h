#ifndef __I2C_H
#define __I2C_H

#include "ht32.h"
#include "systick.h"


/* 定义I2C的SCL连接的GPIO端口 */
#define I2C_SCL_GPIO_ID					GPIO_PA					/* GPIO端口 */
#define I2C_SCL_PIN						GPIO_PIN_0				/* 连接到SCL时钟线的GPIO */
#define I2C_SCL_AFIO_MODE				AFIO_FUN_GPIO
#define I2C_SCL_PORT					HT_GPIOA
 
/* 定义I2C的SDA连接的GPIO端口 */
#define I2C_SDA_GPIO_ID					GPIO_PA					/* GPIO端口 */
#define I2C_SDA_PIN						GPIO_PIN_1				/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_AFIO_MODE				AFIO_FUN_GPIO
#define I2C_SDA_PORT					HT_GPIOA
 
#define I2C_SCL_1()						GPIO_SetOutBits(I2C_SCL_PORT, I2C_SCL_PIN)
#define I2C_SCL_0()						GPIO_ClearOutBits(I2C_SCL_PORT, I2C_SCL_PIN)
	
#define I2C_SDA_1()						GPIO_SetOutBits(I2C_SDA_PORT, I2C_SDA_PIN)
#define I2C_SDA_0()						GPIO_ClearOutBits(I2C_SDA_PORT, I2C_SDA_PIN)
#define I2C_SDA_Direction_Input()		GPIO_DirectionConfig(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_DIR_IN)	
#define I2C_SDA_InputConfig()			GPIO_InputConfig(I2C_SDA_PORT, I2C_SDA_PIN, ENABLE)
#define I2C_SDA_InputDisable()			GPIO_InputConfig(I2C_SDA_PORT, I2C_SDA_PIN, DISABLE)
#define I2C_SDA_Direction_Output()		GPIO_DirectionConfig(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_DIR_OUT)
#define I2C_SDA_READ()					GPIO_ReadInBit(I2C_SDA_PORT, I2C_SDA_PIN)

#define DELAYTIME 						2
 
void I2C_Initia(void); 
void I2C_Start(void);
void I2C_Stop(void);
u8 I2C_WaitAck(void);
void I2C_SendByte(u8 _ucByte);
u8 I2C_ReadByte(void);
void I2C_Ack(void);
void I2C_NAck(void);
void Sth3x_ReadResult(u8 addr);
u8 CRC_Check(u8 *check_data,u8 num_of_data,u8 CrcData);
//u8 I2C_CheckDevice(u8 _Address);




#endif

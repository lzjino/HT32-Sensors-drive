#ifndef _BMP1750_IIC_H
#define _BMP1750_IIC_H

/* Include --------------------------------------------------------------------------*/
#include "ht32.h"
#include "systick.h"
#include "usart1.h" 

/* Definition -----------------------------------------------------------------------*/
#define USE_BMP180_APP 						1

// PC2 - SCL
// PC9 - SDA
/* ����IIC��SCL���ӵ�GPIO�˿� */
#define BMP_SCL_CLK(CK)						CK.Bit.PC						/* ����ʱ�ӿ���λ */
#define BMP_SCL_GPIO_ID						GPIO_PC					/* GPIO�˿� */
#define BMP_SCL_PIN							GPIO_PIN_2				/* ���ӵ�SCLʱ���ߵ�GPIO */
#define BMP_SCL_AFIO_MODE					AFIO_FUN_GPIO
#define BMP_SCL_PORT						HT_GPIOC
 
/* ����IIC��SDA���ӵ�GPIO�˿� */
#define BMP_SDA_GPIO_ID						GPIO_PC					/* GPIO�˿� */
#define BMP_SDA_PIN							GPIO_PIN_9				/* ���ӵ�SCLʱ���ߵ�GPIO */
#define BMP_SDA_AFIO_MODE					AFIO_FUN_GPIO
#define BMP_SDA_PORT						HT_GPIOC


#define BMP_SDA_IN()    					{ GPIO_DirectionConfig(BMP_SDA_PORT, BMP_SDA_PIN, GPIO_DIR_IN);  \
												GPIO_InputConfig(BMP_SDA_PORT, BMP_SDA_PIN, ENABLE);       }	

#define BMP_SDA_OUT()   					{ GPIO_DirectionConfig(BMP_SDA_PORT, BMP_SDA_PIN, GPIO_DIR_OUT);}			
	 
#define BMP_SCL_H()  						GPIO_WriteOutBits(BMP_SCL_PORT, BMP_SCL_PIN, SET) 	//SCL���H
#define BMP_SCL_L() 						GPIO_WriteOutBits(BMP_SCL_PORT, BMP_SCL_PIN, RESET)	//SCL���L
#define BMP_SDA_H()  						GPIO_WriteOutBits(BMP_SDA_PORT, BMP_SDA_PIN, SET) 	//SDA���H
#define BMP_SDA_L()  						GPIO_WriteOutBits(BMP_SDA_PORT, BMP_SDA_PIN, RESET)	//SDA���L
#define BMP_READ_SDA()  					GPIO_ReadInBit(BMP_SDA_PORT, BMP_SDA_PIN)  //SDA���� 

#define BMP180_RD 						0xef          
#define BMP180_WR 						0xee 
#define DelayTime						4				

/* Exported types ------------------------------------------------------------------*/
typedef struct __BMP180
{
	short AC1;
	short AC2;
	short AC3;
	unsigned short AC4;
	unsigned short AC5;
	unsigned short AC6;
	short B1;
	short B2;
	short MB;
	short MC;
	short MD;
	long UT;
	long UP;
	long X1;
	long X2;
	long X3;
	long B3;
	unsigned long B4;
	long B5;
	long B6;
	long B7;
	long p;
}_bmp180;

/* Exported variables ---------------------------------------------------------------*/
extern _bmp180 bmp180;

/* Exported functions ---------------------------------------------------------------*/

//IIC��������
void BMP_IIC_Init(void);               			 	//��ʼ��IIC��IO��				 
void BMP_IIC_Start(void);							//����IIC��ʼ�ź�
void BMP_IIC_Stop(void);	  						//����IICֹͣ�ź�
void BMP_IIC_Send_Byte(u8 txd);						//IIC����һ���ֽ�
u8 BMP_IIC_Read_Byte(unsigned char ack);			//IIC��ȡһ���ֽ�
u8 BMP_IIC_Wait_Ack(void); 							//IIC�ȴ�ACK�ź�
void BMP_IIC_Ack(void);								//IIC����ACK�ź�
void BMP_IIC_NAck(void);							//IIC������ACK�ź�

//BMP180��������       				  
void BMP_Init(void);
uint8_t BMP_ReadOneByte(uint8_t ReadAddr);
void BMP_WriteOneByte(uint8_t WriteAddr,uint8_t DataToWrite);
short BMP_ReadTwoByte(uint8_t ReadAddr);
void BMP_ReadCalibrationData(void);
long BMP_Read_UT(void);
long BMP_Read_UP(void);
void BMP180_Convert(long *temperature, long *pressure,float *altitide);


#endif

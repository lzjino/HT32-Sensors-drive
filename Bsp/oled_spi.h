#ifndef __OLED_SPI_H
#define __OLED_SPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Include --------------------------------------------------------------------------*/
#include "ht32.h"
#include "systick.h"
#include "stdlib.h"	 


/* Exported constants --------------------------------------------------------------------------------------*/
#define USE_OLED_APP 						1
#define OLED_MODE							0
//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define OLED_MODE 0
#define SIZE 12
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    


/* ����OLED��CS���ӵ�GPIO�˿� */
#define OLED_CS_CLK(CK)						CK.Bit.PB						/* ����ʱ�ӿ���λ */

#define OLED_CS_GPIO_ID						GPIO_PC					/* GPIO�˿� */
#define OLED_CS_PIN							GPIO_PIN_7				/* ���ӵ�CSʱ���ߵ�GPIO */
#define OLED_CS_AFIO_MODE					AFIO_FUN_GPIO
#define OLED_CS_PORT						HT_GPIOB
 
/* ����OLED��RST���ӵ�GPIO�˿� */
#define OLED_RST_CLK(CK)					CK.Bit.PA						/* ����ʱ�ӿ���λ */

#define OLED_RST_GPIO_ID					GPIO_PA					/* GPIO�˿� */
#define OLED_RST_PIN						GPIO_PIN_15				/* ���ӵ�CSʱ���ߵ�GPIO */
#define OLED_RST_AFIO_MODE					AFIO_FUN_GPIO
#define OLED_RST_PORT						HT_GPIOA

/* ����OLED��DC���ӵ�GPIO�˿� */
#define OLED_DC_CLK(CK)						CK.Bit.PC
#define OLED_DC_GPIO_ID						GPIO_PC					/* GPIO�˿� */
#define OLED_DC_PIN							GPIO_PIN_0				/* ���ӵ�CSʱ���ߵ�GPIO */
#define OLED_DC_AFIO_MODE					AFIO_FUN_GPIO
#define OLED_DC_PORT						HT_GPIOC


#define OLED_SPI 							HT_SPI1
#define OLED_SPI_CLK(CK)					CK.Bit.SPI1						/* ����ʱ�ӿ���λ */
/* ����OLED��SCLK(D0)���ӵ�SPI�˿� */
#define OLED_SCLK_GPIO_ID					GPIO_PC					/* GPIO�˿� */
#define OLED_SCLK_PIN						GPIO_PIN_5				/* ���ӵ�CSʱ���ߵ�GPIO */
#define OLED_SCLK_AFIO_MODE					AFIO_FUN_SPI
#define OLED_SCLK_PORT						HT_GPIOC
/* ����OLED��MOSI(D1)���ӵ�SPI�˿� */
#define OLED_MOSI_GPIO_ID					GPIO_PC					/* GPIO�˿� */
#define OLED_MOSI_PIN						GPIO_PIN_8				/* ���ӵ�CSʱ���ߵ�GPIO */
#define OLED_MOSI_AFIO_MODE					AFIO_FUN_SPI
#define OLED_MOSI_PORT						HT_GPIOC


//��������
///* ����OLED��WR���ӵ�GPIO�˿� */
//#define OLED_SPI_WR_CLK(CK)				CK.Bit.PB						/* ����ʱ�ӿ���λ */

//#define OLED_WR_GPIO_ID					GPIO_PB					/* GPIO�˿� */
//#define OLED_WR_PIN						GPIO_PIN_2				/* ���ӵ�CSʱ���ߵ�GPIO */
//#define OLED_WR_AFIO_MODE					AFIO_FUN_GPIO
//#define OLED_WR_PORT						HT_GPIOB

///* ����OLED��RD���ӵ�GPIO�˿� */
//#define OLED_SPI_RD_CLK(CK)				CK.Bit.PB						/* ����ʱ�ӿ���λ */

//#define OLED_RD_GPIO_ID					GPIO_PB					/* GPIO�˿� */
//#define OLED_RD_PIN						GPIO_PIN_3				/* ���ӵ�CSʱ���ߵ�GPIO */
//#define OLED_RD_AFIO_MODE					AFIO_FUN_GPIO
//#define OLED_RD_PORT						HT_GPIOB


	 
//-----------------OLED�˿ڶ���----------------  					   
#define OLED_CS_Clr()  GPIO_WriteOutBits(OLED_CS_PORT, OLED_CS_PIN, RESET)//CS
#define OLED_CS_Set()  GPIO_WriteOutBits(OLED_CS_PORT,OLED_CS_PIN,SET)

#define OLED_RST_Clr() GPIO_WriteOutBits(OLED_RST_PORT, OLED_RST_PIN, RESET)//RES
#define OLED_RST_Set() GPIO_WriteOutBits(OLED_RST_PORT, OLED_RST_PIN, SET)

#define OLED_DC_Clr() GPIO_WriteOutBits(OLED_DC_PORT, OLED_DC_PIN, RESET)//DC
#define OLED_DC_Set() GPIO_WriteOutBits(OLED_DC_PORT, OLED_DC_PIN, SET)

//#define OLED_WR_Clr() GPIO_WriteOutBits(OLED_WR_PORT, OLED_WR_PIN, RESET)
//#define OLED_WR_Set() GPIO_WriteOutBits(OLED_WR_PORT, OLED_WR_PIN, SET)

//#define OLED_RD_Clr() GPIO_WriteOutBits(OLED_RD_PORT, OLED_RD_PIN, RESET)
//#define OLED_RD_Set() GPIO_WriteOutBits(OLED_RD_PORT, OLED_RD_PIN, SET)


//PC0~7,��Ϊ������
#define DATAOUT(x) GPIO_Write(GPIOC,x);//���  
//ʹ��4�ߴ��нӿ�ʱʹ�� 

#define OLED_SCLK_Clr() GPIO_WriteOutBits(OLED_SCLK_PORT, OLED_SCLK_PIN, RESET)//SCLK
#define OLED_SCLK_Set() GPIO_WriteOutBits(OLED_SCLK_PORT, OLED_SCLK_PIN, SET)

#define OLED_MOSI_Clr() GPIO_WriteOutBits(OLED_MOSI_PORT, OLED_MOSI_PIN, RESET)//DIN
#define OLED_MOSI_Set() GPIO_WriteOutBits(OLED_MOSI_PORT, OLED_MOSI_PIN, SET)

// 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


//OLED�����ú���
u16 SPI_OLED_SendByte(u8 byte);
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, char *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowChinese(u8 x,u8 y,u8 no);
void OLED_DrawOLED(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char OLED[]);
 	 

/* Exported functions --------------------------------------------------------------------------------------*/
#ifdef __cplusplus
}	 
#endif

#endif

	 

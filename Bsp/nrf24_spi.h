#ifndef _NRF24_SPI_H
#define _NRF24_SPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Include --------------------------------------------------------------------------*/
#include "ht32.h"
#include "systick.h"
#include "stdlib.h"	 


/* Defined constants --------------------------------------------------------------------------------------*/
#define USE_NRF24_APP 						1

//NRF24L01�Ĵ�����������
#define NRF_READ_REG    0x00  //�����üĴ���,��5λΪ�Ĵ�����ַ
#define NRF_WRITE_REG   0x20  //д���üĴ���,��5λΪ�Ĵ�����ַ
#define RD_RX_PLOAD     0x61  //��RX��Ч����,1~32�ֽ�
#define WR_TX_PLOAD     0xA0  //дTX��Ч����,1~32�ֽ�
#define FLUSH_TX        0xE1  //���TX FIFO�Ĵ���.����ģʽ����
#define FLUSH_RX        0xE2  //���RX FIFO�Ĵ���.����ģʽ����
#define REUSE_TX_PL     0xE3  //����ʹ����һ������,CEΪ��,���ݰ������Ϸ���.
#define NOP             0xFF  //�ղ���,����������״̬�Ĵ���	
//SPI(NRF24L01)�Ĵ�����ַ
#define CONFIG          0x00  //���üĴ�����ַ;bit0:1����ģʽ,0����ģʽ;bit1:��ѡ��;bit2:CRCģʽ;bit3:CRCʹ��;
                              //bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��
#define EN_AA           0x01  //ʹ���Զ�Ӧ����  bit0~5,��Ӧͨ��0~5
#define EN_RXADDR       0x02  //���յ�ַ����,bit0~5,��Ӧͨ��0~5
#define SETUP_AW        0x03  //���õ�ַ���(��������ͨ��):bit1,0:00,3�ֽ�;01,4�ֽ�;02,5�ֽ�;
#define SETUP_RETR      0x04  //�����Զ��ط�;bit3:0,�Զ��ط�������;bit7:4,�Զ��ط���ʱ 250*x+86us
#define RF_CH           0x05  //RFͨ��,bit6:0,����ͨ��Ƶ��;
#define RF_SETUP        0x06  //RF�Ĵ���;bit3:��������(0:1Mbps,1:2Mbps);bit2:1,���书��;bit0:�������Ŵ�������
#define STATUS          0x07  //״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
                              //bit5:���ݷ�������ж�;bit6:���������ж�;
#define MAX_TX  		0x10  //�ﵽ����ʹ����ж�
#define TX_OK   		0x20  //TX��������ж�
#define RX_OK   		0x40  //���յ������ж�
#define OBSERVE_TX      0x08  //���ͼ��Ĵ���,bit7:4,���ݰ���ʧ������;bit3:0,�ط�������
#define CD              0x09  //�ز����Ĵ���,bit0,�ز����;
#define RX_ADDR_P0      0x0A  //����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P1      0x0B  //����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P2      0x0C  //����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P3      0x0D  //����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P4      0x0E  //����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P5      0x0F  //����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define TX_ADDR         0x10  //���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���
#define RX_PW_P0        0x11  //��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P1        0x12  //��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P2        0x13  //��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P3        0x14  //��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P4        0x15  //��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P5        0x16  //��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_FIFO_STATUS 0x17  //FIFO״̬�Ĵ���;bit0,RX FIFO�Ĵ����ձ�־;bit1,RX FIFO����־;bit2,3,����
                              //bit4,TX FIFO�ձ�־;bit5,TX FIFO����־;bit6,1,ѭ��������һ���ݰ�.0,��ѭ��;

//24L01���ͽ������ݿ�ȶ���
#define TX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH  32  	//32�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH  32  	//32�ֽڵ��û����ݿ��





/* ����NRF��CE���ӵ�GPIO�˿� */
#define NRF_CE_CLK(CK)						CK.Bit.PD						/* ����ʱ�ӿ���λ */
#define NRF_CE_GPIO_ID						GPIO_PD					/* GPIO�˿� */
#define NRF_CE_PIN							GPIO_PIN_1				/* ���ӵ�CEʱ���ߵ�GPIO */
#define NRF_CE_AFIO_MODE					AFIO_FUN_GPIO
#define NRF_CE_PORT							HT_GPIOD
 
/* ����NRF��IRQ���ӵ�GPIO�˿� */
#define NRF_IRQ_CLK(CK)						CK.Bit.PD
#define NRF_IRQ_GPIO_ID						GPIO_PD					/* GPIO�˿� */
#define NRF_IRQ_PIN							GPIO_PIN_2				/* ���ӵ�CSʱ���ߵ�GPIO */
#define NRF_IRQ_AFIO_MODE					AFIO_FUN_GPIO
#define NRF_IRQ_PORT						HT_GPIOD

/* SPI�˿� */
#define NRF_SPI 							HT_SPI0
#define NRF_SPI_CLK(CK)						CK.Bit.SPI0						/* ����ʱ�ӿ���λ */
/* ����NRF��SLK���ӵ�SPI�˿� */
#define NRF_SCK_GPIO_ID						GPIO_PB						/* GPIO�˿� */
#define NRF_SCK_PIN							GPIO_PIN_3					/* ���ӵ�SCKʱ���ߵ�GPIO */
#define NRF_SCK_AFIO_MODE					AFIO_FUN_SPI
#define NRF_SCK_PORT						HT_GPIOB
/* ����NRF��MOSI���ӵ�SPI�˿� */
#define NRF_MOSI_GPIO_ID					GPIO_PB					/* GPIO�˿� */
#define NRF_MOSI_PIN						GPIO_PIN_4				/* ���ӵ�MOSIʱ���ߵ�GPIO */
#define NRF_MOSI_AFIO_MODE					AFIO_FUN_SPI
#define NRF_MOSI_PORT						HT_GPIOB

/* ����NRF��MISO���ӵ�SPI�˿� */
#define NRF_MISO_GPIO_ID					GPIO_PB					/* GPIO�˿� */
#define NRF_MISO_PIN						GPIO_PIN_5				/* ���ӵ�MISOʱ���ߵ�GPIO */
#define NRF_MISO_AFIO_MODE					AFIO_FUN_SPI
#define NRF_MISO_PORT						HT_GPIOB

/* ����NRF��CSN(SEL)���ӵ�GPIO�˿� */
#define NRF_CSN_CLK(CK)						CK.Bit.PB						/* ����ʱ�ӿ���λ */
#define NRF_CSN_GPIO_ID						GPIO_PB					/* GPIO�˿� */
#define NRF_CSN_PIN							GPIO_PIN_2				/* ���ӵ�CSNʱ���ߵ�GPIO */
#define NRF_CSN_AFIO_MODE					AFIO_FUN_GPIO
#define NRF_CSN_PORT						HT_GPIOB

	 
//-----------------NRF�˿ڲ�������----------------  					   
#define NRF_CE_Clr()  	GPIO_WriteOutBits(NRF_CE_PORT, NRF_CE_PIN, RESET)//CE
#define NRF_CE_Set()  	GPIO_WriteOutBits(NRF_CE_PORT,NRF_CE_PIN,SET)

#define NRF_CSN_Clr()	 GPIO_WriteOutBits(NRF_CSN_PORT, NRF_CSN_PIN, RESET)//RES
#define NRF_CSN_Set()	 GPIO_WriteOutBits(NRF_CSN_PORT, NRF_CSN_PIN, SET)

#define NRF_IRQ_In() 	GPIO_ReadInBit(NRF_IRQ_PORT, NRF_IRQ_PIN) 


//#define NRF_SCK_Clr() GPIO_WriteOutBits(NRF_SCK_PORT, NRF_SCK_PIN, RESET)//SCK
//#define NRF_SCK_Set() GPIO_WriteOutBits(NRF_SCK_PORT, NRF_SCK_PIN, SET)

//#define NRF_MOSI_Clr() GPIO_WriteOutBits(NRF_MOSI_PORT, NRF_MOSI_PIN, RESET)//DIN
//#define NRF_MOSI_Set() GPIO_WriteOutBits(NRF_MOSI_PORT, NRF_MOSI_PIN, SET)




/* Exported function prototypes ------------------------------------------------------------*/
//NRF�����ú���
void 	NRF24L01_Init(void);													//NRF24L01��ʼ������
void 	NRF24L01_RX_Mode(u8 chl);											//����Ϊ����ģʽ����
void 	NRF24L01_TX_Mode(u8 chl);												//����Ϊ����ģʽ����
u8 		NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 u8s);	//д������
u8 		NRF24L01_Read_Buf(u8 reg, u8 *pBuf, u8 u8s);	//��������		  
u8		NRF24L01_Read_Reg(u8 reg);										//���Ĵ���
u8 		NRF24L01_Write_Reg(u8 reg, u8 value);					//д�Ĵ���
u8 		NRF24L01_Check(void);													//���24L01�Ƿ����
u8 		NRF24L01_TxPacket(u8 *txbuf);									//����һ����������
u8 		NRF24L01_RxPacket(u8 *rxbuf);									//����һ����������
u8		SPI1_ReadWriteByte(u8 TxData);
void 	SPI1_SetSpeed(u8 SPIspeed);

#ifdef __cplusplus
}	 
#endif


#endif /* _NRF24_SPI_H */

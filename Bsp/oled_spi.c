#include "oled_spi.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "systick.h"

//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 	
/**
  * @brief  SPI ʱ������
  * @param  None
  * @retval None
  */
static void SPI_CKCU_Config()
{
	CKCU_PeripClockConfig_TypeDef CCLOCK = {{0}};
	
	OLED_CS_CLK(CCLOCK)   		= 1;
	OLED_RST_CLK(CCLOCK)    	= 1;
	OLED_DC_CLK(CCLOCK)			= 1;
	OLED_SPI_CLK(CCLOCK)		= 1;
	CCLOCK.Bit.AFIO  			= 1;
	
	CKCU_PeripClockConfig(CCLOCK, ENABLE);
}

/**
  * @brief  SPI I/O����
  * @param  None
  * @retval None
  */ 
static void SPI_APIO_Config()
{
	//SPI �������
	AFIO_GPxConfig(OLED_SCLK_GPIO_ID, OLED_SCLK_PIN, OLED_SCLK_AFIO_MODE);
	AFIO_GPxConfig(OLED_MOSI_GPIO_ID, OLED_MOSI_PIN, OLED_MOSI_AFIO_MODE);

	//GPIO �������
	AFIO_GPxConfig(OLED_CS_GPIO_ID, OLED_CS_PIN, OLED_CS_AFIO_MODE);
	AFIO_GPxConfig(OLED_RST_GPIO_ID, OLED_RST_PIN, OLED_RST_AFIO_MODE);
	AFIO_GPxConfig(OLED_DC_GPIO_ID, OLED_DC_PIN, OLED_DC_AFIO_MODE);
	/* Configure the GPIO pin                                                                                 */
//	//SCLK
//	GPIO_PullResistorConfig(OLED_SCLK_PORT, OLED_SCLK_PIN, GPIO_PR_DISABLE);
//	GPIO_DriveConfig(OLED_SCLK_PORT, OLED_SCLK_PIN, GPIO_DV_8MA);
//	GPIO_DirectionConfig(OLED_SCLK_PORT, OLED_SCLK_PIN, GPIO_DIR_OUT);
//	//MOSI	
//	GPIO_PullResistorConfig(OLED_MOSI_PORT, OLED_MOSI_PIN, GPIO_PR_DISABLE);
//	GPIO_DriveConfig(OLED_MOSI_PORT, OLED_MOSI_PIN, GPIO_DV_8MA);
//	GPIO_DirectionConfig(OLED_MOSI_PORT, OLED_MOSI_PIN, GPIO_DIR_OUT);
	//CS
	GPIO_PullResistorConfig(OLED_CS_PORT, OLED_CS_PIN, GPIO_PR_DISABLE);
	GPIO_DriveConfig(OLED_CS_PORT, OLED_CS_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(OLED_CS_PORT, OLED_CS_PIN, GPIO_DIR_OUT);
	//RST
	GPIO_PullResistorConfig(OLED_RST_PORT, OLED_RST_PIN, GPIO_PR_DISABLE);
	GPIO_DriveConfig(OLED_RST_PORT, OLED_RST_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(OLED_RST_PORT, OLED_RST_PIN, GPIO_DIR_OUT);
	//DC
	GPIO_PullResistorConfig(OLED_DC_PORT, OLED_DC_PIN, GPIO_PR_DISABLE);
	GPIO_DriveConfig(OLED_DC_PORT, OLED_DC_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(OLED_DC_PORT, OLED_DC_PIN, GPIO_DIR_OUT);
}
static void SPI_Config()
{
	SPI_InitTypeDef  SPI_InitStructure;
	
	  /* SPI Configuration                                                                                      */
	SPI_InitStructure.SPI_Mode = SPI_MASTER;
	SPI_InitStructure.SPI_FIFO = SPI_FIFO_DISABLE;
	SPI_InitStructure.SPI_DataLength = SPI_DATALENGTH_8;
	SPI_InitStructure.SPI_SELMode = SPI_SEL_SOFTWARE;
	SPI_InitStructure.SPI_SELPolarity = SPI_SELPOLARITY_LOW;
	SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_LOW;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_FIRST;
	SPI_InitStructure.SPI_RxFIFOTriggerLevel = 1;
	SPI_InitStructure.SPI_TxFIFOTriggerLevel = 0;
	SPI_InitStructure.SPI_ClockPrescaler = 2;
	SPI_Init(OLED_SPI, &SPI_InitStructure);

	SPI_SELOutputCmd(OLED_SPI, ENABLE);	
	SPI_Cmd(OLED_SPI, ENABLE);
}	

/*********************************************************************************************************//**
  * @brief  Sends a byte through the SPI interface and return the byte received from the SPI bus.
  * @param  byte : byte to send.
  * @retval The value of the received byte.
  ***********************************************************************************************************/
u16 SPI_OLED_SendByte(u8 byte)
{
  /* Loop while Tx buffer register is empty                                                                 */
  while (!SPI_GetFlagStatus(OLED_SPI, SPI_FLAG_TXBE));

  /* Send byte through the SPIx peripheral                                                                  */
  SPI_SendData(OLED_SPI, byte);

  /* Loop while Rx is not empty                                                                             */
  while (!SPI_GetFlagStatus(OLED_SPI, SPI_FLAG_RXBNE));

  /* Return the byte read from the SPI                                                                      */
  return SPI_ReceiveData(OLED_SPI);
}

#if OLED_MODE==1

//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
//void OLED_WR_Byte(u8 dat,u8 cmd)
//{
//	DATAOUT(dat);	    
//	if(cmd)
//	  OLED_DC_Set();
//	else 
//	  OLED_DC_Clr();		   
//	OLED_CS_Clr();
//	OLED_WR_Clr();	 
//	OLED_WR_Set();
//	OLED_CS_Set();	  
//	OLED_DC_Set();	 
//} 	    	    
#else
//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
//void OLED_WR_Byte(u8 dat,u8 cmd)
//{	
//	u8 i;			  
//	if(cmd)
//	  OLED_DC_Set();
//	else 
//	  OLED_DC_Clr();		  
//	OLED_CS_Clr();
//	for(i=0;i<8;i++)
//	{			  
//		OLED_SCLK_Clr();
//		if(dat&0x80)
//		   OLED_MOSI_Set();
//		else 
//		   OLED_MOSI_Clr();
//		OLED_SCLK_Set();
//		dat<<=1;   
//	}				 		  
//	OLED_CS_Set();
//	OLED_DC_Set();   	  
//} 

//Ӳ��SPI1д��һ���ֽ�
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  	
	OLED_CS_Clr();
	/* Check on OLED_SPI Tx Buffer empty  */
	SPI_OLED_SendByte(dat);
	OLED_CS_Set();
	OLED_DC_Set();
}
#endif

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y+1);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
	}
} 
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x,u8 y,char *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//��ʾ����
void OLED_ShowChinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 


//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
 
 	 
// 	GPIO_InitTypeDef  GPIO_InitStructure;
// 	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PC,D,G�˿�ʱ��

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_3|GPIO_Pin_8;	 //PD3,PD6�������  
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
// 	GPIO_Init(GPIOD, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
// 	GPIO_SetBits(GPIOD,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_3|GPIO_Pin_8);	//PD3,PD6 �����

 #if OLED_MODE==1
 
// 	GPIO_InitStructure.GPIO_Pin =0xFF; //PC0~7 OUT�������
// 	GPIO_Init(GPIOC, &GPIO_InitStructure);
// 	GPIO_SetBits(GPIOC,0xFF); //PC0~7�����

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //PG13,14,15 OUT�������
// 	GPIO_Init(GPIOG, &GPIO_InitStructure);
// 	GPIO_SetBits(GPIOG,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);						 //PG13,14,15 OUT  �����

 #else
 	SPI_CKCU_Config();
	SPI_APIO_Config();
	SPI_Config();
 #endif


 
	OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
}  







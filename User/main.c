/* Includes ------------------------------------------------------------------------------------------------*/
#include "main_include.h"

char OLED_Buf[32],NRF_Buf1[32],NRF_Buf2[32];
u8 nrf1_send_ctr,nrf2_send_ctr;
float sh_temp,sh_humi;
long bmp_temp;
long bmp_pres;
float bmp_altitide;
u32 dat;
u16 co2Data,TVOCData;
float light;
u8 ID = 0;
s32 count = 0;
	
int main(void)
{
	
	
	
	COM1_Configuration();				/* COM1 configuration	*/
	  /* COM1 Tx                                                                                                */
	URTxWriteIndex = sizeof(HelloString);
	memcpy(URTxBuf, HelloString, sizeof(HelloString));
	/* URx Tx > URx Rx                                                                       */
	Usart_SendStr(COM1_PORT,URTxBuf);
	
	NVIC_Configuration();               /* NVIC configuration                                                 */
	APP_Configuration();
	                                                           
	
  


	while (1)
	{	  
		/* COM1 Rx.waiting for receive the fifth data,
		then move date from UR1RxBuf to UR1TxBuf.                                                               */
		memset(URTxBuf, 0, sizeof(URTxBuf));
		if (URRxWriteIndex >= 15)
		{
			memcpy(URTxBuf, URRxBuf, 15);
			URRxWriteIndex = 0;
			/* COM1 Tx                                                                                            */
			URTxWriteIndex = 15;
			Usart_SendStr(COM1_PORT,URTxBuf);
		  //USART_IntConfig(COM1_PORT, USART_INT_TXDE, ENABLE);	//不使用发送中断
		}
	
	
#if USE_SHT31_APP	
	Sth3x_ReadResult(0x44);	//sht3x 湿度读取
	sh_temp = SHT_Temp;
	sh_humi = SHT_Humi;	
	printf("temp: %2.1f humi: %2.1f\r\n",sh_temp,sh_humi);	
#endif
	
#if USE_SGP30_APP
	SGP30_ad_write(0x20,0x08);
	dat = SGP30_ad_read();
	co2Data = (dat & 0xffff0000) >> 16;
	TVOCData = dat & 0x0000ffff;		
	printf("TVOC:%d Ppb,co2:%d Ppm\r\n",TVOCData,co2Data);
#endif
	
#if USE_BH1750_APP
	light = BH1750_Read_Coversion();
	printf("light:%6.1f lx\r\n",light);
#endif

#if USE_BMP180_APP

	ID = BMP_ReadOneByte(0xd0);			//0xd0：BMP180的ID寄存器地址
	BMP180_Convert(&bmp_temp, &bmp_pres, &bmp_altitide);
	printf("ID: %d\t  temp: %ld.%ld C\t  Pressure: %ldPa\t Altitude: %.1f m\r\n",ID,bmp_temp/10,bmp_temp%10,bmp_pres,bmp_altitide);
#endif
	
	
#if USE_OLED_APP
	
	OLED_ShowString(0,0,"0.96' OLED TEST!");
	memset((char*)OLED_Buf, 0, sizeof(OLED_Buf));
	sprintf((char *)OLED_Buf,"temp:%5.1f C",sh_temp);
	OLED_ShowString(0,1,OLED_Buf);
	memset((char*)OLED_Buf, 0, sizeof(OLED_Buf));
	sprintf((char *)OLED_Buf,"humi:%5.1f %%",sh_humi);
	OLED_ShowString(0,2,OLED_Buf);
	memset((char*)OLED_Buf, 0, sizeof(OLED_Buf));
	sprintf((char *)OLED_Buf,"TVOC:%4d Ppb ",TVOCData);
	OLED_ShowString(0,3,OLED_Buf);
	memset((char*)OLED_Buf, 0, sizeof(OLED_Buf));
	sprintf((char *)OLED_Buf,"Co2:%5d Ppm ",co2Data);
	OLED_ShowString(0,4,OLED_Buf);
	memset((char*)OLED_Buf, 0, sizeof(OLED_Buf));
	sprintf((char *)OLED_Buf,"light:%7.1f lx",light);
	OLED_ShowString(0,5,OLED_Buf);
	memset((char*)OLED_Buf, 0, sizeof(OLED_Buf));
	sprintf((char *)OLED_Buf,"Pr:   %7ld Pa",bmp_pres);
	OLED_ShowString(0,6,OLED_Buf);

#endif

#if USE_NRF24_APP
	NRF24L01_TX_Mode(80);
	memset((char *)NRF_Buf1, 0, sizeof(NRF_Buf1));
	memset((char *)NRF_Buf2, 0, sizeof(NRF_Buf2));
	sprintf(NRF_Buf1, "#t:%5.1f#h:%5.1f#T:%4d", sh_temp, sh_humi,TVOCData);
	sprintf(NRF_Buf2, "#C:%5d#l:%7.1f#p:%7ld", co2Data, light,bmp_pres);
	if(*NRF_Buf1 != 0 && *NRF_Buf2 != 0 )
	{
		printf("nrf发送数据中...\r\n");
		if(NRF24L01_TxPacket((u8*)NRF_Buf1)== TX_OK)//数据发送
		{
			nrf1_send_ctr++;
			if(nrf1_send_ctr > 10)
			{
				nrf1_send_ctr = 0;
//				printf("NRF_Buf1达到最大重发次数\r\n");
//				break;
			}
		}
		delay_ms(20);
		if(NRF24L01_TxPacket((u8*)NRF_Buf2))//数据发送
		{
			nrf2_send_ctr++;
			if(nrf2_send_ctr > 10)
			{
				nrf2_send_ctr = 0;
//				printf("NRF_Buf2达到最大重发次数\r\n");
//				break;
			}
		}
		memset((char*)NRF_Buf1, 0, sizeof(NRF_Buf1));
		memset((char*)NRF_Buf2, 0, sizeof(NRF_Buf2));
	}

#endif
	printf("count:%d s\r\n\r\n",count++);
	
#if USE_LED_APP	
	HT32F_LEDOn(HT_LED1);
	HT32F_LEDOn(HT_LED2);
	delay_ms(250);		 
	HT32F_LEDOff(HT_LED2);
	delay_us(250000); 	
	HT32F_LEDOn(HT_LED2);
	HT32F_LEDOff(HT_LED1);
	delay_us(250000); 	
	HT32F_LEDOff(HT_LED2);
	delay_ms(250); 		
#endif
	
	}
}

/* 中断向量表入口地址设置		*/
void NVIC_Configuration(void)
{
  NVIC_SetVectorTable(NVIC_VECTTABLE_FLASH, 0x0);     /* Set the Vector Table base location at 0x00000000   */
}

void APP_Configuration(void)
{
#if USE_SHT31_APP
	Sht3x_Init();
#endif
#if USE_SGP30_APP
	SGP30_Init();
#endif
	
#if USE_BH1750_APP
	BH1750_Init();
#endif
#if USE_BMP180_APP
	BMP_Init();
#endif
#if USE_OLED_APP
	OLED_Init();
	OLED_Clear(); 
#endif
#if USE_NRF24_APP
	NRF24L01_Init();
	while(NRF24L01_Check())
	{
		printf("NRF24L01 INIT ERROR!\r\n");
		delay_ms(1000);
	}
	printf("nrf24l01 config success!\r\n");
#endif
#if USE_LED_APP
  /* Initialize LED1,LED2 on HT32 board                                                                          */
	HT32F_LEDInit(HT_LED1);
	HT32F_LEDInit(HT_LED2);
#endif
}



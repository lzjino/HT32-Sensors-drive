#include "nrf24_spi.h"

//��������
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���յ�ַ

/**
  * @brief  SPI ʱ������
  * @param  None
  * @retval None
  */
static void SPI_CKCU_Config()
{
	CKCU_PeripClockConfig_TypeDef CCLOCK = {{0}};
	
	NRF_CE_CLK(CCLOCK)   		= 1;
	NRF_IRQ_CLK(CCLOCK)			= 1;
	NRF_CSN_CLK(CCLOCK)			= 1;
	NRF_SPI_CLK(CCLOCK)			= 1;
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
	AFIO_GPxConfig(NRF_SCK_GPIO_ID, NRF_SCK_PIN, NRF_SCK_AFIO_MODE);
	AFIO_GPxConfig(NRF_MOSI_GPIO_ID, NRF_MOSI_PIN, NRF_MOSI_AFIO_MODE);
	AFIO_GPxConfig(NRF_MISO_GPIO_ID, NRF_MISO_PIN, NRF_MISO_AFIO_MODE);
	//GPIO �������
	AFIO_GPxConfig(NRF_CE_GPIO_ID, NRF_CE_PIN, NRF_CE_AFIO_MODE);
	AFIO_GPxConfig(NRF_CSN_GPIO_ID, NRF_CSN_PIN, NRF_CSN_AFIO_MODE);
	AFIO_GPxConfig(NRF_IRQ_GPIO_ID, NRF_IRQ_PIN, NRF_IRQ_AFIO_MODE);
	/* Configure the GPIO pin                                                                                 */
	//SCK
//	GPIO_PullResistorConfig(NRF_SCK_PORT, NRF_SCK_PIN, GPIO_PR_DISABLE);
//	GPIO_DriveConfig(NRF_SCK_PORT, NRF_SCK_PIN, GPIO_DV_8MA);
//	GPIO_DirectionConfig(NRF_SCK_PORT, NRF_SCK_PIN, GPIO_DIR_OUT);
//	//MOSI	
//	GPIO_PullResistorConfig(NRF_MOSI_PORT, NRF_MOSI_PIN, GPIO_PR_DISABLE);
//	GPIO_DriveConfig(NRF_MOSI_PORT, NRF_MOSI_PIN, GPIO_DV_8MA);
//	GPIO_DirectionConfig(NRF_MOSI_PORT, NRF_MOSI_PIN, GPIO_DIR_OUT);
//	//MISO	
//	GPIO_PullResistorConfig(NRF_MISO_PORT, NRF_MISO_PIN, GPIO_PR_DISABLE);
////	GPIO_DriveConfig(NRF_MISO_PORT, NRF_MISO_PIN, GPIO_DV_8MA);
//	GPIO_DirectionConfig(NRF_MISO_PORT, NRF_MISO_PIN, GPIO_DIR_IN);	//����ģʽ
//	GPIO_InputConfig(NRF_MISO_PORT, NRF_MISO_PIN, ENABLE);
	//CE
	GPIO_PullResistorConfig(NRF_CE_PORT, NRF_CE_PIN, GPIO_PR_DISABLE);
//	GPIO_DriveConfig(NRF_CE_PORT, NRF_CE_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(NRF_CE_PORT, NRF_CE_PIN, GPIO_DIR_OUT);
	//CSN
	GPIO_PullResistorConfig(NRF_CSN_PORT, NRF_CSN_PIN, GPIO_PR_DISABLE);
//	GPIO_DriveConfig(NRF_CSN_PORT, NRF_CSN_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(NRF_CSN_PORT, NRF_CSN_PIN, GPIO_DIR_OUT);
	//IRQ
//	GPIO_PullResistorConfig(NRF_IRQ_PORT, NRF_IRQ_PIN, GPIO_PR_DISABLE);
//	GPIO_DriveConfig(NRF_IRQ_PORT, NRF_IRQ_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(NRF_IRQ_PORT, NRF_IRQ_PIN, GPIO_DIR_IN);	//����ģʽ
	GPIO_InputConfig(NRF_IRQ_PORT, NRF_IRQ_PIN, ENABLE);
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
	SPI_InitStructure.SPI_ClockPrescaler = 6;	
	SPI_Init(NRF_SPI, &SPI_InitStructure);

	SPI_SELOutputCmd(NRF_SPI, ENABLE);

	SPI_Cmd(NRF_SPI, ENABLE);
	SPI1_ReadWriteByte(0xff);//��������	
}	

//NRF24L01��ʼ������
void NRF24L01_Init(void)
{
	SPI_CKCU_Config();
	SPI_APIO_Config();
	SPI_Config();		
	NRF_CE_Clr(); 																												//ʹ��24L01
	NRF_CSN_Set();																												//SPIƬѡȡ��	 
}

//SPI1�ֽڶ�д����
//������TxData(д���ֽ�)
//����ֵ����ȡ�ֽ�
u8	SPI1_ReadWriteByte(u8 TxData)
{
		u8 retry=0;				 	
		while (SPI_GetFlagStatus(NRF_SPI, SPI_FLAG_TXBE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
				retry++;
				if(retry>200)return 0;
		}			  
		SPI_SendData(NRF_SPI, TxData); //ͨ������SPIx����һ������
		retry=0;

		while (SPI_GetFlagStatus(NRF_SPI, SPI_FLAG_RXBNE) == RESET)//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
				retry++;
				if(retry>200)return 0;
		}	  						    
		return SPI_ReceiveData(NRF_SPI); //����ͨ��SPIx������յ�����	
}

//SPI1�ٶ����ú���
void SPI1_SetSpeed(u8 SPIspeed)
{
	NRF_SPI->CPR = (SPIspeed/2) - 1;
}


//NRF24L01��⺯��
//����ֵ��0�����ɹ���  1�����ʧ�ܣ�
u8	NRF24L01_Check(void)
{
		u8 buf[5] = {0xa5, 0xa5, 0xa5, 0xa5, 0xa5};
		u8 i;
		SPI1_SetSpeed(6); 				//spi�ٶ�Ϊ8Mhz��24L01�����SPIʱ��Ϊ10Mhz�� 
		NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ
		NRF24L01_Read_Buf(TX_ADDR,buf,5); 							//����д��ĵ�ַ
		for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
		if(i!=5)return 1;																//���24L01����	
		return 0;		 																		//��⵽24L01
}

//SPIд�Ĵ�������
//������reg���Ĵ�����ַ��  value��д��ֵ��
//����ֵ��״ֵ̬
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	NRF_CSN_Clr();                 		//ʹ��SPI����
  	status =SPI1_ReadWriteByte(reg);		//���ͼĴ����� 
  	SPI1_ReadWriteByte(value);      		//д��Ĵ�����ֵ
  	NRF_CSN_Set();                 		//��ֹSPI����	   
  	return(status);       							//����״ֵ̬
}

//��ȡSPI�Ĵ���ֵ
//������reg���Ĵ�����ַ��
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
	NRF_CSN_Clr();          					//ʹ��SPI����		
  	SPI1_ReadWriteByte(reg);   					//���ͼĴ�����
  	reg_val=SPI1_ReadWriteByte(0XFF);		//��ȡ�Ĵ�������
  	NRF_CSN_Set();          					//��ֹSPI����		    
  	return(reg_val);           					//����״ֵ̬
}	

//��ָ��λ�ö���ָ�����ȵ�����
//������reg���Ĵ�����ַ��  *pBuf������ָ�룩  len�����ݳ��ȣ�
//����ֵ����ȡֵ
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  	NRF_CSN_Clr();           				//ʹ��SPI����
  	status=SPI1_ReadWriteByte(reg);			//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
	for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI1_ReadWriteByte(0XFF);//��������
  	NRF_CSN_Set();       							//�ر�SPI����
  	return status;        							//���ض�����״ֵ̬
}

//��ָ��λ��дָ�����ȵ�����
//������reg���Ĵ�����ַ��  *pBuf������ָ�룩  len�����ݳ��ȣ�
//����ֵ����ȡֵ
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;	    
	NRF_CSN_Clr();          					//ʹ��SPI����
  	status = SPI1_ReadWriteByte(reg);		//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI1_ReadWriteByte(*pBuf++); //д������	 
  	NRF_CSN_Set();       						//�ر�SPI����
  	return status;          						//���ض�����״ֵ̬
}			

//**********************************************NRF24L01��������****************************************************

//����NRF24L01����һ������
//������txbuf�������������׵�ַ��
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
	SPI1_SetSpeed(6);								//spi�ٶ�Ϊ8Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	NRF_CE_Clr();
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);	//д���ݵ�TX BUF  32���ֽ�
	NRF_CE_Set();																				//��������	   
	while(NRF_IRQ_In()!=0);																//�ȴ��������
	sta=NRF24L01_Read_Reg(STATUS);  											//��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); 				//���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)																				//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);								//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(sta&TX_OK)																					//�������
	{
		return TX_OK;
	}
	return 0xff;																					//����ԭ����ʧ��
}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ�������������룩
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	SPI1_SetSpeed(6); 							//spi�ٶ�Ϊ8Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	sta=NRF24L01_Read_Reg(STATUS);  											//��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); 						//���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)//���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);			//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);								//���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;															//û�յ��κ�����
}	

//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void NRF24L01_RX_Mode(u8 chl)
{
	NRF_CE_Clr();	  
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    					//ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);					//ʹ��ͨ��0�Ľ��յ�ַ  	 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,chl);	    				//����RFͨ��Ƶ��		  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);					//����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);						//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  	NRF_CE_Set(); 																					//CEΪ��,�������ģʽ 
}		

//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void NRF24L01_TX_Mode(u8 chl)
{														 
	NRF_CE_Clr();	    
  	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);			//дTX�ڵ��ַ 
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); 	//����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     													//ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); 													//ʹ��ͨ��0�Ľ��յ�ַ  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);													//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,chl);       													//����RFͨ��Ϊ40
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  													//����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    													//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	NRF_CE_Set();																															//CEΪ��,10us����������
}


#include "nrf24_spi.h"

//变量定义
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //接收地址

/**
  * @brief  SPI 时钟配置
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
  * @brief  SPI I/O配置
  * @param  None
  * @retval None
  */ 
static void SPI_APIO_Config()
{
	//SPI 相关引脚
	AFIO_GPxConfig(NRF_SCK_GPIO_ID, NRF_SCK_PIN, NRF_SCK_AFIO_MODE);
	AFIO_GPxConfig(NRF_MOSI_GPIO_ID, NRF_MOSI_PIN, NRF_MOSI_AFIO_MODE);
	AFIO_GPxConfig(NRF_MISO_GPIO_ID, NRF_MISO_PIN, NRF_MISO_AFIO_MODE);
	//GPIO 相关引脚
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
//	GPIO_DirectionConfig(NRF_MISO_PORT, NRF_MISO_PIN, GPIO_DIR_IN);	//输入模式
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
	GPIO_DirectionConfig(NRF_IRQ_PORT, NRF_IRQ_PIN, GPIO_DIR_IN);	//输入模式
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
	SPI1_ReadWriteByte(0xff);//启动传输	
}	

//NRF24L01初始化函数
void NRF24L01_Init(void)
{
	SPI_CKCU_Config();
	SPI_APIO_Config();
	SPI_Config();		
	NRF_CE_Clr(); 																												//使能24L01
	NRF_CSN_Set();																												//SPI片选取消	 
}

//SPI1字节读写函数
//参数：TxData(写入字节)
//返回值：读取字节
u8	SPI1_ReadWriteByte(u8 TxData)
{
		u8 retry=0;				 	
		while (SPI_GetFlagStatus(NRF_SPI, SPI_FLAG_TXBE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
				retry++;
				if(retry>200)return 0;
		}			  
		SPI_SendData(NRF_SPI, TxData); //通过外设SPIx发送一个数据
		retry=0;

		while (SPI_GetFlagStatus(NRF_SPI, SPI_FLAG_RXBNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
				retry++;
				if(retry>200)return 0;
		}	  						    
		return SPI_ReceiveData(NRF_SPI); //返回通过SPIx最近接收的数据	
}

//SPI1速度设置函数
void SPI1_SetSpeed(u8 SPIspeed)
{
	NRF_SPI->CPR = (SPIspeed/2) - 1;
}


//NRF24L01检测函数
//返回值：0（检测成功）  1（检测失败）
u8	NRF24L01_Check(void)
{
		u8 buf[5] = {0xa5, 0xa5, 0xa5, 0xa5, 0xa5};
		u8 i;
		SPI1_SetSpeed(6); 				//spi速度为8Mhz（24L01的最大SPI时钟为10Mhz） 
		NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址
		NRF24L01_Read_Buf(TX_ADDR,buf,5); 							//读出写入的地址
		for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
		if(i!=5)return 1;																//检测24L01错误	
		return 0;		 																		//检测到24L01
}

//SPI写寄存器函数
//参数：reg（寄存器地址）  value（写入值）
//返回值：状态值
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	NRF_CSN_Clr();                 		//使能SPI传输
  	status =SPI1_ReadWriteByte(reg);		//发送寄存器号 
  	SPI1_ReadWriteByte(value);      		//写入寄存器的值
  	NRF_CSN_Set();                 		//禁止SPI传输	   
  	return(status);       							//返回状态值
}

//读取SPI寄存器值
//参数：reg（寄存器地址）
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
	NRF_CSN_Clr();          					//使能SPI传输		
  	SPI1_ReadWriteByte(reg);   					//发送寄存器号
  	reg_val=SPI1_ReadWriteByte(0XFF);		//读取寄存器内容
  	NRF_CSN_Set();          					//禁止SPI传输		    
  	return(reg_val);           					//返回状态值
}	

//在指定位置读出指定长度的数据
//参数：reg（寄存器地址）  *pBuf（数据指针）  len（数据长度）
//返回值：读取值
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  	NRF_CSN_Clr();           				//使能SPI传输
  	status=SPI1_ReadWriteByte(reg);			//发送寄存器值(位置),并读取状态值   	   
	for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI1_ReadWriteByte(0XFF);//读出数据
  	NRF_CSN_Set();       							//关闭SPI传输
  	return status;        							//返回读到的状态值
}

//在指定位置写指定长度的数据
//参数：reg（寄存器地址）  *pBuf（数据指针）  len（数据长度）
//返回值：读取值
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;	    
	NRF_CSN_Clr();          					//使能SPI传输
  	status = SPI1_ReadWriteByte(reg);		//发送寄存器值(位置),并读取状态值
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI1_ReadWriteByte(*pBuf++); //写入数据	 
  	NRF_CSN_Set();       						//关闭SPI传输
  	return status;          						//返回读到的状态值
}			

//**********************************************NRF24L01操作函数****************************************************

//启动NRF24L01发送一次数据
//参数：txbuf（待发送数据首地址）
//返回值:发送完成状况
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
	SPI1_SetSpeed(6);								//spi速度为8Mhz（24L01的最大SPI时钟为10Mhz）   
	NRF_CE_Clr();
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);	//写数据到TX BUF  32个字节
	NRF_CE_Set();																				//启动发送	   
	while(NRF_IRQ_In()!=0);																//等待发送完成
	sta=NRF24L01_Read_Reg(STATUS);  											//读取状态寄存器的值	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); 				//清除TX_DS或MAX_RT中断标志
	if(sta&MAX_TX)																				//达到最大重发次数
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);								//清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(sta&TX_OK)																					//发送完成
	{
		return TX_OK;
	}
	return 0xff;																					//其他原因发送失败
}

//启动NRF24L01接收一次数据
//txbuf:待接收数据首地址
//返回值:0（接收完成）其他（错误代码）
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	SPI1_SetSpeed(6); 							//spi速度为8Mhz（24L01的最大SPI时钟为10Mhz）   
	sta=NRF24L01_Read_Reg(STATUS);  											//读取状态寄存器的值    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); 						//清除TX_DS或MAX_RT中断标志
	if(sta&RX_OK)//接收到数据
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);			//读取数据
		NRF24L01_Write_Reg(FLUSH_RX,0xff);								//清除RX FIFO寄存器 
		return 0; 
	}	   
	return 1;															//没收到任何数据
}	

//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了		   
void NRF24L01_RX_Mode(u8 chl)
{
	NRF_CE_Clr();	  
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    					//使能通道0的自动应答    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);					//使能通道0的接收地址  	 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,chl);	    				//设置RF通信频率		  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 	    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);					//设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);						//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
  	NRF_CE_Set(); 																					//CE为高,进入接收模式 
}		

//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了		   
//CE为高大于10us,则启动发送.	 
void NRF24L01_TX_Mode(u8 chl)
{														 
	NRF_CE_Clr();	    
  	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);			//写TX节点地址 
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); 	//设置TX节点地址,主要为了使能ACK	  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     													//使能通道0的自动应答    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); 													//使能通道0的接收地址  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);													//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,chl);       													//设置RF通道为40
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  													//设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    													//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	NRF_CE_Set();																															//CE为高,10us后启动发送
}


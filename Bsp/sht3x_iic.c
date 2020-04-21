#include "sht3x_iic.h"

/**
  * @brief  I2C 时钟配置
  * @param  无
  * @retval 无
  */
static void IIC_CKCU_Config()
{
	CKCU_PeripClockConfig_TypeDef CCLOCK = {{0}};
	
	SHT_CLK(CCLOCK)     = 1; 
	CCLOCK.Bit.AFIO  = 1;
	
	CKCU_PeripClockConfig(CCLOCK, ENABLE);
}

/**
  * @brief  I2C I/O配置
  * @param  无
  * @retval 无
  */ 
static void IIC_GPIO_Config()
{
	AFIO_GPxConfig(SHT_SCL_GPIO_ID, SHT_SCL_PIN, SHT_SCL_AFIO_MODE);
	AFIO_GPxConfig(SHT_SDA_GPIO_ID, SHT_SDA_PIN, SHT_SDA_AFIO_MODE);
	
	/* Configure the GPIO pin                                                                                 */
	GPIO_PullResistorConfig(SHT_SCL_PORT, SHT_SCL_PIN, GPIO_PR_DOWN);
	GPIO_DriveConfig(SHT_SCL_PORT, SHT_SCL_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(SHT_SCL_PORT, SHT_SCL_PIN, GPIO_DIR_OUT);
	
	GPIO_PullResistorConfig(SHT_SDA_PORT, SHT_SDA_PIN, GPIO_PR_DOWN);
	GPIO_DriveConfig(SHT_SDA_PORT, SHT_SDA_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(SHT_SDA_PORT, SHT_SDA_PIN, GPIO_DIR_OUT);
}

/*初始化IIC         */
void SHT_IIC_Init(void)
{					     
	IIC_CKCU_Config();
	IIC_GPIO_Config();
}
/*产生IIC起始信号         */
void SHT_IIC_Start(void)
{

	SHT_SDA_H();	  	  
	SHT_SCL_H();
	delay_us(2);
 	SHT_SDA_L();//开始：当SCL为高时，数据从高到低变化
	delay_us(2);
	SHT_SCL_L();//钳住IIC总线，准备发送或接收数据 
}	  
/*产生IIC停止信号         */
void SHT_IIC_Stop(void)
{
	SHT_SDA_OUT();//sda线输出
	SHT_SCL_L();
	SHT_SDA_L();//停止:当SCL为高时，数据从低到高变化
 	delay_us(2);
	SHT_SCL_H();
	delay_us(2);	 
	SHT_SDA_H();//发送IIC总线结束信号
							   	
}
/*等待应答信号到来
返回值：1，接收应答失败
        0，接收应答成功*/
u8 SHT_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SHT_SCL_L();
	SHT_SDA_H();
	SHT_SDA_IN();      //SDA设置为输入  
	delay_us(1);	   
	SHT_SCL_H();
	delay_us(1);	 
	while(SHT_READ_SDA())//等到SDA变低
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			printf("SHT31 slave no ack!\r\n");
			SHT_IIC_Stop();
			return 1;
		}
		delay_us(1);
	}
	SHT_SCL_L();//时钟输出0
	return 0;  
} 
/*产生ACK应答         */
void SHT_IIC_Ack(void)
{
	SHT_SCL_L();
	SHT_SDA_OUT();
	SHT_SDA_L();
	delay_us(2);
	SHT_SCL_H();
	delay_us(2);
	SHT_SCL_L();
}
/*产生非ACK应答		 */
void SHT_IIC_NAck(void)
{
	SHT_SCL_L();
	SHT_SDA_OUT();
	SHT_SDA_H();
	delay_us(2);
	SHT_SCL_H();
	delay_us(2);
	SHT_SCL_L();
}					 				     
/*IIC发送一个字节
返回从机有无应答
1，有应答
0，无应答			*/  
void SHT_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SHT_SDA_OUT(); 	    
    SHT_SCL_L();//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		SHT_SCL_L();
		if((txd&0x80)>>7)
			SHT_SDA_H();
		else
			SHT_SDA_L();
		txd<<=1; 	  
		delay_us(2);   //延时
		SHT_SCL_H();
		delay_us(2); 
		SHT_SCL_L();
		delay_us(2); 		
    }
		delay_us(2);
} 	    
/*IIC读取一个字节
输入主机发送有无应答
1，有应答
0，无应答
返回读取的字节			*/   
u8 SHT_IIC_Read_Byte(unsigned char ack)
{
	u8 i;
	u8 receive=0;
	SHT_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        SHT_SCL_L(); 
        delay_us(2);
		SHT_SCL_H();
		delay_us(2);
        receive<<=1;
        if(SHT_READ_SDA())receive++; 
				delay_us(2);			
    }
        SHT_SCL_L();						 
    if (!ack)
        SHT_IIC_NAck();//发送nACK
    else
        SHT_IIC_Ack(); //发送ACK  
    return receive;
}




//**********************************操作函数****************************
double SHT_Temp=0;
double SHT_Humi=0;
/* SHT3x初始化		 */
void Sht3x_Init(void)
{
    SHT_IIC_Init();
	SHT_SCL_H();
	SHT_SDA_H();
}

void Sth3x_ReadResult(u8 addr)
{
	u16 tem,hum;
	u8 tempbuf[3];
	u8 humibuf[3];
	SHT_IIC_Start();
	SHT_IIC_Send_Byte(addr<<1 | Sht3x_write);//写7位IIC设备地址加0作为写取位,1为读取位
	SHT_IIC_Wait_Ack();
	SHT_IIC_Send_Byte(0x2C);
	SHT_IIC_Wait_Ack();
	SHT_IIC_Send_Byte(0x06);
	SHT_IIC_Wait_Ack();
	SHT_IIC_Stop();
	delay_ms(50);
	SHT_IIC_Start();
	SHT_IIC_Send_Byte(addr<<1 | Sht3x_read);//写7位IIC设备地址加0作为写取位,1为读取位
	if(SHT_IIC_Wait_Ack()==0)
	{
		tempbuf[0]=SHT_IIC_Read_Byte(1);	
		tempbuf[1]=SHT_IIC_Read_Byte(1);
		tempbuf[2]=SHT_IIC_Read_Byte(1);
		humibuf[0]=SHT_IIC_Read_Byte(1);
		humibuf[1]=SHT_IIC_Read_Byte(1);
		humibuf[2]=SHT_IIC_Read_Byte(0);
		SHT_IIC_Stop();
	}
	if(sht30_crc8_check(tempbuf,2,tempbuf[2])==0)//校验正确
		tem = ((tempbuf[0]<<8) | tempbuf[1]);//温度拼接
	if(sht30_crc8_check(humibuf,2,humibuf[2])==0)
		hum = ((humibuf[0]<<8) | humibuf[1]);//湿度拼接	
	/*转换实际温度*/
	SHT_Temp= (175.0*(double)tem/65535.0-45.0) ;// T = -45 + 175 * tem / (2^16-1)
	SHT_Humi= (100.0*(double)hum/65535.0);// RH = hum*100 / (2^16-1)	
}

/*crc校验和计算			*/
u8 sht30_crc8_check(u8 *check_data,u8 num_of_data,u8 CrcData)
{
	u8 bit;        // bit mask
	u8 crc = 0xFF; // calculated checksum
	u8 byteCtr;    // byte counter

	// calculates 8-Bit checksum with given polynomial
	for(byteCtr = 0; byteCtr < num_of_data; byteCtr++) 
	{
		 crc ^= (check_data[byteCtr]);
		//crc校验,最高位为1就^0x31
		 for(bit = 8; bit > 0; --bit) 
			{
			 if(crc & 0x80) 
			 {
				 crc = (crc << 1) ^ 0x31;
			 }  else {
				 crc = (crc << 1);
			 }
		 }
	}
	if(crc != CrcData) 
	{   
		return 1;           
	}
	return 0;
}

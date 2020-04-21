#include "sgp30_iic.h"


/**
  * @brief  I2C 时钟配置
  * @param  无
  * @retval 无
  */
static void IIC_CKCU_Config()
{
	CKCU_PeripClockConfig_TypeDef CCLOCK = {{0}};
	
	SGP_ClK(CCLOCK)     = 1;
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
	AFIO_GPxConfig(SGP_SCL_GPIO_ID, SGP_SCL_PIN, SGP_SCL_AFIO_MODE);
	AFIO_GPxConfig(SGP_SDA_GPIO_ID, SGP_SDA_PIN, SGP_SDA_AFIO_MODE);
	
	/* Configure the GPIO pin                                                                                 */
	GPIO_PullResistorConfig(SGP_SCL_PORT, SGP_SCL_PIN, GPIO_PR_DOWN);
	GPIO_DriveConfig(SGP_SCL_PORT, SGP_SCL_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(SGP_SCL_PORT, SGP_SCL_PIN, GPIO_DIR_OUT);
	
	GPIO_PullResistorConfig(SGP_SDA_PORT, SGP_SDA_PIN, GPIO_PR_DOWN);
	GPIO_DriveConfig(SGP_SDA_PORT, SGP_SDA_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(SGP_SDA_PORT, SGP_SDA_PIN, GPIO_DIR_OUT);
}

/*初始化IIC         */
void SGP_IIC_Init(void)
{					     
	IIC_CKCU_Config();
	IIC_GPIO_Config();
}

/*产生IIC起始信号         */
void SGP_IIC_Start(void)
{
//	SDA_OUT();     //sda线输出
	SGP_SDA_H();	  	  
	SGP_SCL_H();
	delay_us(2);
 	SGP_SDA_L();//开始：当SCL为高时，数据从高到低变化
	delay_us(2);
	SGP_SCL_L();//钳住IIC总线，准备发送或接收数据 
}	  
/*产生IIC停止信号         */
void SGP_IIC_Stop(void)
{
	SGP_SDA_OUT();//sda线输出
	SGP_SCL_L();
	SGP_SDA_L();//停止:当SCL为高时，数据从低到高变化
 	delay_us(2);
	SGP_SCL_H();
	delay_us(2);	 
	SGP_SDA_H();//发送IIC总线结束信号
							   	
}
/*等待应答信号到来
返回值：1，接收应答失败
        0，接收应答成功*/
u8 SGP_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SGP_SCL_L();
	SGP_SDA_H();
	SGP_SDA_IN();      //SDA设置为输入  
	delay_us(1);	   
	SGP_SCL_H();
	delay_us(1);	 
	while(SGP_READ_SDA())//等到SDA变低
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			printf("SGP30 slave no ack!\r\n");
			SGP_IIC_Stop();
			return 1;
		}
		delay_us(1);
	}
	SGP_SCL_L();//时钟输出0
	return 0;  
} 
/*产生ACK应答         */
void SGP_IIC_Ack(void)
{
	SGP_SCL_L();
	SGP_SDA_OUT();
	SGP_SDA_L();
	delay_us(2);
	SGP_SCL_H();
	delay_us(2);
	SGP_SCL_L();
}
/*产生非ACK应答		 */
void SGP_IIC_NAck(void)
{
	SGP_SCL_L();
	SGP_SDA_OUT();
	SGP_SDA_H();
	delay_us(2);
	SGP_SCL_H();
	delay_us(2);
	SGP_SCL_L();
}					 				     
/*IIC发送一个字节
返回从机有无应答
1，有应答
0，无应答			*/  
void SGP_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SGP_SDA_OUT(); 	    
    SGP_SCL_L();//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		SGP_SCL_L();
		if((txd&0x80)>>7)
			SGP_SDA_H();
		else
			SGP_SDA_L();
		txd<<=1; 	  
		delay_us(2);   //延时
		SGP_SCL_H();
		delay_us(2); 
		SGP_SCL_L();
		delay_us(2); 		
    }
		delay_us(2);
} 	    
/*IIC读取一个字节
输入主机发送有无应答
1，有应答
0，无应答
返回读取的字节			*/   
u8 SGP_IIC_Read_Byte(unsigned char ack)
{
	u8 i;
	u8 receive=0;
	SGP_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        SGP_SCL_L(); 
        delay_us(2);
		SGP_SCL_H();
		delay_us(2);
        receive<<=1;
        if(SGP_READ_SDA())receive++; 
				delay_us(2);			
    }
        SGP_SCL_L();						 
    if (!ack)
        SGP_IIC_NAck();//发送nACK
    else
        SGP_IIC_Ack(); //发送ACK  
    return receive;
}



/**********************************操作函数**********************************/

//初始化SGP30接口
void SGP30_Init(void)
{
	SGP_IIC_Init();
	SGP30_ad_write(0x20,0x03);
//	SGP30_ad_write(0x20,0x61);
//	SGP30_ad_write(0x01,0x00);
}

void SGP30_ad_write(u8 a, u8 b)
{
  	SGP_IIC_Start();
  	SGP_IIC_Send_Byte(SGP30_write); //发送器件地址+写指令
	SGP_IIC_Wait_Ack();
  	SGP_IIC_Send_Byte(a);		//发送控制字节
	SGP_IIC_Wait_Ack();
	SGP_IIC_Send_Byte(b);
	SGP_IIC_Wait_Ack();
	SGP_IIC_Stop();
	delay_ms(100);
}

u32 SGP30_ad_read(void)
{
  	u32 dat;
	u8 crc;
  	SGP_IIC_Start();
  	SGP_IIC_Send_Byte(SGP30_read); //发送器件地址+读指令
	SGP_IIC_Wait_Ack();
  	dat = SGP_IIC_Read_Byte(1);
	dat <<= 8;
	dat += SGP_IIC_Read_Byte(1);
	crc = SGP_IIC_Read_Byte(1); //crc数据，舍去
	crc = crc;  //为了不让出现编译警告
	dat <<= 8;
	dat += SGP_IIC_Read_Byte(1);
	dat <<= 8;
	dat += SGP_IIC_Read_Byte(0);
  	SGP_IIC_Stop();
  	return(dat);
}

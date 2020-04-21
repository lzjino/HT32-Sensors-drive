#include "bh1750_iic.h"

/**
  * @brief  I2C 时钟配置
  * @param  None
  * @retval None
  */
static void IIC_CKCU_Config()
{
	CKCU_PeripClockConfig_TypeDef CCLOCK = {{0}};
	
	BH_SCL_CLK(CCLOCK)     = 1;
	BH_SDA_CLK(CCLOCK)     = 1;
	CCLOCK.Bit.AFIO  = 1;
	
	CKCU_PeripClockConfig(CCLOCK, ENABLE);
}

/**
  * @brief  I2C I/O配置
  * @param  None
  * @retval None
  */ 
static void IIC_GPIO_Config()
{
	AFIO_GPxConfig(BH_SCL_GPIO_ID, BH_SCL_PIN, BH_SCL_AFIO_MODE);
	AFIO_GPxConfig(BH_SDA_GPIO_ID, BH_SDA_PIN, BH_SDA_AFIO_MODE);
//	AFIO_GPxConfig(BH_ADDR_GPIO_ID, BH_ADDR_PIN, BH_ADDR_AFIO_MODE);
	
	/* Configure the GPIO pin                                                                                 */
	GPIO_PullResistorConfig(BH_SCL_PORT, BH_SCL_PIN, GPIO_PR_DOWN);
	GPIO_DriveConfig(BH_SCL_PORT, BH_SCL_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(BH_SCL_PORT, BH_SCL_PIN, GPIO_DIR_OUT);
	
	GPIO_PullResistorConfig(BH_SDA_PORT, BH_SDA_PIN, GPIO_PR_DOWN);
	GPIO_DriveConfig(BH_SDA_PORT, BH_SDA_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(BH_SDA_PORT, BH_SDA_PIN, GPIO_DIR_OUT);
	
//	GPIO_PullResistorConfig(BH_ADDR_PORT, BH_ADDR_PIN, GPIO_PR_DOWN);
//	GPIO_DriveConfig(BH_ADDR_PORT, BH_ADDR_PIN, GPIO_DV_8MA);
//	GPIO_DirectionConfig(BH_ADDR_PORT, BH_ADDR_PIN, GPIO_DIR_OUT);
}

/**
  * @brief  初始化IIC
  * @param  None
  * @retval None
*/
void BH_IIC_Init(void)
{					     
	IIC_CKCU_Config();
	IIC_GPIO_Config();
//	GPIO_WriteOutBits(BH_ADDR_PORT, BH_ADDR_PIN, RESET);	//SDA输出L
}

/**
  * @brief  产生IIC起始信号
  * @param  None
  * @retval None
*/
void BH_IIC_Start(void)
{
//	SDA_OUT();     //sda线输出
	BH_SDA_H();	  	  
	BH_SCL_H();
	delay_us(2);
 	BH_SDA_L();//开始：当SCL为高时，数据从高到低变化
	delay_us(2);
	BH_SCL_L();//钳住IIC总线，准备发送或接收数据 
}	  
/**
  * @brief  产生IIC停止信号
  * @param  None
  * @retval None
*/
void BH_IIC_Stop(void)
{
	BH_SDA_OUT();//sda线输出
	BH_SCL_L();
	BH_SDA_L();//停止:当SCL为高时，数据从低到高变化
 	delay_us(2);
	BH_SCL_H();
	delay_us(2);	 
	BH_SDA_H();//发送IIC总线结束信号
							   	
}

/**
  * @brief  等待应答信号到来
  * @param  1，接收应答失败 0，接收应答成功
  * @retval None
*/
u8 BH_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	BH_SCL_L();
	BH_SDA_H();
	BH_SDA_IN();      //SDA设置为输入  
	delay_us(1);	   
	BH_SCL_H();
	delay_us(1);	 
	while(BH_READ_SDA())//等到SDA变低
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			printf("BH1750 slave no ack!\r\n");
			BH_IIC_Stop();
			return 1;
		}
		delay_us(1);
	}
	BH_SCL_L();//时钟输出0
	return 0;  
} 
/**
  * @brief  产生ACK应答
  * @param  None
  * @retval None
*/
void BH_IIC_Ack(void)
{
	BH_SCL_L();
	BH_SDA_OUT();
	BH_SDA_L();
	delay_us(2);
	BH_SCL_H();
	delay_us(2);
	BH_SCL_L();
}
/**
  * @brief  产生非ACK应答
  * @param  None
  * @retval None
*/
void BH_IIC_NAck(void)
{
	BH_SCL_L();
	BH_SDA_OUT();
	BH_SDA_H();
	delay_us(2);
	BH_SCL_H();
	delay_us(2);
	BH_SCL_L();
}					 				     
/**
  * @brief  IIC发送一个字节
  * @param  txd: 8位字节
  * @retval None
*/ 
void BH_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	BH_SDA_OUT(); 	    
    BH_SCL_L();//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		BH_SCL_L();
		if((txd&0x80)>>7)
			BH_SDA_H();
		else
			BH_SDA_L();
		txd<<=1; 	  
		delay_us(2);   //延时
		BH_SCL_H();
		delay_us(2); 
		BH_SCL_L();
		delay_us(2); 		
    }
		delay_us(2);
}

/**
  * @brief  IIC读取一个字节
  * @param  ack: 1有应答,0非应答
  * @retval receive: 读取的字节
*/   
u8 BH_IIC_Read_Byte(unsigned char ack)
{
	u8 i;
	u8 receive=0;
	BH_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        BH_SCL_L(); 
        delay_us(2);
		BH_SCL_H();
		delay_us(2);
        receive<<=1;
        if(BH_READ_SDA())receive++; 
		delay_us(2);			
    }
        BH_SCL_L();						 
    if (!ack)
        BH_IIC_NAck();//发送nACK
    else
        BH_IIC_Ack(); //发送ACK  
    return receive;
}



/**********************************BH1750操作函数**********************************/

/**
  * @brief  BH1750配置
  * @param  None
  * @retval None
  */ 
void BH1750_Init(void)
{
	BH_IIC_Init();
	BH_SCL_H();
	BH_SDA_H();
	Single_Write_BH1750(0x01);   // power on
}

/**
  * @brief  BH1750写一个字节
  * @param  addr: 寄存器地址
  * @retval None
  */ 
void Single_Write_BH1750(u8 addr)
{
    BH_IIC_Start();                 	//起始信号
    BH_IIC_Send_Byte(SlaveAddress+BH1750_write);  	//发送设备地址+写信号
    BH_IIC_Wait_Ack();
	BH_IIC_Send_Byte(addr);    			//内部寄存器地址，请参考中文pdf22页 
	BH_IIC_Wait_Ack();      	
    BH_IIC_Stop();                  	//发送停止信号
}


/**
  * @brief  连续读出BH1750内部数据
  * @param  None
  * @retval temp: 光强初始16位数据
  */ 
u16 Multiple_read_BH1750(void)
{
	u16 temp = 0;	
    BH_IIC_Start();                          			//起始信号
    BH_IIC_Send_Byte(SlaveAddress+BH1750_read);         //发送设备地址+读信号
	BH_IIC_Wait_Ack();
	                     			 //连续读取2个地址数据，存储中BUF    
    temp =  BH_IIC_Read_Byte(1);
	temp = 	(temp << 8)+ BH_IIC_Read_Byte(0);         			 //BUF[0]存储0x32地址中的数据
   

    BH_IIC_Stop();                          //停止信号
    delay_ms(5);
	return temp;
}
/**
  * @brief  设置BH1750高分辨率模式，读BH1750内部数据
  * @param  None
  * @retval light: 光强转换后的数据
  */ 
float BH1750_Read_Coversion(void)
{
	float light;
	Single_Write_BH1750(0x01);   // power on
    Single_Write_BH1750(0x10);   // H- resolution mode
	delay_ms(120);              //延时120ms
	light = Multiple_read_BH1750()/1.2;
	return light;
}

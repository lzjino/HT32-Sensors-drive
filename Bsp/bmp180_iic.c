#include "bmp180_iic.h"
#include <math.h>
/**
  * @brief  I2C 时钟配置
  * @param  None
  * @retval None
  */
static void IIC_CKCU_Config()
{
	CKCU_PeripClockConfig_TypeDef CCLOCK = {{0}};
	
	BMP_SCL_CLK(CCLOCK)     = 1;
//	BMP_SDA_CLK(CCLOCK)     = 1;
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
	AFIO_GPxConfig(BMP_SCL_GPIO_ID, BMP_SCL_PIN, BMP_SCL_AFIO_MODE);
	AFIO_GPxConfig(BMP_SDA_GPIO_ID, BMP_SDA_PIN, BMP_SDA_AFIO_MODE);
	
	/* Configure the GPIO pin                                                                                 */
	GPIO_PullResistorConfig(BMP_SCL_PORT, BMP_SCL_PIN, GPIO_PR_DOWN);
	GPIO_DriveConfig(BMP_SCL_PORT, BMP_SCL_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(BMP_SCL_PORT, BMP_SCL_PIN, GPIO_DIR_OUT);
	
	GPIO_PullResistorConfig(BMP_SDA_PORT, BMP_SDA_PIN, GPIO_PR_DOWN);
	GPIO_DriveConfig(BMP_SDA_PORT, BMP_SDA_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(BMP_SDA_PORT, BMP_SDA_PIN, GPIO_DIR_OUT);
}

/**
  * @brief  初始化IIC
  * @param  None
  * @retval None
*/
void BMP_IIC_Init(void)
{					     
	IIC_CKCU_Config();
	IIC_GPIO_Config();
}


/**
  * @brief  产生IIC起始信号
  * @param  None
  * @retval None
*/
void BMP_IIC_Start(void)
{
	BMP_SDA_OUT();     //sda线输出
	BMP_SDA_H();	  	  
	BMP_SCL_H();
	delay_us(DelayTime);
 	BMP_SDA_L();//开始：当SCL为高时，数据从高到低变化
	delay_us(DelayTime);
	BMP_SCL_L();//钳住IIC总线，准备发送或接收数据 
}	  

/**
  * @brief  产生IIC停止信号
  * @param  None
  * @retval None
*/
void BMP_IIC_Stop(void)
{
	BMP_SDA_OUT();//sda线输出
	BMP_SCL_L();
	BMP_SDA_L();//停止:当SCL为高时，数据从低到高变化
 	delay_us(DelayTime);
	BMP_SCL_H();
	delay_us(DelayTime);	 
	BMP_SDA_H();//发送IIC总线结束信号
							   	
}

/**
  * @brief  等待应答信号到来
  * @param  1，接收应答失败 0，接收应答成功
  * @retval None
*/
u8 BMP_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	BMP_SCL_L();
	BMP_SDA_H();
	BMP_SDA_IN();      //SDA设置为输入  
	delay_us(1);	   
	BMP_SCL_H();
	delay_us(1);	 
	while(BMP_READ_SDA())//等到SDA变低
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			printf("BMP180 slave no ack!\r\n");
			BMP_IIC_Stop();
			return 1;
		}
		delay_us(1);
	}
	BMP_SCL_L();//时钟输出0
	return 0;  
} 

/**
  * @brief  产生ACK应答
  * @param  None
  * @retval None
*/
void BMP_IIC_Ack(void)
{
	BMP_SCL_L();
	BMP_SDA_OUT();
	BMP_SDA_L();
	delay_us(DelayTime);
	BMP_SCL_H();
	delay_us(DelayTime);
	BMP_SCL_L();
}

/**
  * @brief  产生非ACK应答
  * @param  None
  * @retval None
*/
void BMP_IIC_NAck(void)
{
	BMP_SCL_L();
	BMP_SDA_OUT();
	BMP_SDA_H();
	delay_us(DelayTime);
	BMP_SCL_H();
	delay_us(DelayTime);
	BMP_SCL_L();
}					 				     

/**
  * @brief  IIC发送一个字节
  * @param  txd: 8位字节
  * @retval None
*/
void BMP_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	BMP_SDA_OUT(); 	    
    BMP_SCL_L();//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		BMP_SCL_L();
		if((txd&0x80)>>7)
			BMP_SDA_H();
		else
			BMP_SDA_L();
		txd<<=1; 	  
		delay_us(DelayTime);   //延时
		BMP_SCL_H();
		delay_us(DelayTime); 
		BMP_SCL_L();
		delay_us(DelayTime); 		
    }
} 	    
 
/**
  * @brief  IIC读取一个字节
  * @param  ack: 1有应答,0非应答
  * @retval receive: 读取的字节
*/ 
u8 BMP_IIC_Read_Byte(unsigned char ack)
{
	u8 i;
	u8 receive=0;
	BMP_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        BMP_SCL_L(); 
        delay_us(DelayTime);
		BMP_SCL_H();
		delay_us(DelayTime);
        receive<<=1;
        if(BMP_READ_SDA())receive++; 
		delay_us(DelayTime);			
    }
        BMP_SCL_L();						 
    if (!ack)
        BMP_IIC_NAck();//发送nACK
    else
        BMP_IIC_Ack(); //发送ACK  
    return receive;
}


//存储BMP180数据的结构
_bmp180 bmp180;
/**********************************BMP180操作函数**********************************/

/**
  * @brief  BMP180初始化
  * @param  None
  * @retval None
  */ 
void BMP_Init(void)
{
	BMP_IIC_Init();
	
	BMP_ReadCalibrationData();
}

/**
  * @brief  写一个数据到BMP180
  * @param  WriteAddr：内部寄存器地址
  * @param  DataToWrite：数据
  * @retval None
  */ 
void BMP_WriteOneByte(uint8_t WriteAddr,uint8_t DataToWrite)
{
	BMP_IIC_Start();
	
	BMP_IIC_Send_Byte(0xEE);
	BMP_IIC_Wait_Ack();
	
	BMP_IIC_Send_Byte(WriteAddr);
	BMP_IIC_Wait_Ack();
	
	BMP_IIC_Send_Byte(DataToWrite);
	BMP_IIC_Wait_Ack();
	BMP_IIC_Stop();
}

/**
  * @brief  从BMP180读一个字节数据
  * @param  ReadAddr：地址
  * @retval data：数据
  */ 
uint8_t BMP_ReadOneByte(uint8_t ReadAddr)
{
	uint8_t data = 0;
	
	BMP_IIC_Start();
	
	BMP_IIC_Send_Byte(0xEE);
	BMP_IIC_Wait_Ack();
	
	BMP_IIC_Send_Byte(ReadAddr);
	BMP_IIC_Wait_Ack();
	
	BMP_IIC_Start();
	
	BMP_IIC_Send_Byte(0xEF);
	BMP_IIC_Wait_Ack();
	
	data = BMP_IIC_Read_Byte(0);
	BMP_IIC_Stop();
	
	return data;
}

/**
  * @brief  从BMP180读一个16位的数据
  * @param  ReadAddr  
  * @retval data
  */ 
short BMP_ReadTwoByte(uint8_t ReadAddr)
{
	short data;
	uint8_t msb,lsb;
	
	BMP_IIC_Start();
	
	BMP_IIC_Send_Byte(0xEE);
	BMP_IIC_Wait_Ack();
	
	BMP_IIC_Send_Byte(ReadAddr);
	BMP_IIC_Wait_Ack();

	
	BMP_IIC_Start();
	
	BMP_IIC_Send_Byte(0xEF);
	BMP_IIC_Wait_Ack();
	
	msb = BMP_IIC_Read_Byte(1);
	lsb = BMP_IIC_Read_Byte(0);
	
	BMP_IIC_Stop();
	
	data = msb*256 + lsb;
	
	return data;
}

/**
  * @brief  从BMP180的获取计算参数
  * @param  None  
  * @retval None
  */
void BMP_ReadCalibrationData(void)
{
	bmp180.AC1 = BMP_ReadTwoByte(0xAA);
	bmp180.AC2 = BMP_ReadTwoByte(0xAC);
	bmp180.AC3 = BMP_ReadTwoByte(0xAE);
	bmp180.AC4 = BMP_ReadTwoByte(0xB0);
	bmp180.AC5 = BMP_ReadTwoByte(0xB2);
	bmp180.AC6 = BMP_ReadTwoByte(0xB4);
	bmp180.B1  = BMP_ReadTwoByte(0xB6);
	bmp180.B2  = BMP_ReadTwoByte(0xB8);
	bmp180.MB  = BMP_ReadTwoByte(0xBA);
	bmp180.MC  = BMP_ReadTwoByte(0xBC);
	bmp180.MD  = BMP_ReadTwoByte(0xBE);
}

/**
  * @brief  从BMP180读取未修正的温度
  * @param  None  
  * @retval temp
  */
long BMP_Read_UT(void)
{
	long temp = 0;
	BMP_WriteOneByte(0xF4,0x2E);	//ctrl_meas寄存器地址0xF4，写入0x2e
	
	delay_ms(5);
	temp = BMP_ReadTwoByte(0xF6);	//out_msb：数据寄存器地址，共读16bit
	temp &= 0x0000FFFF;
	return temp;
}

/**
  * @brief  从BMP180读取未修正的大气压
  * @param  None  
  * @retval pressure
  */
long BMP_Read_UP(void)
{
	long pressure = 0;
	
	BMP_WriteOneByte(0xF4,0x34);		//ctrl_meas寄存器地址0xF4，写入0x34
	delay_ms(5);
	
	pressure = BMP_ReadTwoByte(0xF6);	//out_msb：数据寄存器地址，共读16bit
	//pressure = pressure + BMP_ReadOneByte(0xf8);
	pressure &= 0x0000FFFF;
	
	return pressure;
}

/**
  * @brief  用获取的参数对温度和大气压进行修正，并计算海拔
  * @param  None  
  * @retval None
  */
void BMP180_Convert(long *temperature, long *pressure,float *altitide)
{
	//read uncompensated temperature value
	bmp180.UT = BMP_Read_UT();

	//read uncompensated pressure value
	bmp180.UP = BMP_Read_UP();

	//calculate true temperature	
	bmp180.X1 = ((bmp180.UT - bmp180.AC6) * bmp180.AC5) >> 15;
	bmp180.X2 = (((long)bmp180.MC) << 11) / (bmp180.X1 + bmp180.MD);
	bmp180.B5 = bmp180.X1 + bmp180.X2;
	*temperature  = (bmp180.B5 + 8) >> 4;
	
	//calculate true pressure
	bmp180.B6 = bmp180.B5 - 4000;
	bmp180.X1 = ((long)bmp180.B2 * (bmp180.B6 * bmp180.B6 >> 12)) >> 11;
	bmp180.X2 = ((long)bmp180.AC2) * bmp180.B6 >> 11;
	bmp180.X3 = bmp180.X1 + bmp180.X2;
	
	bmp180.B3 = ((((long)bmp180.AC1) * 4 + bmp180.X3) + 2)>> 2;
	bmp180.X1 = ((long)bmp180.AC3) * bmp180.B6 >> 13;
	bmp180.X2 = (((long)bmp180.B1) *(bmp180.B6*bmp180.B6 >> 12)) >>16;
	bmp180.X3 = ((bmp180.X1 + bmp180.X2) + 2) >> 2;
	bmp180.B4 = ((long)bmp180.AC4) * (unsigned long)(bmp180.X3 + 32768) >> 15;
	bmp180.B7 = ((unsigned long)bmp180.UP - bmp180.B3) * 50000;
	
	if(bmp180.B7 < 0x80000000)
	{
		bmp180.p = (bmp180.B7 * 2) / bmp180.B4;		
	}
	else
	{
		bmp180.p = (bmp180.B7 / bmp180.B4) * 2;
	}
	
	bmp180.X1 = (bmp180.p >> 8) * (bmp180.p >>8);
	bmp180.X1 = (((long)bmp180.X1) * 3038) >> 16;
	bmp180.X2 = (-7357 * bmp180.p) >> 16;
		
	*pressure = bmp180.p + ((bmp180.X1 + bmp180.X2 + 3791) >> 4);
	*altitide = 44330*(1 - pow((*pressure)/ 101325.0, 1.0 / 5.255));
 
}



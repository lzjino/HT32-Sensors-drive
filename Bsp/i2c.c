#include "i2c.h"
#include "systick.h"
#include "usart1.h"
 
static void I2C_CKCU_Config()
{
	CKCU_PeripClockConfig_TypeDef CCLOCK;
	
	CCLOCK.Bit.PA     = 1;
	CCLOCK.Bit.AFIO  = 1;
	
	CKCU_PeripClockConfig(CCLOCK, ENABLE);
}
 
static void I2C_GPIO_Config()
{
	AFIO_GPxConfig(I2C_SCL_GPIO_ID, I2C_SCL_PIN, I2C_SCL_AFIO_MODE);
	AFIO_GPxConfig(I2C_SDA_GPIO_ID, I2C_SDA_PIN, I2C_SDA_AFIO_MODE);
	
	/* Configure the GPIO pin                                                                                 */
	GPIO_PullResistorConfig(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_PR_DOWN);
	GPIO_DriveConfig(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_DIR_OUT);
	
	GPIO_PullResistorConfig(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PR_DOWN);
	GPIO_DriveConfig(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_DV_8MA);
	GPIO_DirectionConfig(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_DIR_OUT);
}
 
void I2C_Initia()
{
	I2C_CKCU_Config();
	I2C_GPIO_Config();
	
	/* 一定要先发一遍停止信号 */
	I2C_Stop();
}


void I2C_Start()
{
	I2C_SCL_1();
	I2C_SDA_1();
	Delay_us(DELAYTIME);
	
	I2C_SDA_0();
	Delay_us(DELAYTIME);
	
	I2C_SCL_0();
	Delay_us(DELAYTIME);
}

void I2C_Stop()
{
	I2C_SDA_0();
	I2C_SCL_1();
	Delay_us(DELAYTIME);
	I2C_SDA_1();
}

 
void I2C_Ack()
{
	I2C_SDA_0();	/* CPU驱动SDA = 0 */
	Delay_us(DELAYTIME);
	
	I2C_SCL_1();	/* CPU产生1个时钟 */
	Delay_us(DELAYTIME);
	
	I2C_SCL_0();
	Delay_us(DELAYTIME);
	I2C_SDA_1();	/* CPU释放SDA总线 */
}
 
void I2C_NAck()
{
	I2C_SDA_1();	/* CPU驱动SDA = 1 */
	Delay_us(DELAYTIME);
	
	I2C_SCL_1();	/* CPU产生1个时钟 */
	Delay_us(DELAYTIME);
	
	I2C_SCL_0();
	Delay_us(DELAYTIME);
}

/* 返回0表示正确应答，1表示无器件响应 */
u8 I2C_WaitAck()
{
	u8 ack;
	
	I2C_SCL_1();
	Delay_us(DELAYTIME);
	
	I2C_SDA_1();
	Delay_us(DELAYTIME);
	I2C_SDA_Direction_Input();
	I2C_SDA_InputConfig();
	
	if( I2C_SDA_READ() ) {
		ack = 1;
	}
	else {
		ack = 0;
	}
	I2C_SCL_0();
	Delay_us(DELAYTIME);
	return ack;
}

void I2C_SendByte(u8 _ucByte)
{
	u8 i;
 
	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		
		Delay_us(DELAYTIME);
		I2C_SCL_1();
		Delay_us(DELAYTIME);
		I2C_SCL_0();
		
		if (i == 7)
		{
			 I2C_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		Delay_us(DELAYTIME);
	}
}


u8 I2C_ReadByte()
{
	u8 i;
	u8 data;
	
	data = 0;
	
	for(i = 0;i < 8;i++)
	{
		data <<= 1;
		I2C_SCL_1();
		Delay_us(DELAYTIME);
		
		I2C_SDA_Direction_Input();
		I2C_SDA_InputConfig();
	
		if( I2C_SDA_READ() ) {
			data++;
		}
		I2C_SCL_0();
		Delay_us(DELAYTIME);
	}
	return data;
}


//double Temperature=0;
//double Humidity=0;

void Sth3x_ReadResult(u8 addr)
{
	u16 tem,hum;
	u8 tempbuf[3];
	u8 humibuf[3];
	I2C_Start();
	I2C_SendByte(addr<<1 | 0);//写7位I2C设备地址加0作为写取位,1为读取位
	I2C_WaitAck();
	I2C_SendByte(0x2C);
	I2C_WaitAck();
	I2C_SendByte(0x06);
	I2C_WaitAck();
	I2C_Stop();
	Delay_ms(50);
	I2C_Start();
	I2C_SendByte(addr<<1 | 1);//写7位I2C设备地址加0作为写取位,1为读取位
	if(I2C_WaitAck()==0)
	{
		tempbuf[0]=I2C_ReadByte();	
		tempbuf[1]=I2C_ReadByte();
		tempbuf[2]=I2C_ReadByte();
		humibuf[0]=I2C_ReadByte();
		humibuf[1]=I2C_ReadByte();
		humibuf[2]=I2C_ReadByte();
		I2C_Stop();
	}
	if(CRC_Check(tempbuf,2,tempbuf[2])==0)//校验正确
		tem = ((tempbuf[0]<<8) | tempbuf[1]);//温度拼接
	if(CRC_Check(humibuf,2,humibuf[2])==0)
		hum = ((humibuf[0]<<8) | humibuf[1]);//湿度拼接	
	/*转换实际温度*/
//	Temperature= (175.0*(double)tem/65535.0-45.0) ;// T = -45 + 175 * tem / (2^16-1)
//	Humidity= (100.0*(double)hum/65535.0);// RH = hum*100 / (2^16-1)	
}

//crc校验比较
u8 CRC_Check(u8 *check_data,u8 num_of_data,u8 CrcData)
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
//// 返回值为0表示正确，返回1表示未探测到
//u8 I2C_CheckDevice(u8 _Address)
//{
////	u8 time = 0;
//	u8 ucAck = 0;
//	
//	I2C_Start();
//	I2C_SendByte(_Address | EEPROM_I2C_WR);
//	ucAck =I2C_WaitAck();
//	I2C_SDA_Direction_Output();
//	I2C_Stop();
// 
//	return ucAck;
//}


#include "bmp180_iic.h"
#include <math.h>
/**
  * @brief  I2C ʱ������
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
  * @brief  I2C I/O����
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
  * @brief  ��ʼ��IIC
  * @param  None
  * @retval None
*/
void BMP_IIC_Init(void)
{					     
	IIC_CKCU_Config();
	IIC_GPIO_Config();
}


/**
  * @brief  ����IIC��ʼ�ź�
  * @param  None
  * @retval None
*/
void BMP_IIC_Start(void)
{
	BMP_SDA_OUT();     //sda�����
	BMP_SDA_H();	  	  
	BMP_SCL_H();
	delay_us(DelayTime);
 	BMP_SDA_L();//��ʼ����SCLΪ��ʱ�����ݴӸߵ��ͱ仯
	delay_us(DelayTime);
	BMP_SCL_L();//ǯסIIC���ߣ�׼�����ͻ�������� 
}	  

/**
  * @brief  ����IICֹͣ�ź�
  * @param  None
  * @retval None
*/
void BMP_IIC_Stop(void)
{
	BMP_SDA_OUT();//sda�����
	BMP_SCL_L();
	BMP_SDA_L();//ֹͣ:��SCLΪ��ʱ�����ݴӵ͵��߱仯
 	delay_us(DelayTime);
	BMP_SCL_H();
	delay_us(DelayTime);	 
	BMP_SDA_H();//����IIC���߽����ź�
							   	
}

/**
  * @brief  �ȴ�Ӧ���źŵ���
  * @param  1������Ӧ��ʧ�� 0������Ӧ��ɹ�
  * @retval None
*/
u8 BMP_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	BMP_SCL_L();
	BMP_SDA_H();
	BMP_SDA_IN();      //SDA����Ϊ����  
	delay_us(1);	   
	BMP_SCL_H();
	delay_us(1);	 
	while(BMP_READ_SDA())//�ȵ�SDA���
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
	BMP_SCL_L();//ʱ�����0
	return 0;  
} 

/**
  * @brief  ����ACKӦ��
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
  * @brief  ������ACKӦ��
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
  * @brief  IIC����һ���ֽ�
  * @param  txd: 8λ�ֽ�
  * @retval None
*/
void BMP_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	BMP_SDA_OUT(); 	    
    BMP_SCL_L();//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
		BMP_SCL_L();
		if((txd&0x80)>>7)
			BMP_SDA_H();
		else
			BMP_SDA_L();
		txd<<=1; 	  
		delay_us(DelayTime);   //��ʱ
		BMP_SCL_H();
		delay_us(DelayTime); 
		BMP_SCL_L();
		delay_us(DelayTime); 		
    }
} 	    
 
/**
  * @brief  IIC��ȡһ���ֽ�
  * @param  ack: 1��Ӧ��,0��Ӧ��
  * @retval receive: ��ȡ���ֽ�
*/ 
u8 BMP_IIC_Read_Byte(unsigned char ack)
{
	u8 i;
	u8 receive=0;
	BMP_SDA_IN();//SDA����Ϊ����
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
        BMP_IIC_NAck();//����nACK
    else
        BMP_IIC_Ack(); //����ACK  
    return receive;
}


//�洢BMP180���ݵĽṹ
_bmp180 bmp180;
/**********************************BMP180��������**********************************/

/**
  * @brief  BMP180��ʼ��
  * @param  None
  * @retval None
  */ 
void BMP_Init(void)
{
	BMP_IIC_Init();
	
	BMP_ReadCalibrationData();
}

/**
  * @brief  дһ�����ݵ�BMP180
  * @param  WriteAddr���ڲ��Ĵ�����ַ
  * @param  DataToWrite������
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
  * @brief  ��BMP180��һ���ֽ�����
  * @param  ReadAddr����ַ
  * @retval data������
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
  * @brief  ��BMP180��һ��16λ������
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
  * @brief  ��BMP180�Ļ�ȡ�������
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
  * @brief  ��BMP180��ȡδ�������¶�
  * @param  None  
  * @retval temp
  */
long BMP_Read_UT(void)
{
	long temp = 0;
	BMP_WriteOneByte(0xF4,0x2E);	//ctrl_meas�Ĵ�����ַ0xF4��д��0x2e
	
	delay_ms(5);
	temp = BMP_ReadTwoByte(0xF6);	//out_msb�����ݼĴ�����ַ������16bit
	temp &= 0x0000FFFF;
	return temp;
}

/**
  * @brief  ��BMP180��ȡδ�����Ĵ���ѹ
  * @param  None  
  * @retval pressure
  */
long BMP_Read_UP(void)
{
	long pressure = 0;
	
	BMP_WriteOneByte(0xF4,0x34);		//ctrl_meas�Ĵ�����ַ0xF4��д��0x34
	delay_ms(5);
	
	pressure = BMP_ReadTwoByte(0xF6);	//out_msb�����ݼĴ�����ַ������16bit
	//pressure = pressure + BMP_ReadOneByte(0xf8);
	pressure &= 0x0000FFFF;
	
	return pressure;
}

/**
  * @brief  �û�ȡ�Ĳ������¶Ⱥʹ���ѹ���������������㺣��
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



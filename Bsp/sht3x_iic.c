#include "sht3x_iic.h"

/**
  * @brief  I2C ʱ������
  * @param  ��
  * @retval ��
  */
static void IIC_CKCU_Config()
{
	CKCU_PeripClockConfig_TypeDef CCLOCK = {{0}};
	
	SHT_CLK(CCLOCK)     = 1; 
	CCLOCK.Bit.AFIO  = 1;
	
	CKCU_PeripClockConfig(CCLOCK, ENABLE);
}

/**
  * @brief  I2C I/O����
  * @param  ��
  * @retval ��
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

/*��ʼ��IIC         */
void SHT_IIC_Init(void)
{					     
	IIC_CKCU_Config();
	IIC_GPIO_Config();
}
/*����IIC��ʼ�ź�         */
void SHT_IIC_Start(void)
{

	SHT_SDA_H();	  	  
	SHT_SCL_H();
	delay_us(2);
 	SHT_SDA_L();//��ʼ����SCLΪ��ʱ�����ݴӸߵ��ͱ仯
	delay_us(2);
	SHT_SCL_L();//ǯסIIC���ߣ�׼�����ͻ�������� 
}	  
/*����IICֹͣ�ź�         */
void SHT_IIC_Stop(void)
{
	SHT_SDA_OUT();//sda�����
	SHT_SCL_L();
	SHT_SDA_L();//ֹͣ:��SCLΪ��ʱ�����ݴӵ͵��߱仯
 	delay_us(2);
	SHT_SCL_H();
	delay_us(2);	 
	SHT_SDA_H();//����IIC���߽����ź�
							   	
}
/*�ȴ�Ӧ���źŵ���
����ֵ��1������Ӧ��ʧ��
        0������Ӧ��ɹ�*/
u8 SHT_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SHT_SCL_L();
	SHT_SDA_H();
	SHT_SDA_IN();      //SDA����Ϊ����  
	delay_us(1);	   
	SHT_SCL_H();
	delay_us(1);	 
	while(SHT_READ_SDA())//�ȵ�SDA���
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
	SHT_SCL_L();//ʱ�����0
	return 0;  
} 
/*����ACKӦ��         */
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
/*������ACKӦ��		 */
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
/*IIC����һ���ֽ�
���شӻ�����Ӧ��
1����Ӧ��
0����Ӧ��			*/  
void SHT_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SHT_SDA_OUT(); 	    
    SHT_SCL_L();//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
		SHT_SCL_L();
		if((txd&0x80)>>7)
			SHT_SDA_H();
		else
			SHT_SDA_L();
		txd<<=1; 	  
		delay_us(2);   //��ʱ
		SHT_SCL_H();
		delay_us(2); 
		SHT_SCL_L();
		delay_us(2); 		
    }
		delay_us(2);
} 	    
/*IIC��ȡһ���ֽ�
����������������Ӧ��
1����Ӧ��
0����Ӧ��
���ض�ȡ���ֽ�			*/   
u8 SHT_IIC_Read_Byte(unsigned char ack)
{
	u8 i;
	u8 receive=0;
	SHT_SDA_IN();//SDA����Ϊ����
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
        SHT_IIC_NAck();//����nACK
    else
        SHT_IIC_Ack(); //����ACK  
    return receive;
}




//**********************************��������****************************
double SHT_Temp=0;
double SHT_Humi=0;
/* SHT3x��ʼ��		 */
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
	SHT_IIC_Send_Byte(addr<<1 | Sht3x_write);//д7λIIC�豸��ַ��0��Ϊдȡλ,1Ϊ��ȡλ
	SHT_IIC_Wait_Ack();
	SHT_IIC_Send_Byte(0x2C);
	SHT_IIC_Wait_Ack();
	SHT_IIC_Send_Byte(0x06);
	SHT_IIC_Wait_Ack();
	SHT_IIC_Stop();
	delay_ms(50);
	SHT_IIC_Start();
	SHT_IIC_Send_Byte(addr<<1 | Sht3x_read);//д7λIIC�豸��ַ��0��Ϊдȡλ,1Ϊ��ȡλ
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
	if(sht30_crc8_check(tempbuf,2,tempbuf[2])==0)//У����ȷ
		tem = ((tempbuf[0]<<8) | tempbuf[1]);//�¶�ƴ��
	if(sht30_crc8_check(humibuf,2,humibuf[2])==0)
		hum = ((humibuf[0]<<8) | humibuf[1]);//ʪ��ƴ��	
	/*ת��ʵ���¶�*/
	SHT_Temp= (175.0*(double)tem/65535.0-45.0) ;// T = -45 + 175 * tem / (2^16-1)
	SHT_Humi= (100.0*(double)hum/65535.0);// RH = hum*100 / (2^16-1)	
}

/*crcУ��ͼ���			*/
u8 sht30_crc8_check(u8 *check_data,u8 num_of_data,u8 CrcData)
{
	u8 bit;        // bit mask
	u8 crc = 0xFF; // calculated checksum
	u8 byteCtr;    // byte counter

	// calculates 8-Bit checksum with given polynomial
	for(byteCtr = 0; byteCtr < num_of_data; byteCtr++) 
	{
		 crc ^= (check_data[byteCtr]);
		//crcУ��,���λΪ1��^0x31
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

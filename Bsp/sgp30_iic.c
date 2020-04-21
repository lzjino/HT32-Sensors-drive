#include "sgp30_iic.h"


/**
  * @brief  I2C ʱ������
  * @param  ��
  * @retval ��
  */
static void IIC_CKCU_Config()
{
	CKCU_PeripClockConfig_TypeDef CCLOCK = {{0}};
	
	SGP_ClK(CCLOCK)     = 1;
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

/*��ʼ��IIC         */
void SGP_IIC_Init(void)
{					     
	IIC_CKCU_Config();
	IIC_GPIO_Config();
}

/*����IIC��ʼ�ź�         */
void SGP_IIC_Start(void)
{
//	SDA_OUT();     //sda�����
	SGP_SDA_H();	  	  
	SGP_SCL_H();
	delay_us(2);
 	SGP_SDA_L();//��ʼ����SCLΪ��ʱ�����ݴӸߵ��ͱ仯
	delay_us(2);
	SGP_SCL_L();//ǯסIIC���ߣ�׼�����ͻ�������� 
}	  
/*����IICֹͣ�ź�         */
void SGP_IIC_Stop(void)
{
	SGP_SDA_OUT();//sda�����
	SGP_SCL_L();
	SGP_SDA_L();//ֹͣ:��SCLΪ��ʱ�����ݴӵ͵��߱仯
 	delay_us(2);
	SGP_SCL_H();
	delay_us(2);	 
	SGP_SDA_H();//����IIC���߽����ź�
							   	
}
/*�ȴ�Ӧ���źŵ���
����ֵ��1������Ӧ��ʧ��
        0������Ӧ��ɹ�*/
u8 SGP_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SGP_SCL_L();
	SGP_SDA_H();
	SGP_SDA_IN();      //SDA����Ϊ����  
	delay_us(1);	   
	SGP_SCL_H();
	delay_us(1);	 
	while(SGP_READ_SDA())//�ȵ�SDA���
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
	SGP_SCL_L();//ʱ�����0
	return 0;  
} 
/*����ACKӦ��         */
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
/*������ACKӦ��		 */
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
/*IIC����һ���ֽ�
���شӻ�����Ӧ��
1����Ӧ��
0����Ӧ��			*/  
void SGP_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SGP_SDA_OUT(); 	    
    SGP_SCL_L();//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
		SGP_SCL_L();
		if((txd&0x80)>>7)
			SGP_SDA_H();
		else
			SGP_SDA_L();
		txd<<=1; 	  
		delay_us(2);   //��ʱ
		SGP_SCL_H();
		delay_us(2); 
		SGP_SCL_L();
		delay_us(2); 		
    }
		delay_us(2);
} 	    
/*IIC��ȡһ���ֽ�
����������������Ӧ��
1����Ӧ��
0����Ӧ��
���ض�ȡ���ֽ�			*/   
u8 SGP_IIC_Read_Byte(unsigned char ack)
{
	u8 i;
	u8 receive=0;
	SGP_SDA_IN();//SDA����Ϊ����
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
        SGP_IIC_NAck();//����nACK
    else
        SGP_IIC_Ack(); //����ACK  
    return receive;
}



/**********************************��������**********************************/

//��ʼ��SGP30�ӿ�
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
  	SGP_IIC_Send_Byte(SGP30_write); //����������ַ+дָ��
	SGP_IIC_Wait_Ack();
  	SGP_IIC_Send_Byte(a);		//���Ϳ����ֽ�
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
  	SGP_IIC_Send_Byte(SGP30_read); //����������ַ+��ָ��
	SGP_IIC_Wait_Ack();
  	dat = SGP_IIC_Read_Byte(1);
	dat <<= 8;
	dat += SGP_IIC_Read_Byte(1);
	crc = SGP_IIC_Read_Byte(1); //crc���ݣ���ȥ
	crc = crc;  //Ϊ�˲��ó��ֱ��뾯��
	dat <<= 8;
	dat += SGP_IIC_Read_Byte(1);
	dat <<= 8;
	dat += SGP_IIC_Read_Byte(0);
  	SGP_IIC_Stop();
  	return(dat);
}

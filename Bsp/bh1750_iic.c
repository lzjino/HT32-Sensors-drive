#include "bh1750_iic.h"

/**
  * @brief  I2C ʱ������
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
  * @brief  I2C I/O����
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
  * @brief  ��ʼ��IIC
  * @param  None
  * @retval None
*/
void BH_IIC_Init(void)
{					     
	IIC_CKCU_Config();
	IIC_GPIO_Config();
//	GPIO_WriteOutBits(BH_ADDR_PORT, BH_ADDR_PIN, RESET);	//SDA���L
}

/**
  * @brief  ����IIC��ʼ�ź�
  * @param  None
  * @retval None
*/
void BH_IIC_Start(void)
{
//	SDA_OUT();     //sda�����
	BH_SDA_H();	  	  
	BH_SCL_H();
	delay_us(2);
 	BH_SDA_L();//��ʼ����SCLΪ��ʱ�����ݴӸߵ��ͱ仯
	delay_us(2);
	BH_SCL_L();//ǯסIIC���ߣ�׼�����ͻ�������� 
}	  
/**
  * @brief  ����IICֹͣ�ź�
  * @param  None
  * @retval None
*/
void BH_IIC_Stop(void)
{
	BH_SDA_OUT();//sda�����
	BH_SCL_L();
	BH_SDA_L();//ֹͣ:��SCLΪ��ʱ�����ݴӵ͵��߱仯
 	delay_us(2);
	BH_SCL_H();
	delay_us(2);	 
	BH_SDA_H();//����IIC���߽����ź�
							   	
}

/**
  * @brief  �ȴ�Ӧ���źŵ���
  * @param  1������Ӧ��ʧ�� 0������Ӧ��ɹ�
  * @retval None
*/
u8 BH_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	BH_SCL_L();
	BH_SDA_H();
	BH_SDA_IN();      //SDA����Ϊ����  
	delay_us(1);	   
	BH_SCL_H();
	delay_us(1);	 
	while(BH_READ_SDA())//�ȵ�SDA���
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
	BH_SCL_L();//ʱ�����0
	return 0;  
} 
/**
  * @brief  ����ACKӦ��
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
  * @brief  ������ACKӦ��
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
  * @brief  IIC����һ���ֽ�
  * @param  txd: 8λ�ֽ�
  * @retval None
*/ 
void BH_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	BH_SDA_OUT(); 	    
    BH_SCL_L();//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
		BH_SCL_L();
		if((txd&0x80)>>7)
			BH_SDA_H();
		else
			BH_SDA_L();
		txd<<=1; 	  
		delay_us(2);   //��ʱ
		BH_SCL_H();
		delay_us(2); 
		BH_SCL_L();
		delay_us(2); 		
    }
		delay_us(2);
}

/**
  * @brief  IIC��ȡһ���ֽ�
  * @param  ack: 1��Ӧ��,0��Ӧ��
  * @retval receive: ��ȡ���ֽ�
*/   
u8 BH_IIC_Read_Byte(unsigned char ack)
{
	u8 i;
	u8 receive=0;
	BH_SDA_IN();//SDA����Ϊ����
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
        BH_IIC_NAck();//����nACK
    else
        BH_IIC_Ack(); //����ACK  
    return receive;
}



/**********************************BH1750��������**********************************/

/**
  * @brief  BH1750����
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
  * @brief  BH1750дһ���ֽ�
  * @param  addr: �Ĵ�����ַ
  * @retval None
  */ 
void Single_Write_BH1750(u8 addr)
{
    BH_IIC_Start();                 	//��ʼ�ź�
    BH_IIC_Send_Byte(SlaveAddress+BH1750_write);  	//�����豸��ַ+д�ź�
    BH_IIC_Wait_Ack();
	BH_IIC_Send_Byte(addr);    			//�ڲ��Ĵ�����ַ����ο�����pdf22ҳ 
	BH_IIC_Wait_Ack();      	
    BH_IIC_Stop();                  	//����ֹͣ�ź�
}


/**
  * @brief  ��������BH1750�ڲ�����
  * @param  None
  * @retval temp: ��ǿ��ʼ16λ����
  */ 
u16 Multiple_read_BH1750(void)
{
	u16 temp = 0;	
    BH_IIC_Start();                          			//��ʼ�ź�
    BH_IIC_Send_Byte(SlaveAddress+BH1750_read);         //�����豸��ַ+���ź�
	BH_IIC_Wait_Ack();
	                     			 //������ȡ2����ַ���ݣ��洢��BUF    
    temp =  BH_IIC_Read_Byte(1);
	temp = 	(temp << 8)+ BH_IIC_Read_Byte(0);         			 //BUF[0]�洢0x32��ַ�е�����
   

    BH_IIC_Stop();                          //ֹͣ�ź�
    delay_ms(5);
	return temp;
}
/**
  * @brief  ����BH1750�߷ֱ���ģʽ����BH1750�ڲ�����
  * @param  None
  * @retval light: ��ǿת���������
  */ 
float BH1750_Read_Coversion(void)
{
	float light;
	Single_Write_BH1750(0x01);   // power on
    Single_Write_BH1750(0x10);   // H- resolution mode
	delay_ms(120);              //��ʱ120ms
	light = Multiple_read_BH1750()/1.2;
	return light;
}

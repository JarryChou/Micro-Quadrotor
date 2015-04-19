#include "stm32f10x.h"

/************************************
��������void SPI_Configuration(void)
˵����SPI����
��ڣ���
���ڣ���
��ע������SPI2
************************************/
void SPI_Configuration(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 			// ����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;								// ����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;							// ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;									// ����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;								// ����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;									// NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;			// ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ2��36MHz/2/4=4.5MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;							// ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;									// CRCֵ����Ķ���ʽ����ȫ˫��ģʽ��CRC��Ϊ���һ���ֽڷ��ͳ�ȥ
	SPI_Init(SPI2, &SPI_InitStructure);  										// ����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI2, ENABLE); 														// ʹ��SPI����
	
	GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);  				// PB13/14/15����
}

/***************************************
��������u8 SPI_ReadWriteByte(u8 txData)
˵����SPI��дһ���ֽں���
��ڣ�u8 txData MOSIд�����ֽ�
���ڣ�u8	MISOд����ֽ�
��ע������ֵΪ0ʱ������û�з���/���ճɹ�
****************************************/
u8 SPI_ReadWriteByte(u8 txData)
{		
	u8 retry = 0;
	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) // ���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if (retry > 200)	// ��ʱ
			return 0;
	}			  
	SPI_I2S_SendData(SPI2, txData); 								// ͨ������SPIx����һ������
	retry = 0;
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) // ���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry > 200)		// ��ʱ
			return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI2); 								// ����ͨ��SPIx������յ�����					    
}

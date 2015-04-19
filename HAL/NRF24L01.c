#include "stm32f10x.h"
#include "NRF24L01.H"
#include "SPI.H"

//-----NRF24L01���ͽ��յ�ַ-----//
const u8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01}; // ���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01}; // ���յ�ַ

//-----NRF24L01��������-----//
u8 g_NRFRevData[RX_PLOAD_WIDTH] = {0,0};
//-----NRF24L01��������-----//
u8 g_NRFSendData[TX_PLOAD_WIDTH] = {0};

//-----NRF24L01���ռ�����-----//
__IO u8 g_NRFRevCnt = 0;

/********************************
��������void NRF24L01_Init(void)
˵������ʼ��24L01
��ڣ���
���ڣ���
��ע����
*********************************/
void NRF24L01_Init(void)
{
	NRF24L01_CE_L; 		// ʹ��24L01
	NRF24L01_CSN_H;		// SPIƬѡȡ��  
}

/******************************
��������u8 NRF24L01_Check(void)
˵�������24L01�Ƿ����
��ڣ���
���ڣ�u8
				0���ɹ�;
				1��ʧ��	
��ע����
*******************************/
u8 NRF24L01_Check(void)
{
	u8 buf[5] = {0XA5,0XA5,0XA5,0XA5,0XA5}, i;

	NRF24L01_Write_Buf(WRITE_REG_NRF + TX_ADDR,buf, 5);		// д��5���ֽڵĵ�ַ��TX_ADDRΪ��ַ��WRITE_REG_NRFΪ����
	NRF24L01_Read_Buf(TX_ADDR,buf,5); 						// ����д��ĵ�ַ  
	for (i = 0; i < 5; i++)
		if (buf[i] != 0XA5)
			break;
	if (i != 5)
		return 1;		// ���24L01����	
	return 0;		 	// ��⵽24L01
}

/************************************************
��������void NRF24L01_Write_Reg(u8 reg, u8 value)
˵����SPIд�Ĵ���
��ڣ�	u8 reg		�Ĵ�����ַ
		u8 value	��Ҫд���ֵ
���ڣ���
��ע����
************************************************/
void NRF24L01_Write_Reg(u8 reg, u8 value)
{
	NRF24L01_CSN_L;                 // ʹ��SPI����
	SPI_ReadWriteByte(reg);			// ���ͼĴ����ţ�������Զ�ȡ���Ĵ�����״̬
	SPI_ReadWriteByte(value);      	// д��Ĵ�����ֵ
	NRF24L01_CSN_H;                 // ��ֹSPI����	   
}

/************************************
��������u8 NRF24L01_Read_Reg(u8 reg)
˵������ȡSPI�Ĵ���ֵ
��ڣ�u8 reg	�Ĵ�����ַ
���ڣ�u8		״̬
��ע����
*************************************/
//
//reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;
	
 	NRF24L01_CSN_L;          				// ʹ��SPI����		
	SPI_ReadWriteByte(reg);   				// ���ͼĴ�����
	reg_val = SPI_ReadWriteByte(0XFF);		// ��ȡ�Ĵ������ݣ�ֻ��Ҫ��ȡ�������������⴫�����ݹ�ȥ
	NRF24L01_CSN_H;          				// ��ֹSPI����		    
	return(reg_val);           				// ����״ֵ̬
}

/*******************************************************
��������void NRF24L01_Read_Buf(u8 reg, u8 *pBuf, u8 len)
˵������ָ��λ�ö���ָ�����ȵ�����
��ڣ�	u8 reg		�Ĵ���(λ��)
		u8 *pBuf	����ָ��
		u8 len		���ݳ���
���ڣ���
��ע����
*******************************************************/
void NRF24L01_Read_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 i;
	
	NRF24L01_CSN_L;           					// ʹ��SPI����
	SPI_ReadWriteByte(reg);						// ���ͼĴ���ֵ(λ��)  	   
	for (i = 0; i < len; i++)
		pBuf[i] = SPI_ReadWriteByte(0XFF);		// ��������
	NRF24L01_CSN_H;       						// �ر�SPI����
}

/********************************************************
��������void NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
˵������ָ��λ��дָ�����ȵ�����
��ڣ�	u8 reg		�Ĵ���(λ��)
		u8 *pBuf	����ָ��
		u8 len		���ݳ���
���ڣ���
��ע����
********************************************************/
void NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 i;
	
 	NRF24L01_CSN_L;          			// ʹ��SPI����
	SPI_ReadWriteByte(reg);				// ���ͼĴ���ֵ(λ��)
	for	(i = 0; i < len; i++)
		SPI_ReadWriteByte(*pBuf++); 	// д������	 
	NRF24L01_CSN_H;       				// �ر�SPI����
}

/***************************************
��������u8 NRF24L01_TxPacket(u8 *txbuf)
˵��������NRF24L01����һ������
��ڣ�u8 *txbuf	�����������׵�ַ
���ڣ�U8		�������״��
��ע����
****************************************/
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
	
	NRF24L01_CE_L;											// Ƭѡ
  NRF24L01_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);	// д���ݵ�TX BUF  32���ֽ�
 	NRF24L01_CE_H;
	while (NRF24L01_IRQ != 0);								// �ȴ��������
	sta = NRF24L01_Read_Reg(STATUS);  						// ��ȡ״̬�Ĵ�����ֵ
	NRF24L01_Write_Reg(WRITE_REG_NRF + STATUS, sta); 		// ���TX_DS��MAX_RT�жϱ�־
	if (sta & MAX_TX)										// �ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX, 0xFF);					// ���TX FIFO�Ĵ��� 
		return MAX_TX;
	}
	if (sta & TX_OK)										// �������
	{
		return TX_OK;
	}
	return 0xFF;											// ����ԭ����ʧ��
}

/***************************************
��������u8 NRF24L01_RxPacket(u8 *rxbuf)
˵��������NRF24L01����һ������
��ڣ�u8 *txbuf	�����������׵�ַ
���ڣ�u8	0��	�������
			1��	���ղ��ɹ�
��ע����
****************************************/
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							    
	sta = NRF24L01_Read_Reg(STATUS);  								// ��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(WRITE_REG_NRF + STATUS,sta); 				// ���TX_DS��MAX_RT�жϱ�־
	if (sta & RX_OK)												// ���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD, rxbuf, RX_PLOAD_WIDTH);		// ��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX, 0xFF);							// ���RX FIFO�Ĵ��� 
		return 0; 
	}
	return 1;														// û�յ��κ�����
}

/*****************************************************
��������void NRF24L01_RX_Mode(void)
˵������ʼ��NRF24L01��RXģʽ
��ڣ���
���ڣ���
��ע��
����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
��CE��ߺ�,������RXģʽ,�����Խ���������
******************************************************/
void NRF24L01_RX_Mode(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;											// ����EXTI�ṹ��
	// ���ڷ���ģʽ���Ѿ����ж����жϣ����Ե��л��ɽ���ģʽʱ����Ҫ������������
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;										// EXTI���жϿ�ͨ
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;								// ����Ϊ�жϻ����¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;							// �½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;										// ʹ��EXTI���ж�
	EXTI_Init(&EXTI_InitStructure);
	
	NRF24L01_CE_L;
	NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);			// дTX�ڵ��ַ 
	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);		// дRX�ڵ��ַ
	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_AA,0x01);    								// ʹ��ͨ��0���Զ�Ӧ��    
	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_RXADDR,0x01);								// ʹ��ͨ��0�Ľ��յ�ַ  	 
	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_CH,40);	    								// ����RFͨ��Ƶ��		  
	NRF24L01_Write_Reg(WRITE_REG_NRF+RX_PW_P0,RX_PLOAD_WIDTH);						// ѡ��ͨ��0����Ч���ݿ�� 	    
	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_SETUP,0x0f);								// ����TX�������,0db����,2Mbps,���������濪��   
	NRF24L01_Write_Reg(WRITE_REG_NRF+NRF24L01_CONFIG, 0x0f);						// ���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
	NRF24L01_CE_H; 																	// CEΪ��,�������ģʽ 
}

/*****************************************************************************************
��������void NRF24L01_TX_Mode(void)
˵������ʼ��NRF24L01��TXģʽ
��ڣ���
���ڣ���
��ע��
����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
PWR_UP,CRCʹ��
CEΪ�ߴ���10us,����������
******************************************************************************************/
void NRF24L01_TX_Mode(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;											// ����EXTI�ṹ��
	// ���ڽ���ģʽ�µ�IRQ��������Ϊ�ж��������ţ����Ե��л��ɷ���ģʽʱ����Ҫ���ж��������ŵĹ���ȡ��
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;										// EXTI���жϿ�ͨ
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;								// ����Ϊ�жϻ����¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;							// �½��ش���
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;										// ʧ��EXTI���ж�
	EXTI_Init(&EXTI_InitStructure);
	
	NRF24L01_CE_L;	    
	NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);			// дTX�ڵ��ַ 
	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); 		// ����TX�ڵ��ַ,��ҪΪ��ʹ��ACK
	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_AA,0x01);     								// ʹ��ͨ��0�Զ�Ӧ��    
	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_RXADDR,0x01); 								// ʹ��ͨ��0�Ľ��յ�ַ  
	NRF24L01_Write_Reg(WRITE_REG_NRF+SETUP_RETR,0x1a);								// �����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:15��
	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_CH,40);       								// ����RFͨ��Ϊ40
	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_SETUP,0x0f);  								// ����TX�������,0db����,2Mbps,���������濪��   
	NRF24L01_Write_Reg(WRITE_REG_NRF+NRF24L01_CONFIG,0x0e);    						// ���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	NRF24L01_CE_H;																	// CEΪ��,10us����������
}		  

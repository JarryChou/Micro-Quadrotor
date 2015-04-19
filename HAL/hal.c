/**********************************************************
HAL.c
��Ҫ����оƬӲ�����ڲ���Χ���ⲿ��Χ�ĳ�ʼ��������INIT����
��MAIN�е��ã�ʹMAIN�����о�����Ӳ�����޹�
***********************************************************/
#include "stm32f10x.h"
#include "HAL.H"

/*****************************
��������void ChipHalInit(void)
˵����Ƭ��Ӳ����ʼ��
��ڣ���
���ڣ���
��ע����
*****************************/
void ChipHal_Init(void)
{
	RCC_Configuration();		// ��ʼ��RCC
	NVIC_Configuration();		// ��ʼ��NVIC
	GPIO_Configuration();		// ��ʼ��GPIO
	TIMx_Configuration();		// ��ʼ��TIM
	USART_Configuration();		// ��ʼ��USART
	SPI_Configuration();		// ��ʼ��SPI
	Delay_Init();				// �ӳٺ�����ʼ��
	IIC_Init();					// ��ʼ��IIC

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	// ʧ��JTAG��ʹ��SW-DP
}

/********************************
��������void ChipOutHalInit(void)
˵����Ƭ��Ӳ����ʼ��
��ڣ���
���ڣ���
��ע����
********************************/
void ChipOutHal_Init(void)
{
	MPU6050_Init();				// MPU6050��ʼ��
	NRF24L01_Init();			// NRF24L01��ʼ��
	while(NRF24L01_Check())		// ���NRF24L01�Ƿ����,��������ڣ�4����ɫLEDͬʱ��˸
	{
		LED1 = LED2 = LED3 = LED4 = 0;
		Delay_ms(500);
		LED1 = LED2 = LED3 = LED4 = 1;
		Delay_ms(500);
	}
}

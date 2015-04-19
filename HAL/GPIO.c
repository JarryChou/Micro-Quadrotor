#include "stm32f10x.h"

/*************************************
��������void GPIO_Configuration(void)
˵����GPIO����
��ڣ���
���ڣ���
��ע����
*************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;		// ����GPIO�ṹ��
	EXTI_InitTypeDef EXTI_InitStructure;		// ����EXTI�ṹ��

	//------PWM_LED------//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;	// PB6,7,8,9-->PWM_LED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;									// �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;									// 50MHz��תƵ��
	GPIO_Init(GPIOB, &GPIO_InitStructure);												// ��ʼ��IO
	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
	
	//------LED------//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;	// PA0,1,2,3,4-->LED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;									// �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;									// 50MHz��תƵ��
	GPIO_Init(GPIOA, &GPIO_InitStructure);												// ��ʼ��IO
	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
	
	//------IIC------//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;							// PA11,12-->IIC
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;									// �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;									// 50MHz��תƵ��
	GPIO_Init(GPIOA, &GPIO_InitStructure);												// ��ʼ��IO
	
	//------PWM------//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;										// �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;									// 50MHz��תƵ��
	GPIO_Init(GPIOA, &GPIO_InitStructure);												// ��ʼ��IO
	GPIO_ResetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_7);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;										// �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;									// 50MHz��תƵ��
	GPIO_Init(GPIOB, &GPIO_InitStructure);												// ��ʼ��IO
	GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);
	
	//-----NRF24L01-----//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;							// PB10,11-->NRF_CE,NRF_CSN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;									// �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;									// 50MHz��תƵ��
	GPIO_Init(GPIOB, &GPIO_InitStructure);												// ��ʼ��IO
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;   										// PB12-->NRF_IRQ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;									// 50MHz��תƵ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 										// ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//-----SPI2-----//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;				// PB13-->SCK,PB14-->MISO,PB15-->MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  									// PB13/14/15�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//-----USART1-----//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;											// PA9����Ϊ����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;					
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;										// ����Ϊ�������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;											// PA10����Ϊ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;								// ��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//-----�ж��¼�������-----//
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);						// ���¼��ߺ�Pin��������	
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;											// EXTI���жϿ�ͨ
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;									// ����Ϊ�жϻ����¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;								// �½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;											// ʹ��EXTI���ж�
	EXTI_Init(&EXTI_InitStructure);
}

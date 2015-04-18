#include "stm32f10x.h"

/***********************************
��������void NVIC_Configuration(void)
˵����NVIC��ʼ��
��ڣ���
���ڣ���
��ע����
************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;												//����NVIC��ʼ���ṹ��
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);							//���ȼ����1������μ�misc.h line80
	
	//-----NRF24L01�����ж�-----//
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;				//IRQ�ж�ͨ��-->NRF24L01,PB12
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//����ʽ���ȼ���
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//�����ȼ���
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//ʹ��ͨ��
  NVIC_Init(&NVIC_InitStructure);															//��ʼ��NVIC
	
	//-----TIM2��ʱ�ж�-----//
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;							//ѡ��TIM2�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//����ʽ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

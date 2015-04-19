#include "stm32f10x.h"

//-----����ʱ�ӽṹ��-----//
RCC_ClocksTypeDef RCC_ClockFreq;

/*************************************
��������void RCC_Configuration(void)
˵����RCCʱ������
��ڣ���
���ڣ���
��ע����
*************************************/
void RCC_Configuration(void)
{
//	SystemInit();//Դ��system_stm32f10x.c�ļ�,�����Ҫ����PLL�����ã���������Զ���ʱ�ӳ�ʼ�������ٳ�ʼ��һ��
	//��Ϊ��V3.5.0�ٷ����е����������Ѿ���SystemInit()Ƕ���ȥ��
//	RCC_PCLK1Config(RCC_HCLK_Div4);		// PCLK1��APB1��AHB����4��Ƶ
	/**************************************************
	��ȡRCC����Ϣ,������
	��ο�RCC_ClocksTypeDef�ṹ�������,��ʱ��������ɺ�,
	���������ֵ��ֱ�ӷ�ӳ�������������ֵ�����Ƶ��
	***************************************************/
	RCC_GetClocksFreq(&RCC_ClockFreq);
	
	/* ������ÿ�ʹ�ⲿ����ͣ���ʱ��,����һ��NMI�ж�,����Ҫ�õĿ����ε�*/
	//RCC_ClockSecuritySystemCmd(ENABLE);
	
	//-----GPIOʱ��-----//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
													RCC_APB2Periph_GPIOB 
													//RCC_APB2Periph_GPIOC |
													//RCC_APB2Periph_GPIOD | 
													//RCC_APB2Periph_GPIOE |
													//RCC_APB2Periph_GPIOF | 
													/*RCC_APB2Periph_GPIOG*/, ENABLE);		// ʹ��GPIOA,B,C,D,E,F,Gʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		// ʹ�ܸ��ù���ʱ��,�����ⲿ�ж�ʱ�����뿪������ʱ��
	//-----SPI2ʱ��ʹ��-----//
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	//-----TIM3ʱ��-----//
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	//-----TIM2ʱ��-----//
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//-----USART1ʱ��-----//
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//-----DMA1ʱ��-----//
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);			// ע��ҽ���AHB������
}

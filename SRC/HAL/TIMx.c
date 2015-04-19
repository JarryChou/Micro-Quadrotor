#include "stm32f10x.h"
#include "PWM.H"

/*************************************
��������void TIMx_Configuration(void)
˵����TIMx����
��ڣ���
���ڣ���
��ע����
*************************************/
void TIMx_Configuration(void)
{
	u16 prescalerValue = 0, ccr1_PWMVal = 0;
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;								//TIM��ʱ����ʼ���ṹ��	
	TIM_OCInitTypeDef  TIM_OCInitStructure;										//TIM��ʱ������ȽϽṹ��	
	prescalerValue = (u16) (SystemCoreClock / 2000000) - 1;						//Ԥ��Ƶֵ��ʱ�ӻ�׼Ϊ72MHz������ʽΪʱ�ӻ�׼/(prescalerValue + 1)

	//-----TIM2��ʱ����-----//
	TIM_TimeBaseStructure.TIM_Period = 2000;									// 2000/2M=1ms����0��ʼ����,���ֵ��д�뵽Auto-Reload Register��
	TIM_TimeBaseStructure.TIM_Prescaler = 0;									// ��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;								// ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;					// ���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;							// ר����TIM1��TIM8���ظ��Ƚϴ��������¼����ҵ�������ӳ��˶�ʱʱ��
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_PrescalerConfig(TIM2, prescalerValue, TIM_PSCReloadMode_Immediate);		// Ԥ��Ƶ,���ڼ�ʱ��Ƶ��Ϊ2MHz
	
	prescalerValue = (u16) (SystemCoreClock / 20000000) - 1;
	//-----TIM3��ʱ����-----//
	TIM_TimeBaseStructure.TIM_Period = MAX_PWM;									// 2000/20M=0.1ms-->10KHz����0��ʼ����,���ֵ��д�뵽Auto-Reload Register��
	TIM_TimeBaseStructure.TIM_Prescaler = 0;									// ��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;								// ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;					// ���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;							// ר����TIM1��TIM8���ظ��Ƚϴ��������¼����ҵ�������ӳ��˶�ʱʱ��
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_PrescalerConfig(TIM3, prescalerValue, TIM_PSCReloadMode_Immediate);		// Ԥ��Ƶ,���ڼ�ʱ��Ƶ��Ϊ20MHz
	
	//-----PWM����-----//
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 							// ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1-->���ϼ���Ϊ��Ч��ƽ
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 				// �Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = ccr1_PWMVal;								// duty cycle
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 					// �������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  									// ��ʼ������TIM3 OC1-->Motor1
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  									// ��ʼ������TIM3 OC2-->Motor2
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  									// ��ʼ������TIM3 OC3-->Motor3
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  									// ��ʼ������TIM3 OC4-->Motor4

	//-----��������-----//
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_ARRPreloadConfig(TIM3, ENABLE);											// �Զ����ؼĴ���ʹ�ܣ���һ�������¼��Զ�����Ӱ�ӼĴ���
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  							// ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���,�ڸ����¼�ʱ��ֵ�ű�д�뵽CCR
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);									// �򿪸����¼��ж�
                                                                                   
	//-----ʹ��-----//
	TIM_Cmd(TIM2, ENABLE);														// ʹ��TIM2
	TIM_Cmd(TIM3, ENABLE);														// ʹ��TIM3
}

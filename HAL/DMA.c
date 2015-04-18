#include "stm32f10x.h"

//-----DMA���仺������С-----//
static u16 g_DMA1_mem_len = 0;

/***********************************************************************************************
��������void DMA_Configuration(DMA_Channel_TypeDef* DMA_CHx, u32 pbase, u32 mbase, u16 bufsize)
˵����DMA����
��ڣ�DMA_Channel_TypeDef* DMA_CHx	DMAͨ��
			u32 pbase											��Χ������ַ
			u32 mbase											�ڴ��ַ
			u16 bufsize										��������С
���ڣ���
��ע����
************************************************************************************************/
void DMA_Configuration(DMA_Channel_TypeDef* DMA_CHx, u32 pbase, u32 mbase, u16 bufsize)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	DMA_DeInit(DMA_CHx);   																									//��DMA��ͨ��x�Ĵ�������Ϊȱʡֵ
	g_DMA1_mem_len = bufsize;
	DMA_InitStructure.DMA_PeripheralBaseAddr = pbase;  											//DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = mbase;  													//DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  										//���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = bufsize;  														//DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  			//�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 								//�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //��Χ�������ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 				//�ڴ����ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  													//��������������ģʽ����ѭ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 									//DMAͨ��xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  													//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure);  																//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
}

/********************************************************
��������void DMA_StartTrans(DMA_Channel_TypeDef*DMA_CHx)
˵����DMA��ʼһ�δ���
��ڣ�DMA_Channel_TypeDef* DMA_CHx	DMAͨ��
���ڣ���
��ע����
*********************************************************/
void DMA_StartTrans(DMA_Channel_TypeDef*DMA_CHx)
{
	DMA_Cmd(DMA_CHx, DISABLE);  																//�ر�USART1 TX DMA1 ��ָʾ��ͨ��
 	DMA_SetCurrDataCounter(DMA1_Channel4, g_DMA1_mem_len);			//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA_CHx, ENABLE);  																	//ʹ��USART1 TX DMA1 ��ָʾ��ͨ��
}

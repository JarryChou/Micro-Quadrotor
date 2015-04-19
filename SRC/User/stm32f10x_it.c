/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "HAL.H"

//-----�ⲿ��������-----//
extern __IO u8 g_ConCycT_flag;
extern __IO u8 g_Fly_Enable;
extern __IO s16 g_BasePWM;

//-----��ʱ��2������-----//
u16 g_tim2counter = 0;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/***********************************
��������void TIM2_IRQHandler(void)
˵����TIM2�жϴ�����
��ڣ���
���ڣ���
��ע������ϵͳ��ʱ��������������
***********************************/
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)	// �����ж�,1ms����һ��
	{
		g_tim2counter++;
		if (g_tim2counter % 500 == 0)	// 500ms,���̵���˸������ϵͳ��������
			PWM3_LED = !PBin(8);
		if (g_tim2counter % 3 == 0)		// 3ms�жϿ�������
			g_ConCycT_flag = 1;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		// ����жϱ�־λ
	}
}

/***************************************
��������void EXTI15_10_IRQHandler(void)
˵����EXTI15_10_IRQ����жϴ�����
��ڣ���
���ڣ���
��ע�����ڽ���NRF24L01����
***************************************/
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line12) != RESET)
  {
		if (NRF24L01_RxPacket(g_NRFRevData) == 0)	// ���ճɹ�
		{
			g_NRFRevCnt++;	// ������++,�������ڼ���,��˵��ң�����ڷ������ݽ���
			if (g_NRFRevData[0] == 239)	// �������
				g_Fly_Enable = 1;
			if (g_NRFRevData[0] == 223)	// ���ʧ��
				g_Fly_Enable = 0;
			if (g_NRFRevData[0] == 127)	// ������
			{
				g_BasePWM += 20;
				if (g_BasePWM >= BASEPWM_MAX)		// ���������������
					g_BasePWM = BASEPWM_MAX;
			}
			if (g_NRFRevData[0] == 191)	// ������
			{
				g_BasePWM -= 20;
				if (g_BasePWM <= 0)
					g_BasePWM = 0;
			}
			if (g_NRFRevData[0] == 253)	// ���
			{
				g_Exp_Roll = -8;					// -8��
				g_Exp_Pitch = 0;
			}
			if (g_NRFRevData[0] == 254)	// �ҹ�
			{
				g_Exp_Roll = 8;						// +8��
				g_Exp_Pitch = 0;
			}
			if (g_NRFRevData[0] == 247)	// ǰ��
			{
				g_Exp_Pitch = -8;					// -8��
				g_Exp_Roll = 0;
			}
			if (g_NRFRevData[0] == 251)	// ����
			{
				g_Exp_Pitch = 8;					// +8��
				g_Exp_Roll = 0;
			}
				
		}
    EXTI_ClearITPendingBit(EXTI_Line12);			// �����־λ
  }	
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

#include "stm32f10x.h"
#include "HAL.H"

//#define BOOTLOADER	// ��Ҫ�޸Ĺ����е�ROM��SIZE

/*******************
	Designed by Nemo
*******************/

///////////////////////////////ϵͳ���ֽӿ�˵��//////////////////////////////
/*****************************************************************************
//---------------------------������������温�ͼ---------------------------//
-----------------------------------------------------------------------------
|					(Motor1)				 (Motor4)						|
|					x								x						|
|						x						x							|
|							x				x								|
|								x		x									|
|									x										|
|								x		x									|
|							x				x								|
|						x						x							|
|					x								x						|
|					(Motor2)				 (Motor3)						|
-----------------------------------------------------------------------------

					Motor1 && Motor3	-->		˳ʱ��
					Motor2 && Motor4	-->		��ʱ��
					

LED1~4:		������ʾPWM�źŵ�4����ɫLEDָʾ��
				LED1-->PA0-->Motor1
				LED2-->PA1-->Motor2
				LED3-->PA2-->Motor3
				LED4-->PA3-->Motor4
PWM1~4_LED:	����ָʾϵͳ״̬�����ֲ�ͬ��ɫ��ָʾ��
				PWM1_LED-->PB6-->��ɫ
				PWM2_LED-->PB7-->��ɫ
				PWM3_LED-->PB8-->����
				PWM4_LED-->PB9-->��ɫ
PWM1~4:		��������4��Motor��PWM�ź�
				PWM1-->PA6-->Motor1
				PWM2-->PA7-->Motor2
				PWM3-->PB0-->Motor3
				PWM4-->PB1-->Motor4
MPU_INT:	MPU6050�ж������
				MOU_INT-->PA4
USART:		��չ����
				USART1_TX-->PA9
				USART1_RX-->PA10
IIC:		IIC�ӿ�
				SCL-->PA11
				SDA-->PA12

NRF24L01���յĹ��ܼ�ӳ��:
"Up"			-->		247
"Down"			-->		251
"Left"			-->		253
"Right"			-->		254
"Start"			-->		239
"Select"		-->		223
"B"				-->		127
"A"				-->		191

*******************************************************************************
***		����ʱ��5����ʱ�����bootloader���£�ʱ�䵽���Զ��������ʼ����	***
***		������յ����򣬻Ƶ���˸���Ρ�								   		***															
***		���û�г�����£��ƵƲ���˸��										***
*******************************************************************************
//------------------------------�����ź�ָʾ--------------------------------//
LED:
		1��	ͬʱ500ms��˸-->δ��⵽NRF24L01
		2��	����-->��Ӧ��Motor��PWM�ź����
PWM_LED:
		1��	PWM3_LED����ɫÿ500ms��˸һ�α���ϵͳ��������
		2��	PWM4_LED��ɫ������˵��bootloader�Ѿ����ص����򲢸���
			PWM4_LED�Ƶ�����˵���ɻ���Pitch��Roll��̬�����5�㷶Χ��
			Ϩ��˵���ɻ��ĽǶȱȽϴ󣬻����ƽ��
		3��	PWM1_LED��ɫ�������˵������ִ��һ�ε�ʱ���Ѿ������˿�������
		4��	PWM2_LED��ɫ���������������Rool����Pitch��̬�Ѿ�����ƫ��ƽ��״̬
�ǳ�Σ�ա�

//----------------------------�����ź����ݸ�ʽ------------------------------//
0~11�����ٶȼ�*6��������*6
12~15��Pitch*10
16~19��Roll*10
20~23��Yaw*10

*****************************************************************************/
//////////////////////////////////////////////////////////////////////////////

//-----�ⲿ��������-----//
extern u16 g_tim2counter;	// ��ʱ��2�������������ڽ������ʱ��ǿ����0

//-----ϵͳ�������ڿ��ر���-----//
__IO u8 g_ConCycT_flag = 0;	// ÿ���������ڿ�ʼʱ��1,volatile���ͱ�֤�жϲ�����

/*********************************************************
��������static void USARTData_Prepare(u8* pt2g_SendBuffer)
˵����USART���ݷ���׼����׼���ú����һ�δ���DMA
��ڣ�u8* pt2g_SendBuffer	��Ҫ���͵Ĵ��ڻ�����
���ڣ���
��ע����
*********************************************************/
static void USARTData_Prepare(u8* pt2g_SendBuffer)
{
	u8 i, j;
	s16 temp;
	struct MPU6050Filter_tag *ptF = &g_MPU6050Data_Filter;	// ָ����ٶȼ��˲�֮��Ľṹ��ָ��

	for (i = 0; i < 6; i++)		//���ٶȼ�����׼��
		for (j = 0; j < 2; j++)
			pt2g_SendBuffer[i * 2 + j] = (u8)(((*((s16*)ptF + i)) >> (j * 8)) & 0x00FF);	// ����ע��Խṹ��ָ�����⣬��ΪMPU6050�ṹ����ȫ��Ϊs16���ͣ����Բ���Ҫ�漰����������
			
	temp = (s16)(g_Pitch * 10);
	pt2g_SendBuffer[12] = (temp >> 8) & 0x00FF;		//��8λ
	pt2g_SendBuffer[13] = temp & 0x00FF;			//��8λ
	temp = (s16)(g_Roll * 10);
	pt2g_SendBuffer[14] = (temp >> 8) & 0x00FF;		//��8λ
	pt2g_SendBuffer[15] = temp & 0x00FF;			//��8λ
	temp = (s16)(g_Yaw * 10);
	pt2g_SendBuffer[16] = (temp >> 8) & 0x00FF;		//��8λ
	pt2g_SendBuffer[17] = temp & 0x00FF;			//��8λ
}

/********
main����
********/
int main(void)
{
	u8 convert_flag = 0;				// �ñ�����������2���ڿ������ڵ�ʱ��,����DMA�Ĵ��ڴ�������
	u8 g_SendBuffer[12 + 2*3] = {0};	// ���ڷ��ͻ�����
	u8 old_NRFRevCnt = 0;				// ǰһ��NRF���ռ�����
	u8 control_times = 0;				// ��ң��������֮��Ŀ��ƴ���

#ifdef BOOTLOADER
  NVIC_SetVectorTable(FLASH_BASE, 0x8000);	// ���������ж�������λ�ã�Bootloader����
#endif

	ChipHal_Init();		// Ƭ��Ӳ����ʼ��
	Delay_ms(500);		// �ϵ�500msϵͳ�������ȶ�
	ChipOutHal_Init();	// Ƭ��Ӳ����ʼ��
	Delay_ms(500);
	
	NRF24L01_RX_Mode();
	DMA_Configuration(DMA1_Channel4, (u32)&USART1->DR, (u32)g_SendBuffer, 12 + 2*3);	// ��ʼ��DMA,ͨ��4��ӦUSART1
	IMU_Calibration();	// IMU�궨
	g_tim2counter = 0;
	g_ConCycT_flag = 0;
	while(1)
	{
		if (g_ConCycT_flag)	// 3ms�������ڽ���һ��
		{
			g_ConCycT_flag = 0;
			convert_flag = !convert_flag;
			ReadFromIMU();		// ��ȡMPU6050��ֵ
			IMU_Filter();		// ���ٶȼ��˲��������Ǳ궨���
			IMUupdata(g_MPU6050Data_Filter.gyro_x_c, g_MPU6050Data_Filter.gyro_y_c, g_MPU6050Data_Filter.gyro_z_c,	// ��Ԫ����̬����
								g_MPU6050Data_Filter.accel_x_f * 0.0098, g_MPU6050Data_Filter.accel_y_f * 0.0098, g_MPU6050Data_Filter.accel_z_f * 0.0098);	//*0.0098��gת��Ϊmg
			if (convert_flag)	// 2*3=6ms����һ��
			{
				USARTData_Prepare(g_SendBuffer);	// ��������׼��
				DMA_StartTrans(DMA1_Channel4);		// ����һ��DMA����
			}
			if (g_NRFRevCnt != old_NRFRevCnt || control_times != 0)	// ��ң�ؿ���������
			{
				PWM1_LED = 0;
				control_times++;
				if (control_times == 50)	// ����50��,��150ms
					control_times = 0;
				Quadrotor_Control(g_Exp_Pitch,g_Exp_Roll,0);	// ����PWM���������-->(0,0,0)Ϊ��ͣ
			}
			else	// ��ͣ
			{
				PWM1_LED = 1;
				Quadrotor_Control(0,0,0);
			}
			old_NRFRevCnt = g_NRFRevCnt;	// ���¼�����
			if(g_ConCycT_flag)	// ��ʱ���׵�����
				PWM1_LED = 0;
		}
	}
}

#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f10x.h"

void SPI_Configuration(void);		// ��ʼ��SPI��
u8 SPI_ReadWriteByte(u8 txData);	// SPI���߶�дһ���ֽ�

#endif

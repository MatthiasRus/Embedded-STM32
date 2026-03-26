/*
 * DMA_driver.h
 *
 *  Created on: Mar 26, 2026
 *      Author: mat-ros
 */

#ifndef INC_STM32L476XX_DMA_DRIVER_H_
#define INC_STM32L476XX_DMA_DRIVER_H_

#include "stm32l476xx.h"

#define SINGLE_MODE_DMA					0
#define CIRCULAR_MODE					1
#define DMA_CIRC_MODE_BIT				5
#define DMA_DIR_BIT						4

#define PRIORITY_VERYHIGH       		3
#define PRIORITY_HIGH					2
#define PRIORITY_MEDIUM					1
#define PRIORITY_LOW					0

#define DMA_PINC_BIT					6
#define DMA_MINC_BIT					7

#define MEMORY_SIZE8					0
#define MEMORY_SIZE16					1
#define MEMORY_SIZE32					2

#define DMA_PSIZE_BIT					8
#define DMA_MSIZE_BIT					10

#define PERIPHERAL_SIZE8				0
#define PERIPHERAL_SIZE16				1
#define PERIPHERAL_SIZE32				2

#define MEMORY_INCREMENT_EN_BIT			7
#define PERIPHERAL_INCREMENT_EN_BIT		6

#define DATA_TRANSFER_DIRECTION_MEMORY  		1
#define DATA_TRANSFER_DIRECTION_PERIPHERAL 		0

#define MEMORY_TO_MEMORY_MODE_BIT				14

#define DMA_REQUEST_CHANNEL1_BIT				0
#define DMA_REQUEST_CHANNEL2_BIT				4
#define DMA_REQUEST_CHANNEL3_BIT				8
#define DMA_REQUEST_CHANNEL4_BIT				12
#define DMA_REQUEST_CHANNEL5_BIT				16
#define DMA_REQUEST_CHANNEL6_BIT				20
#define DMA_REQUEST_CHANNEL7_BIT				24

#define REQUEST_CHANNEL_BIT(x)		 (  	(x == 1) ? DMA_REQUEST_CHANNEL1_BIT :\
											(x == 2) ? DMA_REQUEST_CHANNEL2_BIT :\
											(x == 3) ? DMA_REQUEST_CHANNEL3_BIT :\
											(x == 4) ? DMA_REQUEST_CHANNEL4_BIT :\
											(x == 5) ? DMA_REQUEST_CHANNEL5_BIT :\
											(x == 6) ? DMA_REQUEST_CHANNEL6_BIT :\
											(x == 7) ? DMA_REQUEST_CHANNEL7_BIT : 0)
typedef struct{
	uint32_t MemoryAddress;    // buffer loc
	uint32_t PeripheralAddress; // ex. usart
	uint32_t TransferSize;
	uint8_t  Priority;
	uint8_t  Mode;
	uint8_t  MemorySize;
	uint8_t  PeripherialSize;
	uint8_t  Direction;
	uint8_t  Channel;
	uint8_t  RequestNumber;
}DMAx_Config_t;

typedef struct{
	DMA_RegDef_t *DMAx;
	DMAx_Config_t DMAx_config;
}DMAx_Handle_t;

void DMAx_Peripheral_Clock_Control(DMA_RegDef_t* DMAx, uint8_t EnOrDi);

void DMAx_Init(DMAx_Handle_t* DMAx_Handle);
void DMAx_Start(DMAx_Handle_t* DMAx_Handle);
void DMAx_Stop(DMAx_Handle_t* DMAx_Handle);
#endif /* INC_STM32L476XX_DMA_DRIVER_H_ */

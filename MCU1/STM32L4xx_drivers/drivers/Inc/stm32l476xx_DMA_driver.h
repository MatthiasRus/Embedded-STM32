/*
 * DMA_driver.h
 *
 *  Created on: Mar 26, 2026
 *      Author: mat-ros
 */

#ifndef INC_STM32L476XX_DMA_DRIVER_H_
#define INC_STM32L476XX_DMA_DRIVER_H_

#include "stm32l476xx.h"
#include "stm32l476xx_usart_driver.h"

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

#define REQUEST_CHANNEL_BIT(x)		 ( (x - 1) * 4)

#define DMA_TCIE_BIT                           1
#define DMA_HTIE_BIT						   2
#define DMA_TCIF6_BIT						   21

#define DMA_IFCR_CGIF_BIT(ch)    (((ch) - 1) * 4)
#define DMA_IFCR_CTCIF_BIT(ch)   (((ch) - 1) * 4 + 1)
#define DMA_IFCR_CHTIF_BIT(ch)   (((ch) - 1) * 4 + 2)
#define DMA_IFCR_CTEIF_BIT(ch)   (((ch) - 1) * 4 + 3)


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
void DMAx_Start(DMAx_Handle_t* DMAx_Handle, uint16_t size);
void DMAx_Stop(DMAx_Handle_t* DMAx_Handle);

void DMAx_Intrrupt_Config(uint8_t IRQNumber, uint8_t EnorDi);
void DMAx_Priority_Config(uint8_t IRQNumber, uint8_t IRQPriority);
void DMAx_EnabelInterrupt(DMAx_Handle_t* DMAx_Handle,uint8_t HTIE_DMA,uint8_t TCIE_DMA);
void DMAx_ClearFlags(DMAx_Handle_t* DMAx_Handle, uint8_t flag);
void DMA_SendString(USART_Handle_t* USARTHandle, DMAx_Handle_t* DMAHandle, char *buffer, volatile uint8_t *tx_done);
#endif /* INC_STM32L476XX_DMA_DRIVER_H_ */

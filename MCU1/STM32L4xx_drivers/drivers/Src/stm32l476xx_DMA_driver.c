/*
 * stm32l476xx_DMA_driver.c
 *
 *  Created on: Mar 26, 2026
 *      Author: mat-ros
 */

#include "stm32l476xx.h"
#include "stm32l476xx_DMA_driver.h"


void DMAx_Peripheral_Clock_Control(DMA_RegDef_t* DMAx, uint8_t EnOrDi){
	if (EnOrDi == ENABLE){
		if (DMAx == DMA1){
		DMA1_PCLK_EN();
		}else if (DMAx == DMA2){
			DMA2_PCLK_EN();
		}
	}else{
		if (DMAx == DMA1){
			DMA1_PCLK_DI();
		}else if (DMAx == DMA2){
			DMA2_PCLK_DI();
		}
	}

}


void DMAx_Init(DMAx_Handle_t* DMAx_Handle){
	uint8_t channel = DMAx_Handle->DMAx_config.Channel;
	DMAx_Peripheral_Clock_Control(DMAx_Handle->DMAx, ENABLE);

	DMAx_Handle->DMAx->CH[channel - 1].CCR &= ~(1 << 0);


	DMAx_Handle->DMAx->CSELR   &= ~(0xF << REQUEST_CHANNEL_BIT(channel));
	DMAx_Handle->DMAx->CSELR   |=  (DMAx_Handle->DMAx_config.RequestNumber << REQUEST_CHANNEL_BIT(channel));

	DMAx_Handle->DMAx->CH[channel - 1].CPAR   =   DMAx_Handle->DMAx_config.PeripheralAddress;
	DMAx_Handle->DMAx->CH[channel - 1].CCR   |=  (DMAx_Handle->DMAx_config.Mode  << DMA_CIRC_MODE_BIT);
	DMAx_Handle->DMAx->CH[channel - 1].CCR   |=  (DMAx_Handle->DMAx_config.Direction << DMA_DIR_BIT);
	DMAx_Handle->DMAx->CH[channel - 1].CCR   &=  ~(0x3 << DMA_MSIZE_BIT);
	DMAx_Handle->DMAx->CH[channel - 1].CCR   &=  ~(0x3 << DMA_PSIZE_BIT);
	DMAx_Handle->DMAx->CH[channel - 1].CCR   |=  (DMAx_Handle->DMAx_config.MemorySize << DMA_MSIZE_BIT);
	DMAx_Handle->DMAx->CH[channel - 1].CCR   |=  (DMAx_Handle->DMAx_config.PeripherialSize << DMA_PSIZE_BIT);
	DMAx_Handle->DMAx->CH[channel - 1].CMAR   =   DMAx_Handle->DMAx_config.MemoryAddress;
	DMAx_Handle->DMAx->CH[channel - 1].CNDTR  =   DMAx_Handle->DMAx_config.TransferSize;
	DMAx_Handle->DMAx->CH[channel - 1].CCR   |=  (ENABLE   <<  DMA_MINC_BIT);
	DMAx_Handle->DMAx->CH[channel - 1].CCR   &=  ~(ENABLE   <<  DMA_PINC_BIT);
}

void DMAx_Start(DMAx_Handle_t* DMAx_Handle){
	uint8_t channel = DMAx_Handle->DMAx_config.Channel;
	DMAx_Handle->DMAx->CH[channel-1].CCR |= (1 << 0);
}

void DMAx_Stop(DMAx_Handle_t* DMAx_Handle){
	uint8_t channel = DMAx_Handle->DMAx_config.Channel;
	DMAx_Handle->DMAx->CH[channel - 1].CCR &= ~(1 << 0);
}

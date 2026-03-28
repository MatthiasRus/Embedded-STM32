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
	DMAx_Peripheral_Clock_Control(DMAx_Handle->DMAx, ENABLE);
	uint8_t channel = DMAx_Handle->DMAx_config.Channel;

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

	DMAx_Handle->DMAx->CH[channel - 1].CCR |= 	 (1 << DMA_TCIE_BIT);  // TCIE
	DMAx_Handle->DMAx->CH[channel - 1].CCR |= 	 (1 << DMA_HTIE_BIT);  // HTIE
}

void DMAx_Start(DMAx_Handle_t* DMAx_Handle){
	uint8_t channel = DMAx_Handle->DMAx_config.Channel;
	DMAx_Handle->DMAx->CH[channel-1].CCR |= (1 << 0);
}

void DMAx_Stop(DMAx_Handle_t* DMAx_Handle){
	uint8_t channel = DMAx_Handle->DMAx_config.Channel;
	DMAx_Handle->DMAx->CH[channel - 1].CCR &= ~(1 << 0);
}

void DMAx_Intrrupt_Config(uint8_t IRQNumber, uint8_t EnorDi){
	if (EnorDi == ENABLE){
					// Setting from M4 General USER GUIDE
					// ISER ===>   Interrupt Set-enable Registers
					if (IRQNumber <= 31){
						// ISER0 for 0 to 31
						*NVIC_ISER0 |= (1 << IRQNumber);

					}else if (IRQNumber > 31 && IRQNumber < 64){
						// ISER1 for  32 to  63
						*NVIC_ISER1 |= (1 << ( IRQNumber % 32));

					}else if (IRQNumber >= 64 && IRQNumber < 96){
						// ISER2 for   64  to  95
						*NVIC_ISER2 |= (1 << ( IRQNumber % 64));
					}
				}

				else {
					// Clearing from M4 General USER GUIDE
					// ICER ===>   Interrupt Clear-enable Registers
					if (IRQNumber <= 31){
						// ICER0 for 0 to 31
						*NVIC_ICER0 |= (1 << IRQNumber);

					}else if (IRQNumber > 31 && IRQNumber < 64){
						// ICER1 for  32 to  63
						*NVIC_ICER1 |= (1 << IRQNumber % 32);


					}else if (IRQNumber >= 64 && IRQNumber < 96){
						// ICER2 for   64  to  95
						*NVIC_ICER2 |= (1 << IRQNumber % 64);
					}
				}
}

void DMAx_Priority_Config(uint8_t IRQNumber, uint8_t IRQPriority){
				uint8_t iprx = IRQNumber / 4;
				uint8_t iprx_Section = IRQNumber % 4;
				uint8_t Shift_amount = (8 * iprx_Section) + (8 - NO_PR_BITS_IMPLEMENTED);
				*(NVIC_IPR_BASE_ADDR + iprx) |= (IRQPriority << Shift_amount);

}

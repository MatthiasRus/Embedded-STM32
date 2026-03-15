
#include "stm32l476xx.h"

uint8_t SPI_Get_FlagStatus(SPI_RegDef_t *pSPIx, uint32_t Flag_Name){
	if (pSPIx->SR & Flag_Name){
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void SPI_Periph_ClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
	if (EnorDi == ENABLE){
		if (pSPIx == SPI1){
			SPI1_PCLK_EN();
		}else if (pSPIx == SPI2){
			SPI2_PCLK_EN();
		}else if (pSPIx == SPI3){
			SPI3_PCLK_EN();
		}
	}else{
		if (pSPIx == SPI1){
					SPI1_PCLK_DI();
				}else if (pSPIx == SPI2){
					SPI2_PCLK_DI();
				}else if (pSPIx == SPI3){
					SPI3_PCLK_DI();
				}
	}
}

/*------------------------------SPI initialization and de-initialization------*/

void SPI_Init(SPI_Handle_t *pSPIHandle){
	// SPI Peripheral Clock Init
	SPI_Periph_ClockControl(pSPIHandle->pSPIx, ENABLE);
	uint32_t tempReg = 0;

	// Device Mode Configuration
	tempReg |= pSPIHandle->SPI_Config.SPI_DeviceMode << SPI_CR1_DEVICE_MODE;

	// Bus Configuration
	if ( pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUS_CONFIG_FD){
		// we should clear the bidi Mode
		 tempReg  &= ~(1 << SPI_CR1_BIDI);
	}else if ( pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUS_CONFIG_HD){
		// we should set the bidi mode
		tempReg  |= (1 << SPI_CR1_BIDI);

	}else if ( pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY){
		// we should clear the bidi mode and enable rx only
		tempReg  &= ~(1 << SPI_CR1_BIDI);
		tempReg  |= (1 << SPI_CR1_RX);

	}

	// Serial Clock Speed Configuration
	tempReg |= ( pSPIHandle->SPI_Config.SPI_SclkSpeed << SPI_CR1_SCLK);

	// Data Frame Format Config
	tempReg |= ( pSPIHandle->SPI_Config.SPI_DFF << SPI_CR1_DFF);

	// CPOL
	tempReg |= ( pSPIHandle->SPI_Config.SPI_CPOL << SPI_CR1_CPOL);

	//CPHA
	tempReg |= ( pSPIHandle->SPI_Config.SPI_CPHA << SPI_CR1_CPHA);

	tempReg |= ( pSPIHandle->SPI_Config.SPI_SSM <<  SPI_CR1_SSM);

	pSPIHandle->pSPIx->CR1 = tempReg;


	// Now we have to configure the FRXTH -> FIFO recp. threshold
	if (pSPIHandle->SPI_Config.SPI_DFF == SPI_DFF_8BITS){
		pSPIHandle->pSPIx->CR2   |=  (1 << SPI_CR2_FRXTH);
	}else{
		pSPIHandle->pSPIx->CR2   &= ~(1 << SPI_CR2_FRXTH);
	}

	// Now let's set DS bits

	if (pSPIHandle->SPI_Config.SPI_DFF  == SPI_DFF_8BITS){
		pSPIHandle->pSPIx->CR2   &= ~(0xF  << SPI_CR2_DATASIZE);
		pSPIHandle->pSPIx->CR2   |=  (0x7 << SPI_CR2_DATASIZE);
	}else if (pSPIHandle->SPI_Config.SPI_DFF  == SPI_DFF_16BITS){
		pSPIHandle->pSPIx->CR2   &= ~(0xF  << SPI_CR2_DATASIZE);
		pSPIHandle->pSPIx->CR2   |=  (0xF << SPI_CR2_DATASIZE);
	}


}
void SPI_DeInit(SPI_RegDef_t *pSPIx){
	// Peripheral clock Reset in one go
	if (pSPIx == SPI1){
		SPI1_REG_RESET();
		}else if(pSPIx == SPI2){
			SPI2_REG_RESET();
		}else if(pSPIx == SPI3){
			SPI3_REG_RESET();
		}
}


/*-----------------------------Send and Receive Data-----------------------------*/

/*				Polling OR Blocking Call */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len){
		//Memory Corruption Guard
		if((pSPIx->CR1 & (1 << SPI_CR1_DFF)) && (Len % 2 != 0)) return;  // silently reject bad input
		while (Len > 0){
			// Let's Wait until TXE is Set
			while (SPI_Get_FlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

			// check the DFF  CRC (Cyclic Redundancy Check)
			if (pSPIx->CR1 & (1 << SPI_CR1_DFF)){
				// 16 Bit field
				// load the data on the data register
				pSPIx->DR = *((uint16_t*)pTxBuffer);
				Len--;Len--; // We just have loaded two bytes of data
				// We have to move pointer to the next data address
//				(uint16_t*)pTxBuffer++; // will be shifted 2 bytes due to the type casting
				pTxBuffer += 2;
			}else{
				// 8 bit field
				pSPIx->DR = *(pTxBuffer);
				Len--;
				pTxBuffer++;
			}


		}
}

void SPI_RecieveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len){
	while(Len > 0)
			{
				//1. wait until RXNE is set
				while(SPI_Get_FlagStatus(pSPIx,SPI_RXNE_FLAG)  == (uint8_t)FLAG_RESET );

				//2. check the DFF bit in CR1
				if( (pSPIx->CR1 & ( 1 << SPI_CR1_DFF) ) )
				{
					//16 bit DFF
					//1. load the data from DR to Rxbuffer address
					 *((uint16_t*)pRxBuffer) = pSPIx->DR ;
					Len--;
					Len--;
//					(uint16_t*)pRxBuffer++;
					pRxBuffer += 2;
				}else
				{
					//8 bit DFF
					*(pRxBuffer) = pSPIx->DR ;
					Len--;
					pRxBuffer++;
				}
			}
}

/*------------------------------IRQ and ISR Handling-----------------------------*/

void SPI_IRQInterrupt_Config(uint8_t IRQNumber, uint8_t EnorDi){
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


void SPI_IRQPriority_Config(uint8_t IRQNumber,uint32_t IRQPriority){
		uint8_t iprx = IRQNumber / 4;
		uint8_t iprx_Section = IRQNumber % 4;
		uint8_t Shift_amount = (8 * iprx_Section) + (8 - NO_PR_BITS_IMPLEMENTED);
		*(NVIC_IPR_BASE_ADDR + iprx) |= (IRQPriority << Shift_amount);

}

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len){

	uint8_t state = pSPIHandle->TxState;

	if (state != SPI_BUSY_IN_TX){
		// save the TX buffer address and length information in to global Variables
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = Len;

		// State the Tx line as Busy
		pSPIHandle->TxState = SPI_BUSY_IN_TX;

		// Enable TXEIE control bit to get interrupt whenever TXE flag set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);

	}

	return state;

}
uint8_t SPI_RecieveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len){
	uint8_t state = pSPIHandle->RxState;

		if (state != SPI_BUSY_IN_RX){
			// save the RX buffer address and length information in to global Variables
			pSPIHandle->pRxBuffer = pRxBuffer;
			pSPIHandle->RxLen = Len;

			// State the Rx line as Busy
			pSPIHandle->RxState = SPI_BUSY_IN_RX;

			// Enable RXNEIE control bit to get interrupt whenever RXE flag set in SR
			pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);

		}

		return state;

}
void SPI_IRQHandler(SPI_Handle_t *pHandle){

	uint8_t temp1, temp2;
	// Check for txe
	temp1 = pHandle->pSPIx->SR & ( 1 << SPI_SR_TXE);
	temp2 = pHandle->pSPIx->CR2 & ( 1 << SPI_CR2_TXEIE);

	if (temp1 && temp2){
		spi_txe_interrupt_handle(pHandle);
	}

	// Check for RXNE
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
	temp2 = pHandle->pSPIx->CR2 & ( 1 << SPI_CR2_RXNEIE);

	if (temp1 && temp2){
		spi_rxne_interrupt_handle(pHandle);
	}

	// Check for ovr
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	temp2 = pHandle->pSPIx->CR2 & ( 1 << SPI_CR2_ERRIE);

	if (temp1 && temp2){
			spi_ovr_err_interrupt_handle(pHandle);
		}

}

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
	if (EnOrDi == ENABLE){
			pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}else{
		pSPIx->CR1 &= ~( 1 << SPI_CR1_SPE);
	}
}

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
	if (EnOrDi == ENABLE){
			pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	}else{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);

	}

}

void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
	if (EnOrDi == ENABLE){
				pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
		}else{
			pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);

		}
}


static void  spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	// check the DFF bit in CR1
	if( (pSPIHandle->pSPIx->CR1 & ( 1 << SPI_CR1_DFF) ) )
	{
		//16 bit DFF
		//1. load the data in to the DR
		pSPIHandle->pSPIx->DR =   *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen--;
		pSPIHandle->TxLen--;
//		(uint16_t*)pSPIHandle->pTxBuffer++;
		pSPIHandle->pTxBuffer += 2;

	}else
	{
		//8 bit DFF
		pSPIHandle->pSPIx->DR =   *pSPIHandle->pTxBuffer;
		pSPIHandle->TxLen--;
		pSPIHandle->pTxBuffer++;
	}

	if(! pSPIHandle->TxLen)
	{
		//TxLen is zero , so close the spi transmission and inform the application that
		//TX is over.

		//this prevents interrupts from setting up of TXE flag
		SPI_CloseTransmisson(pSPIHandle);
//		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_TX_CMPLT);
	}

}


static void  spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	//do rxing as per the dff
	if(pSPIHandle->pSPIx->CR1 & ( 1 << 11))
	{
		//16 bit
		*((uint16_t*)pSPIHandle->pRxBuffer) = (uint16_t) pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen -= 2;
		pSPIHandle->pRxBuffer += 2;

	}else
	{
		//8 bit
		*(pSPIHandle->pRxBuffer) = (uint8_t) pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer++;
	}

	if(! pSPIHandle->RxLen)
	{
		//reception is complete
		SPI_CloseReception(pSPIHandle);
//		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_RX_CMPLT);
	}

}


static void  spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle)
{

	uint8_t temp;
	//1. clear the ovr flag
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
	{
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;
	}
	(void)temp;
	//2. inform the application
//	SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_OVR_ERR);

}


void SPI_CloseTransmisson(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~( 1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;

}

void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~( 1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;

}



void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;

}



// void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t AppEv)
//{
//
//	//This is weak implementation
//}

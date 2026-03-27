/*
 * DMA_USART_RX.c
 *
 *  Created on: Mar 27, 2026
 *      Author: mat-ros
 */


#include "stm32l476xx.h"


// GLOBAL Definition
USART_Handle_t usart2;
DMAx_Handle_t  dma1;
Servo_command cmd;
static int8_t bufferSize = 50;
int8_t channel = USART2_RX_DMA_CHANNEL_NO;
char buffer[50];

int main(void){
	return 0;
}


void USART2_IRQHandler(void){
	if(usart2.USARTx->ISR & (1 << USART_IDLE_BIT)){
		usart2.USARTx->ICR  |= (1 << USART_IDLE_CLEAR_BIT);

		DMAx_Stop(&dma1);

		uint32_t receivedLen = bufferSize - dma1.DMAx->CH[channel - 1].CNDTR;

		if (receivedLen > 0){
			USART_ParseData(buffer, &cmd);
		}
		dma1.DMAx_config.TransferSize = bufferSize;
		DMAx_Start(&dma1);
	}
}

/*
 * DMA_TX.c
 *
 *  Created on: Mar 30, 2026
 *      Author: mat-ros
 */


#include "stm32l476xx.h"
#include <string.h>

USART_Handle_t usart2;
DMAx_Handle_t dma1;
volatile int8_t tx_complete = 0;
static char msg[50];
static int8_t channel = USART2_TX_DMA_CHANNEL_NO;

int main(void){

	RCC->CR |= (1 << 8);
	while(!(RCC->CR & (1 << 10)));
	RCC->CFGR &= ~(3 << 0);
	RCC->CFGR |= (1 << 0);
	while((RCC->CFGR & (3 << 2)) != (1 << 2));
	memset(&usart2, 0, sizeof(usart2));
	SysTick_Init();

		usart2.USARTx = USART2;
		usart2.USARTx_Config.USART_BaudRate = USART_DIV_115200;
		usart2.USARTx_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
		usart2.USARTx_Config.USART_Mode = USART_MODE_TX_RX;
		usart2.USARTx_Config.USART_OverSampling = OVERSAMPLING_16;
		usart2.USARTx_Config.USART_Parity = NO_PARITY;
		usart2.USARTx_Config.USART_WordLength = EIGHT_BIT_WL;
		usart2.USARTx_Config.USART_StopBit = ONE_STOP_BIT;

		USART_Init(&usart2);
//		usart2.USARTx->CR1 |= (1 << USART_IDLE_BIT);

		USART2_DMA_TX_Enable(&usart2, &dma1, msg, 0);
		DMAx_EnableInterrupt(&dma1, ENABLE, DISABLE);   // TCIE yes, HTIE no
		DMAx_Intrrupt_Config(IRQ_NO_DMA1_CH7, ENABLE);
		DMAx_Priority_Config(IRQ_NO_DMA1_CH7, NVIC_IRQ_PRI15);
		tx_complete = 1;  // mark ready so first send goes through
		while(1){


					   DMA_SendString(&usart2,&dma1, "Pulse:2323\n", &tx_complete);


			 }
	return 0;
}

void DMA1_Channel7_IRQHandler(void){
    DMAx_ClearFlag(&dma1, DMA_IFCR_CTCIF_BIT(channel));
    usart2.USARTx->CR3 &= ~(1 << USART_DMAT_BIT);
    tx_complete = 1;
}

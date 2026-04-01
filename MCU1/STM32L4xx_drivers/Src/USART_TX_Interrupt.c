/*
 * USART_TX_Interrupt.c
 *
 *  Created on: Apr 1, 2026
 *      Author: mat-ros
 */


#include "stm32l476xx.h"
#include "stm32l476xx_usart_driver.h"

static USART_IRQ_TX_State_t tx_state = {0};
USART_Handle_t usart;
char* buf;

int main(void){
	RCC->CR |= (1 << 8);
	while(!(RCC->CR & (1 << 10)));
	RCC->CFGR &= ~(3 << 0);
	RCC->CFGR |= (1 << 0);
	while((RCC->CFGR & (3 << 2)) != (1 << 2));
	memset(&usart, 0, sizeof(usart));

	usart.USARTx = USART2;
	usart.USARTx_Config.USART_BaudRate = USART_DIV_115200;
	usart.USARTx_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	usart.USARTx_Config.USART_Mode = USART_MODE_TX_RX;
	usart.USARTx_Config.USART_OverSampling = OVERSAMPLING_16;
	usart.USARTx_Config.USART_Parity = NO_PARITY;
	usart.USARTx_Config.USART_StopBit = ONE_STOP_BIT;
	usart.USARTx_Config.USART_WordLength = EIGHT_BIT_WL;

	USART_Init(&usart);
	buf = "Hello\n";
	tx_state.buf = buf;
	USART2_Interrupt_Config(IRQ_NO_USART2, ENABLE);
	USART2_Priority_Config(IRQ_NO_USART2, NVIC_IRQ_PRI15);
	if(!tx_state.isActive){
			USART2_IRQ_TX_Start(&usart, &tx_state);
		}
	while(1){


	}


	return 0;
}


void USART2_IRQHandler(void){
	if (usart.USARTx->ISR & (1 << USART_TXE_BIT)){
		if(!tx_state.isActive) return;
		if  (tx_state.pos < tx_state.len){
		usart.USARTx->TDR = tx_state.buf[tx_state.pos];
		tx_state.pos++;

	}
		else{
			usart.USARTx->CR1  &= ~(1 << USART_TXEIE_BIT);
			tx_state.isActive = 0;

		}

	}

}

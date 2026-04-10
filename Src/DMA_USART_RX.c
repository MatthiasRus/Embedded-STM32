/*
 * DMA_USART_RX.c
 *
 *  Created on: Mar 27, 2026
 *      Author: mat-ros
 */


#include "stm32l476xx.h"
#include <string.h>

// GLOBAL Definition
USART_Handle_t usart2;
DMAx_Handle_t  dma1;
Servo_command cmd = {0};
uint8_t channel = USART2_RX_DMA_CHANNEL_NO;
static int8_t bufferSize = 50;
char buffer[50];
volatile uint8_t cmd_ready = 0;

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
	usart2.USARTx->CR1 |= (1 << USART_IDLE_BIT);

	USART2_DMA_RX_Enable(&usart2, &dma1, buffer, bufferSize);  // dma1 initialized first

	// Clear IDLE flag before enabling interrupt, in case it's already set
	usart2.USARTx->ICR |= (1 << USART_IDLE_CLEAR_BIT);

	USART2_Interrupt_Config(IRQ_NO_USART2, ENABLE);   // now safe
	USART2_Priority_Config(IRQ_NO_USART2, NVIC_IRQ_PRI15);

	 while(1){
		 if(cmd_ready){
			 cmd_ready = 0;
		   Servo_command local_cmd = cmd;
			 // Add debug echo here
		   USART_SendString(&usart2, "Pulse :");
		   USART_SendNumber(&usart2, local_cmd.pulse_us);
		   USART_SendString(&usart2, " CH:");
		   USART_SendNumber(&usart2, local_cmd.channel);
		   USART_SendChar(&usart2, '\n');

		   cmd.channel = 0;
		   cmd.pulse_us = 0;
		 }



	 }
	return 0;
}

void USART2_IRQHandler(void){
    if(usart2.USARTx->ISR & (1 << USART_IDLE_BIT)){
        usart2.USARTx->ICR |= (1 << USART_IDLE_CLEAR_BIT);

        DMAx_Stop(&dma1);

        uint32_t receivedLen = bufferSize - dma1.DMAx->CH[channel - 1].CNDTR;

        if (receivedLen > 0 && buffer[0] == '$'){
            buffer[receivedLen] = '\0';
            USART_ParseData(buffer, &cmd);
            cmd_ready = 1;
        }

        DMAx_Start(&dma1,bufferSize);
    }
}


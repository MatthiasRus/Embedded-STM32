/*
 * PWM_Generation.c
 *
 *  Created on: Mar 12, 2026
 *      Author: mat-ros
 */

#include "stm32l476xx.h"
#include <stdint.h>


int main(void){

	// clock setup
	    RCC->CR |= (1 << 8);
	    while(!(RCC->CR & (1 << 10)));
	    RCC->CFGR &= ~(3 << 0);
	    RCC->CFGR |= (1 << 0);
	    while((RCC->CFGR & (3 << 2)) != (1 << 2));
				SysTick_Init();
				TIM_Handle_t tim2;
				tim2.pTIMx   = TIM2;
				PWM_Init(&tim2);

				USART_Handle_t usart;
					usart.USARTx = USART2;
					usart.USARTx_Config.USART_BaudRate = USART_DIV_115200;
					usart.USARTx_Config.USART_Mode = USART_MODE_TX_ONLY;
					usart.USARTx_Config.USART_OverSampling = OVERSAMPLING_16;
					usart.USARTx_Config.USART_WordLength = EIGHT_BIT_WL;
					usart.USARTx_Config.USART_StopBit = ONE_STOP_BIT;
					usart.USARTx_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
					usart.USARTx_Config.USART_Parity = NO_PARITY;

					USART_Init(&usart);

	while(1){
		for (volatile int i = 1000; i <= 2000; i+=10){
			TIM2->CCR1 = i;
			USART_SendNumber(&usart, i);
			USART_SendChar(&usart, '\n');
			Delay_ms(20);
		}

		for (volatile int i = 2000; i>=1000; i-=10){
			TIM2->CCR1 = i;
			USART_SendNumber(&usart, i);
			USART_SendChar(&usart, '\n');
			Delay_ms(20);
		}
	}

	return 0;
}


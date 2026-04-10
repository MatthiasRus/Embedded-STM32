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
				TIM_Handle_t tim3;

				tim2.pTIMx   = TIM2;
				tim3.pTIMx   = TIM3;


				PWM_TIM2_Init(&tim2);
				PWM_TIM3_Init(&tim3);


				USART_Handle_t usart;
					usart.USARTx = USART2;
					usart.USARTx_Config.USART_BaudRate = USART_DIV_115200;
					usart.USARTx_Config.USART_Mode = USART_MODE_TX_RX;
					usart.USARTx_Config.USART_OverSampling = OVERSAMPLING_16;
					usart.USARTx_Config.USART_WordLength = EIGHT_BIT_WL;
					usart.USARTx_Config.USART_StopBit = ONE_STOP_BIT;
					usart.USARTx_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
					usart.USARTx_Config.USART_Parity = NO_PARITY;

					USART_Init(&usart);
//
				while(1){
					char buf[50];
					Servo_command cmd;
					USART_ReceiveLine(&usart, buf, 50);
					if(buf[0] == '$'){
					    USART_ParseData(buf, &cmd);
					    USART_ServoCommand(&cmd, TIM2, TIM3);
					}

					// debug echo
					    USART_SendString(&usart, "Received: ");
					    USART_SendString(&usart, buf);
					    USART_SendChar(&usart, '\n');
					    USART_SendString(&usart, "Channel: ");
					    USART_SendNumber(&usart, cmd.channel);
					    USART_SendString(&usart, " Pulse: ");
					    USART_SendNumber(&usart, cmd.pulse_us);
					    USART_SendChar(&usart, '\n');
				}

	return 0;
}


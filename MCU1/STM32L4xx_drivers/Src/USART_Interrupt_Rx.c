/*
 * USART_Interrupt_Rx.c
 *
 *  Created on: Mar 24, 2026
 *      Author: mat-ros
 */

#include "stm32l476xx.h"
#include <string.h>

// --- GLOBAL VARIABLES (The Bridge between Main and ISR) ---
USART_Handle_t usart;
char rx_buffer[50];
volatile uint8_t rx_index = 0;
volatile uint8_t msg_ready = 0;


int main(void){
					// clock setup
					RCC->CR |= (1 << 8);
					while(!(RCC->CR & (1 << 10)));
					RCC->CFGR &= ~(3 << 0);
					RCC->CFGR |= (1 << 0);
					while((RCC->CFGR & (3 << 2)) != (1 << 2));
					memset(&usart, 0, sizeof(usart));
	                SysTick_Init();

	                usart.USARTx = USART2;
	                usart.USARTx_Config.USART_BaudRate = USART_DIV_115200;
	                usart.USARTx_Config.USART_Mode = USART_MODE_TX_RX;
	                usart.USARTx_Config.USART_OverSampling = OVERSAMPLING_16;
	                usart.USARTx_Config.USART_WordLength = EIGHT_BIT_WL;
	                usart.USARTx_Config.USART_StopBit = ONE_STOP_BIT;
	                usart.USARTx_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	                usart.USARTx_Config.USART_Parity = NO_PARITY;

	                USART_Init(&usart);

	                usart.USARTx->CR1 |= (1 << 5);  // RXNEIE enable

	                USART2_Interrupt_Config(IRQ_NO_USART2, ENABLE);
	                USART2_Priority_Config(IRQ_NO_USART2, NVIC_IRQ_PRI15);

	                while(1) {
	                        // The CPU chills here until the ISR flags that a full message is ready
	                        if (msg_ready == 1) {

	                            // WE HAVE A FULL LINE! Parse it safely.
	                        	if(rx_buffer[0] == '$'){
	                        	    Servo_command cmd;
	                        	    USART_ParseData(rx_buffer, &cmd);
	                        	    // Add debug echo here
	                        	    USART_SendString(&usart, "Pulse :");
	                        	    USART_SendNumber(&usart, cmd.pulse_us);
	                        	    USART_SendChar(&usart, '\n');
	                        	}


	                            // Reset everything for the next incoming message
	                            rx_index = 0;
	                            msg_ready = 0;
	                            memset(rx_buffer, 0, sizeof(rx_buffer)); // Optional, but good practice
	                        }
	                    }
	return 0;
}

void USART2_IRQHandler(void) {

    // 1. Clear Overrun error if it happens
    if(usart.USARTx->ISR & (1 << 3)){
        usart.USARTx->ICR |= (1 << 3);
    }

    // 2. Check if a byte actually arrived (RXNE flag)
    if(usart.USARTx->ISR & (1 << 5)){

        // Read exactly ONE byte (this automatically clears the RXNE flag)
        char c = (char)(usart.USARTx->RDR & 0xFF);

        // 3. Is it the end of the message? (Assuming '\n' or '$' ends it)
        if(c == '\n' || c == '\r'){
            rx_buffer[rx_index] = '\0'; // Null-terminate the string
            msg_ready = 1;              // Raise the flag for main()!
        }
        else {
            // Store the byte and move to the next slot
            if (rx_index < 49) {        // Prevent buffer overflow!
                rx_buffer[rx_index] = c;
                rx_index++;
            }
        }
    }
}

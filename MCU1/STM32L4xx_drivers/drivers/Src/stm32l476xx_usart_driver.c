/*
 * stm32l476xx_usart_driver.c
 *
 *  Created on: Mar 11, 2026
 *      Author: mat-ros
 */

#include "stm32l476xx.h"

#define  BAUD_RATE_VAL(n,fclk, baudRate)          ((fclk * n) / (baudRate))

void USART_Init(USART_Handle_t *pUSARTxHandle){
		// Step 1: Enable USART2 peripheral clock
			USART2_PCLK_EN();

		// step 1.1 GPIO pa2 init af7
			GPIO_Handle_t GPIO_UART;
				GPIO_UART.pGPIOx = GPIOA;
				GPIO_UART.GPIO_PIN_Config.GPIO_PinMode = GPIO_MODE_ALTFN;
				GPIO_UART.GPIO_PIN_Config.GPIO_AltFunMode = 7;
				GPIO_UART.GPIO_PIN_Config.GPIO_PinNumber = 2;
				GPIO_UART.GPIO_PIN_Config.GPIO_PinSpeed = GPIO_SP_HIGH;
				GPIO_UART.GPIO_PIN_Config.GPIO_PinPuPdControl = GPIO_NO_PUPD;
				GPIO_UART.GPIO_PIN_Config.GPIO_OPType = GPIO_OP_TYPE_PP;


				GPIO_Init(&GPIO_UART);
	    // Step 2: Configure word length, parity, mode in CR1
			if (pUSARTxHandle->USARTx_Config.USART_WordLength == SEVEN_BIT_WL){
				pUSARTxHandle->USARTx->CR1   |= (1 << 28);
				pUSARTxHandle->USARTx->CR1   &= ~(1 << 12);

			}else if (pUSARTxHandle->USARTx_Config.USART_WordLength == EIGHT_BIT_WL){
				pUSARTxHandle->USARTx->CR1   &= ~(1 << 28);
				pUSARTxHandle->USARTx->CR1   &= ~(1 << 12);

			}else if (pUSARTxHandle->USARTx_Config.USART_WordLength == NINE_BIT_WL){
				pUSARTxHandle->USARTx->CR1   |= (1 << 12);
				pUSARTxHandle->USARTx->CR1   &= ~(1 << 28);

			}
			// 2.2 Configure Mode

			if (pUSARTxHandle->USARTx_Config.USART_Mode == USART_MODE_TX_ONLY){
							pUSARTxHandle->USARTx->CR1   |= (1 << 3);

						}else if (pUSARTxHandle->USARTx_Config.USART_Mode == USART_MODE_RX_ONLY){
							pUSARTxHandle->USARTx->CR1   |= (1 << 2);

						}else if (pUSARTxHandle->USARTx_Config.USART_Mode == USART_MODE_TX_RX){
							pUSARTxHandle->USARTx->CR1   |= (1 << 2);
							pUSARTxHandle->USARTx->CR1   |= (1 << 3);

						}else{

						}

			// 2.5 configure the Parity shit

			if (pUSARTxHandle->USARTx_Config.USART_Parity == EVEN_PARITY){
			    pUSARTxHandle->USARTx->CR1 |= (1 << 10);  // PCE enable
				pUSARTxHandle->USARTx->CR1   &= ~(1 << 9);

			}else if (pUSARTxHandle->USARTx_Config.USART_Parity == ODD_PARITY){
			    pUSARTxHandle->USARTx->CR1 |= (1 << 10);  // PCE enable
			    pUSARTxHandle->USARTx->CR1   |= (1 << 9);
			}
			else{
			    pUSARTxHandle->USARTx->CR1 &= ~(1 << 10);  // PCE disable

			}

	    // Step 3: Configure stop bits in CR2
			if (pUSARTxHandle->USARTx_Config.USART_StopBit == ONE_STOP_BIT){
							pUSARTxHandle->USARTx->CR2   &= ~(1 << 13);
							pUSARTxHandle->USARTx->CR2   &= ~(1 << 12);

						}else if (pUSARTxHandle->USARTx_Config.USART_StopBit == TWO_STOP_BIT){
							pUSARTxHandle->USARTx->CR2   |= (1 << 13);
							pUSARTxHandle->USARTx->CR2   &= ~(1 << 12);

						}else if (pUSARTxHandle->USARTx_Config.USART_StopBit == HALF_STOP_BIT){
							pUSARTxHandle->USARTx->CR2   |= (1 << 12);
							pUSARTxHandle->USARTx->CR2   &= ~(1 << 13);

						}else if (pUSARTxHandle->USARTx_Config.USART_StopBit == OAH_STOP_BIT){
							pUSARTxHandle->USARTx->CR2   |= (1 << 12);
							pUSARTxHandle->USARTx->CR2   |= (1 << 13);

						}else{
							// I really have to implement the error handling
						}
	    // Step 4: Configure HW flow control in CR3
			if (pUSARTxHandle->USARTx_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_NONE){
										pUSARTxHandle->USARTx->CR3   &= ~(1 << 9);
										pUSARTxHandle->USARTx->CR3   &= ~(1 << 8);

				}else if (pUSARTxHandle->USARTx_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS){
										pUSARTxHandle->USARTx->CR3   |= (1 << 9);

				}else if (pUSARTxHandle->USARTx_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS){
										pUSARTxHandle->USARTx->CR3   |= (1 << 8);

				}else if (pUSARTxHandle->USARTx_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS){
										pUSARTxHandle->USARTx->CR3   |= (1 << 9);
										pUSARTxHandle->USARTx->CR3   |= (1 << 8);

				}else{
										// I really have to implement the error handling
					}

			//5.0 Oversampling
			if (pUSARTxHandle->USARTx_Config.USART_OverSampling == OVERSAMPLING_8){
			    pUSARTxHandle->USARTx->CR1 |= (1 << 15);
			} else {
			    pUSARTxHandle->USARTx->CR1 &= ~(1 << 15);  // oversampling 16
			}
	    // Step 5: Configure BRR
			if (pUSARTxHandle->USARTx_Config.USART_OverSampling == OVERSAMPLING_16){
				pUSARTxHandle->USARTx->BRR  = BAUD_RATE_VAL(1,16000000,pUSARTxHandle->USARTx_Config.USART_BaudRate) ;
			}else if (pUSARTxHandle->USARTx_Config.USART_OverSampling == OVERSAMPLING_8){
				/*
				 * When OVER8 = 1:
					BRR[2:0] = USARTDIV[3:0] shifted 1 bit to the right.
					BRR[3] must be kept cleared.
				 */

					// i will implement it later

			}
	    // Step 6: Enable USART (UE bit in CR1)

			pUSARTxHandle->USARTx->CR1  |= (1 << 0);
}

void USART_DeInit(USART_RegDef_t *pUSARTx){

}


void USART_SendData(USART_Handle_t *pUSARTxHandle, uint8_t *pTxBuffer, uint32_t Len){

    for (uint32_t i = 0; i < Len; i++){
        while(!(pUSARTxHandle->USARTx->ISR & (1 << 7)));
        pUSARTxHandle->USARTx->TDR = pTxBuffer[i];
    }
    while(!(pUSARTxHandle->USARTx->ISR & (1 << 6)));

}

void USART_SendChar(USART_Handle_t *pUSARTxHandle, char c){
    while(!(pUSARTxHandle->USARTx->ISR & (1 << 7)));
    pUSARTxHandle->USARTx->TDR = (uint8_t)c;
}

void USART_SendString(USART_Handle_t *pUSARTxHandle, char *str){
    while(*str){
        USART_SendChar(pUSARTxHandle, *str++);
    }
    // wait TC once after entire string
    while(!(pUSARTxHandle->USARTx->ISR & (1 << 6)));
}

void USART_SendNumber(USART_Handle_t *pUSARTxHandle, int num){
    if(num < 0){
        USART_SendChar(pUSARTxHandle, '-');
        num = -num;
    }
    char buf[10];
    int i = 0;
    do {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    } while(num > 0);
    // send in reverse — digits were stored backwards
    for(int j = i-1; j >= 0; j--){
        USART_SendChar(pUSARTxHandle, buf[j]);
    }
}


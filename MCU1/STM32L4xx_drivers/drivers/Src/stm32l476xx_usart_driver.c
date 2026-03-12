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
			USART3_PCLK_EN();

		// step 1.1 GPIO pa2 init af7
			GPIO_Handle_t GPIO_USART_Tx;
			GPIO_USART_Tx.pGPIOx = GPIOA;
			GPIO_USART_Tx.GPIO_PIN_Config.GPIO_PinMode = GPIO_MODE_ALTFN;
			GPIO_USART_Tx.GPIO_PIN_Config.GPIO_AltFunMode = 7;
			GPIO_USART_Tx.GPIO_PIN_Config.GPIO_PinNumber = 2;
			GPIO_USART_Tx.GPIO_PIN_Config.GPIO_PinSpeed = GPIO_SP_HIGH;
			GPIO_USART_Tx.GPIO_PIN_Config.GPIO_PinPuPdControl = GPIO_NO_PUPD;
			GPIO_USART_Tx.GPIO_PIN_Config.GPIO_OPType = GPIO_OP_TYPE_PP;

			// step 1.1 GPIO pa3 init af7
			GPIO_Handle_t GPIO_USART_Rx;
			GPIO_USART_Rx.pGPIOx = GPIOA;
			GPIO_USART_Rx.GPIO_PIN_Config.GPIO_PinMode = GPIO_MODE_ALTFN;
			GPIO_USART_Rx.GPIO_PIN_Config.GPIO_AltFunMode = 7;
			GPIO_USART_Rx.GPIO_PIN_Config.GPIO_PinNumber = 3;
			GPIO_USART_Rx.GPIO_PIN_Config.GPIO_PinSpeed = GPIO_SP_HIGH;
			GPIO_USART_Rx.GPIO_PIN_Config.GPIO_PinPuPdControl = GPIO_PIN_PU;

			GPIO_Init(&GPIO_USART_Tx);
			GPIO_Init(&GPIO_USART_Rx);

			// Enable OVRDIS — disable overrun, never block on ORE
			pUSARTxHandle->USARTx->CR3 |= (1 << 12);
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

uint8_t USART_ReceiveChar(USART_Handle_t *h){
	// clear overrun error
	    if(h->USARTx->ISR & (1 << 3)){
	        h->USARTx->ICR |= (1 << 3);  // ORECF — clear ORE
	    }
		while(!(h->USARTx->ISR & (1 << 5)));
		uint8_t byte = (uint8_t)(h->USARTx->RDR & 0xFF);
		return byte;

}

void USART_ReceiveLine(USART_Handle_t *h, char *buf, uint32_t maxLen){
    uint32_t i = 0;
    char c;

    // wait for '$'
    do { c = USART_ReceiveChar(h); } while(c != '$');
    buf[i++] = '$';

    // read rest until newline
    while(i < maxLen - 1){
        c = USART_ReceiveChar(h);
        if(c == '\n' || c == '\r') break;
        buf[i++] = c;
    }
    buf[i] = '\0';
}

void USART_ParseData(char *buf, Servo_command *cmd){
	if (buf[0] == '$'){
		cmd->channel = buf[1] - '0';
		if (buf[2] == ':'){
			cmd->pulse_us = atoi(&buf[3]);
		}
	}
}

void USART_ServoCommand(Servo_command *cmd, TIM_RegDef_t *pTIM2, TIM_RegDef_t *pTIM3){
	if (cmd->pulse_us >= 1000 || cmd->pulse_us<=2000){
		switch(cmd->channel){
        case 1: pTIM2->CCR1 = cmd->pulse_us; break;
        case 2: pTIM2->CCR2 = cmd->pulse_us; break;
        case 3: pTIM3->CCR1 = cmd->pulse_us; break;
        case 4: pTIM3->CCR2 = cmd->pulse_us; break;
        case 5: pTIM3->CCR3 = cmd->pulse_us; break;
        case 6: pTIM3->CCR4 = cmd->pulse_us; break;
        default: break;
    }
	}

}

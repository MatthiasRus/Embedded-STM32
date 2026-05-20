/*
 * microros_transport.c
 *
 *  Created on: May 20, 2026
 *      Author: mat-ros
 */

#include "microros_transport.h"
#include "stm32l476xx.h"
#include "FreeRTOS.h"

bool transport_open(struct uxrCustomTransport* transport){
	if (RCC->APB1ENR1 & (1 << 17)){
		return true;
	}
	return false;
}

bool transport_close(struct uxrCustomTransport* transport){

	return true;
}


size_t transport_write(struct uxrCustomTransport* transport,
                       const uint8_t* buf,
                       size_t len,
                       uint8_t* errcode){
		size_t count = 0;

		USART_Handle_t* usart = (USART_Handle_t*)transport->args;

		for (size_t i = 0; i<len; i++){
			USART_SendChar(usart, (char)buf[i]);
			count++;
		}

		return count;
}

size_t transport_read(struct uxrCustomTransport* transport,
		uint8_t* buf, size_t len,
		int timeout_ms, uint8_t* errcode){
		USART_Handle_t* usart = (USART_Handle_t*)transport->args;
		size_t count = 0;

		TickType_t start = xTaskGetTickCount();

		for (size_t i = 0; i < len; i++){

				while(!((usart->USARTx->ISR) & (1 << 5))){
					if ((xTaskGetTickCount() - start ) > timeout_ms)return count;
					}

				buf[i] = USART_ReceiveChar(usart);
				count++;
		}
		return count;

}


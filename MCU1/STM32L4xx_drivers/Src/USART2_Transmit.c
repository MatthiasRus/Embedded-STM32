/*
 * USART2_Transmit.c
 *
 *  Created on: Mar 11, 2026
 *      Author: mat-ros
 */


#include "stm32l476xx.h"
#include <stdio.h>
#include <string.h>

#define  TS_CAL1  					(*(uint16_t*)0x1FFF75A8U)
#define  TS_CAL2 					(*(uint16_t*)0x1FFF75CAU)

int main(void){
			// Switch to HSI16 (16MHz) — simple and fast
			RCC->CR |= (1 << 8);              // HSION
			while(!(RCC->CR & (1 << 10)));    // wait HSIRDY
			RCC->CFGR &= ~(3 << 0);          // clear SW
			RCC->CFGR |= (1 << 0);           // SW = HSI16
			while((RCC->CFGR & (3 << 2)) != (1 << 2)); // wait SWS=HS
	// Enable FPU
			*((volatile uint32_t*)0xE000ED88) |= (0xF << 20);
			__asm volatile("DSB");
			__asm volatile("ISB");
			SysTick_Init();
	GPIO_Handle_t GPIO_UART;
	GPIO_UART.pGPIOx = GPIOA;
	GPIO_UART.GPIO_PIN_Config.GPIO_PinMode = GPIO_MODE_ALTFN;
	GPIO_UART.GPIO_PIN_Config.GPIO_AltFunMode = 7;
	GPIO_UART.GPIO_PIN_Config.GPIO_PinNumber = 2;
	GPIO_UART.GPIO_PIN_Config.GPIO_PinSpeed = GPIO_SP_HIGH;
	GPIO_UART.GPIO_PIN_Config.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_UART.GPIO_PIN_Config.GPIO_OPType = GPIO_OP_TYPE_PP;


	GPIO_Init(&GPIO_UART);


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
	char msg[32];


		ADC_Handle_t ADC ;

		ADC.ADCx = ADC1;
		ADC.ADC_configuration.ADC_Channel = 17;
		ADC.ADC_configuration.ADC_Resolution = 12;
		ADC.ADC_configuration.ADC_mode = 0;
		ADC.ADC_configuration.ADC_SamplingTime = 7;


		ADC_Init(&ADC);

		// enable the temperature sensor
		ADC1_COMMON->CCR |= (1 << 23);
		for (volatile int i = 0; i < 80000; i++);  // temp sensor startup ~10us minimum

		/*
		 * temp = ((110 - 30) / (TS_CAL2 - TS_CAL1)) * (adc_value - TS_CAL1) + 30
		 */
	while (1){
		float cal1 = TS_CAL1, cal2 = TS_CAL2;
		float adcValue = ADC_ReadData(&ADC);
		float adcCorrected = (float)adcValue * (3.3f / 3.0f);
		float temperature = ((80.0f / (cal2 - cal1)) * (adcCorrected - cal1)) + 30.0f;
		int temp_int = (int)temperature;
		int temp_dec = (int)((temperature - (float)temp_int) * 100);
		if(temp_dec < 0) temp_dec = -temp_dec;  // absolute value

		USART_SendString(&usart, "T: ");
		USART_SendNumber(&usart, temp_int);
		USART_SendChar(&usart, '.');
		if(temp_dec < 10) USART_SendChar(&usart, '0');  // leading zero
		USART_SendNumber(&usart, temp_dec);
		USART_SendString(&usart, " C\r\n");
		Delay_ms(1000);
	}


	return 0;
}

/*
 * Servo_Control.c
 *
 *  Created on: Mar 22, 2026
 *      Author: mat-ros
 */


#include "stm32l476xx.h"
#include "pca9685_driver.h"
#include <stdint.h>


int main(void){

		// clock setup
		RCC->CR |= (1 << 8);
		while(!(RCC->CR & (1 << 10)));
		RCC->CFGR &= ~(3 << 0);
		RCC->CFGR |= (1 << 0);
		while((RCC->CFGR & (3 << 2)) != (1 << 2));
		SysTick_Init();


		// I2C Control
		I2C_Handle_t I2C3_Master;
		I2C3_Master.I2Cx = I2C3;
		I2C3_Master.I2C_Config.I2C_SCLSpeed = SCLSpeed_FM4K;

		// USART2 Control
		USART_Handle_t usart2;
		usart2.USARTx = USART2;
		usart2.USARTx_Config.USART_Mode = USART_MODE_TX_RX;
		usart2.USARTx_Config.USART_BaudRate = USART_DIV_115200;
		usart2.USARTx_Config.USART_OverSampling = OVERSAMPLING_16;
		usart2.USARTx_Config.USART_Parity = NO_PARITY;
		usart2.USARTx_Config.USART_WordLength = EIGHT_BIT_WL;
		usart2.USARTx_Config.USART_StopBit    = ONE_STOP_BIT;
		usart2.USARTx_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;


		// Bus recovery — release SCL before I2C init
		GPIOC_PCLK_EN();
		GPIOC->MODER &= ~(3 << 0);
		GPIOC->MODER |= (1 << 0);   // PC0 output
		GPIOC->ODR |= (1 << 0);     // drive HIGH
		for(volatile int i = 0; i < 10000; i++);

		// Reset I2C3
		I2C3_REG_RESET();
		for(volatile int i = 0; i < 1000; i++);
		// PCA Control

		USART_Init(&usart2);
		I2C_Init(&I2C3_Master);
		PCA9685_Init(&I2C3_Master);


		// Main Logic for my own use case
		while (1){
			char buf[50];
			Servo_command cmd;
			USART_ReceiveLine(&usart2, buf, 50);
			if(buf[0] == '$'){
				USART_ParseData(buf, &cmd);
				PCA9685_SetPulse(&I2C3_Master, cmd.channel - 1, cmd.pulse_us);
			}

			// debug echo
			USART_SendString(&usart2, "Received: ");
			USART_SendString(&usart2, buf);
			USART_SendChar(&usart2, '\n');
			USART_SendString(&usart2, "Channel: ");
			USART_SendNumber(&usart2, cmd.channel);
			USART_SendString(&usart2, " Pulse: ");
			USART_SendNumber(&usart2, cmd.pulse_us);
			USART_SendChar(&usart2, '\n');
					}
	return 0;
}

/*
 * stm32l476xx_usart_driver.h
 *
 *  Created on: Mar 11, 2026
 *      Author: mat-ros
 */

#ifndef INC_STM32L476XX_USART_DRIVER_H_
#define INC_STM32L476XX_USART_DRIVER_H_
#include "stm32l476xx.h"

#define NO_PARITY    0
#define EVEN_PARITY  1
#define ODD_PARITY   2


/*
 * M1: Word length
This bit, with bit 12 (M0), determines the word length. It is set or cleared by software.
M[1:0] = 00: 1 Start bit, 8 data bits, n stop bits
M[1:0] = 01: 1 Start bit, 9 data bits, n stop bits
M[1:0] = 10: 1 Start bit, 7 data bits, n stop bits
 */
#define SEVEN_BIT_WL			2
#define EIGHT_BIT_WL			0
#define NINE_BIT_WL				1

/*
 * 00: 1 stop bit
01: 0.5 stop bit
10: 2 stop bits
11: 1.5 stop bits
 */

#define ONE_STOP_BIT			0
#define HALF_STOP_BIT			1
#define TWO_STOP_BIT			2
#define OAH_STOP_BIT			3


#define OVERSAMPLING_16			0
#define OVERSAMPLING_8			1

#define USART_DIV_9600			9600
#define USART_DIV_115200		115200
#define USART_DIV_38400 		38400


#define USART_MODE_TX_ONLY    1
#define USART_MODE_RX_ONLY    2
#define USART_MODE_TX_RX      3

#define USART_HW_FLOW_CTRL_NONE    0
#define USART_HW_FLOW_CTRL_CTS     1
#define USART_HW_FLOW_CTRL_RTS     2
#define USART_HW_FLOW_CTRL_CTS_RTS 3


typedef struct{
	uint32_t  USART_BaudRate;
	uint32_t  USART_WordLength;
	uint32_t  USART_StopBit;
	uint32_t  USART_Parity;
	uint32_t  USART_Mode;
	uint32_t  USART_HWFlowControl;
	uint32_t  USART_OverSampling;

}USART_Config_t;


typedef struct {
	USART_RegDef_t *USARTx;
	USART_Config_t USARTx_Config;
}USART_Handle_t;


typedef struct{
	uint8_t channel;
	uint16_t pulse_us;
}Servo_command;

void USART_Init(USART_Handle_t *pUSARTxHandle);
void USART_DeInit(USART_RegDef_t *pUSARTx);

// Transmitting Data
void USART_SendData(USART_Handle_t *pUSARTxHandle, uint8_t *pTxBuffer, uint32_t Len);
void USART_SendChar(USART_Handle_t *pUSARTxHandle, char c);
void USART_SendString(USART_Handle_t *pUSARTxHandle, char *str);
void USART_SendNumber(USART_Handle_t *pUSARTxHandle, int num);

// Receive data
uint8_t USART_ReceiveChar(USART_Handle_t *h);
void USART_ReceiveLine(USART_Handle_t *h, char *buf, uint32_t maxLen);
void USART_ParseData(char *buf , Servo_command *cmd);
void USART_ServoCommand(Servo_command *cmd, TIM_RegDef_t *pTIM2, TIM_RegDef_t *pTIM3);

// Interruption
void USART2_Interrupt_Config(uint8_t IRQNumber, uint8_t ENorDI);

void USART2_Priority_Config(uint8_t IRQNumber, uint8_t PriorityNumber);

//void USART2_IRQHandler(void);
#endif /* INC_STM32L476XX_USART_DRIVER_H_ */

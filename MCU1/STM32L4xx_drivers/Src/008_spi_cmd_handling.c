/*
 * 006spi_tx_testing.c
 *
 *  Created on: Feb 10, 2019
 *      Author: admin
 */

#include<stdio.h>
#include<string.h>
#include "stm32l476xx.h"

//extern void initialise_monitor_handles();

//command codes
#define COMMAND_LED_CTRL      		0x50
#define COMMAND_SENSOR_READ      	0x51
#define COMMAND_LED_READ      		0x52
#define COMMAND_PRINT      			0x53
#define COMMAND_ID_READ      		0x54

#define LED_ON     1
#define LED_OFF    0

//arduino analog pins
#define ANALOG_PIN0 	0
#define ANALOG_PIN1 	1
#define ANALOG_PIN2 	2
#define ANALOG_PIN3 	3
#define ANALOG_PIN4 	4

//arduino led

#define LED_PIN  9

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}

/*
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * PB13 -> SPI2_SCLK
 * PB12 --> SPI2_NSS
 * ALT function mode : 5
 */
void SPI2_GPIO_Inits(void){
	 GPIO_Handle_t SPI_Pins;
	 SPI_Pins.pGPIOx  = GPIOB;
	 SPI_Pins.GPIO_PIN_Config.GPIO_PinMode = GPIO_MODE_ALTFN;
	 SPI_Pins.GPIO_PIN_Config.GPIO_AltFunMode = 5;
	 SPI_Pins.GPIO_PIN_Config.GPIO_OPType = GPIO_OP_TYPE_PP;
	 SPI_Pins.GPIO_PIN_Config.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	 SPI_Pins.GPIO_PIN_Config.GPIO_PinSpeed = GPIO_SP_HIGH;

	 // Pin configuring for SCK
	 SPI_Pins.GPIO_PIN_Config.GPIO_PinNumber = 3;
	 GPIO_Init(&SPI_Pins);

	 // Pin configuring for MOSI
	 SPI_Pins.GPIO_PIN_Config.GPIO_PinNumber = 5;
	 GPIO_Init(&SPI_Pins);

	 // Pin configuring for MISO
	 SPI_Pins.GPIO_PIN_Config.GPIO_PinNumber = 14;
	 GPIO_Init(&SPI_Pins);

	 // Pin configuring for NSS
	 SPI_Pins.GPIO_PIN_Config.GPIO_PinNumber = 12;
	 GPIO_Init(&SPI_Pins);
}

void SPI2_Inits(void){
	SPI_Handle_t SPI_Handle;
	SPI_Handle.pSPIx = SPI1;
	SPI_Handle.SPI_Config.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI_Handle.SPI_Config.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI_Handle.SPI_Config.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;
	SPI_Handle.SPI_Config.SPI_DFF = SPI_DFF_8BITS;
	SPI_Handle.SPI_Config.SPI_CPOL = SPI_CPOL_LOW;
	SPI_Handle.SPI_Config.SPI_CPHA = SPI_CPHA_LOW;
	SPI_Handle.SPI_Config.SPI_SSM = SPI_SSM_DI; // Software Slave Management Enabled

	SPI_Init(&SPI_Handle);
}


void GPIO_Button_Init(void){

		GPIO_Handle_t GpioButton;
	 // GPIO Button Definition
		GpioButton.pGPIOx = GPIOC;		// Button
	    GpioButton.GPIO_PIN_Config.GPIO_PinNumber = GPIO_PIN_13;
	    GpioButton.GPIO_PIN_Config.GPIO_PinMode = GPIO_MODE_IN;
	    GpioButton.GPIO_PIN_Config.GPIO_PinSpeed = GPIO_SP_HIGH;
	    GpioButton.GPIO_PIN_Config.GPIO_PinPuPdControl = GPIO_PIN_PU;   // pull up is available on the board itself


	    GPIO_Init(&GpioButton);
}


uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{

	if(ackbyte == (uint8_t)0xF5)
	{
		//ack
		return 1;
	}

	return 0;
}


int main(void){

	uint8_t dummy_write = 0xff;
	uint8_t dummy_read;
	uint8_t args[2];
		//initialise_monitor_handles();

	printf("Application is running\n");

	GPIO_Button_Init();

	SPI2_GPIO_Inits();

	SPI2_Inits();

	SPI_SSOEConfig(SPI1, ENABLE);
	while(1){
		while(!GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_13));

	delay();
	// Let's enable the SPI using SPE
	SPI_PeripheralControl(SPI1, ENABLE);

	uint8_t commandCode = COMMAND_LED_CTRL;
	SPI_SendData(SPI1, &commandCode,1);

	// read to clear off RXNE
	SPI_RecieveData(SPI1, &dummy_read, 1);

	uint8_t ackbyte;
	SPI_SendData(SPI1, &dummy_write,1);

	SPI_RecieveData(SPI1, &ackbyte, 1);

	if (SPI_VerifyResponse(ackbyte)){
		args[0] = LED_PIN;
		args[1] = LED_ON;
		SPI_SendData(SPI1, &args,2);

	}

	// Commands code CMD SENSOR READ
	while(!GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_13));

    delay();

		commandCode = COMMAND_SENSOR_READ;
		SPI_SendData(SPI1, &commandCode,1);


		// read to clear off RXNE
		SPI_RecieveData(SPI1, &dummy_read, 1);

		SPI_SendData(SPI1, &dummy_write,1);

		SPI_RecieveData(SPI1, &ackbyte, 1);

		if (SPI_VerifyResponse(ackbyte)){
				args[0] = ANALOG_PIN0;
				SPI_SendData(SPI1, &args,1);
				// read to clear off RXNE
						SPI_RecieveData(SPI1, &dummy_read, 1);
						delay();
						SPI_SendData(SPI1, &dummy_write,1);
						uint8_t analog_read;
						SPI_SendData(SPI1, &analog_read,1);
			}



	while(SPI_Get_FlagStatus(SPI1, SPI_BSY_FLAG));
	SPI_PeripheralControl(SPI1, DISABLE);

	}

	return 0;
}

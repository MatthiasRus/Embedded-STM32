/*
 * 007SPI_txonly_arduino.c
 *
 *  Created on: Sep 29, 2025
 *      Author: matth_ik1hap6
 */


#include "stm32l476xx.h"
#include <string.h>
// PB15    MOSI
// PB12	   NSS
// PB13    SCK
// PB14	   MISO

void delay(void){
	for (int i = 0; i < 500000/2; i++);
}

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

//	 // Pin configuring for MISO
//	 SPI_Pins->GPIO_PIN_Config.GPIO_PinNumber = 14;
//	 GPIO_Init(&SPI_Pins);
//
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



int main(void){

	char userData[] = "Hello World";
	GPIO_Button_Init();

	SPI2_GPIO_Inits();

	SPI2_Inits();

	SPI_SSOEConfig(SPI1, ENABLE);
	while(1){
		while(! GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_13));

	delay();
	// Let's enable the SPI using SPE
	SPI_PeripheralControl(SPI1, ENABLE);
	SPI_SendData(SPI1, (uint8_t*)userData, strlen(userData));

	while( ! SPI_Get_FlagStatus(SPI1, SPI_BSY_FLAG));
	SPI_PeripheralControl(SPI1, DISABLE);

	}

	return 0;
}

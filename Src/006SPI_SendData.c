#include "stm32l476xx.h"
#include <string.h>
// PB15    MOSI
// PB12	   NSS
// PB13    SCK
// PB14	   MISO


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
//	 // Pin configuring for NSS
//	 SPI_Pins->GPIO_PIN_Config.GPIO_PinNumber = 12;
//	 GPIO_Init(&SPI_Pins);
}


void SPI2_Inits(void){
	SPI_Handle_t SPI_Handle;
	SPI_Handle.pSPIx = SPI1;
	SPI_Handle.SPI_Config.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI_Handle.SPI_Config.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI_Handle.SPI_Config.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;
	SPI_Handle.SPI_Config.SPI_DFF = SPI_DFF_8BITS;
	SPI_Handle.SPI_Config.SPI_CPOL = SPI_CPOL_LOW;
	SPI_Handle.SPI_Config.SPI_CPHA = SPI_CPHA_LOW;
	SPI_Handle.SPI_Config.SPI_SSM = SPI_SSM_EN; // Software Slave Management Enabled

	SPI_Init(&SPI_Handle);
}




int main(void){

	char userData[] = "Hello World";

	SPI2_GPIO_Inits();

	SPI2_Inits();
	// This makes the NSS internally HIGH and avoid the MODF Error
	SPI_SSIConfig(SPI1, ENABLE);
	// Let's enable the SPI using SPE
	SPI_PeripheralControl(SPI1, ENABLE);

	uint8_t dataLen = strlen(userData);
	SPI_SendData(SPI1, &dataLen, 1);

	while( ! SPI_Get_FlagStatus(SPI1, SPI_BSY_FLAG));
	SPI_PeripheralControl(SPI1, DISABLE);

	while(1);
	return 0;
}

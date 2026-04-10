/*
 * SPI_MS_arudino_FD.c
 *
 *  Created on: Mar 14, 2026
 *      Author: mat-ros
 */


#include "stm32l476xx.h"
#include <string.h>
#include <stdint.h>

static void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 800; i++);
}

int main(void){

	RCC->CR   |=  (1 << 8);  // HSI ON
	while(!(RCC->CR  & (1 << 10))); // Wait until HSIRDY flag set
	RCC->CFGR   &=  ~(3  <<   0);   // Clear SW bits[1:0]
	RCC->CFGR   |=   (1  <<   0);   // SET clock as HSI16 on clock Switch
	while((RCC->CFGR  & (3 << 2)) !=(1 << 2)); // busy wait for clock switch status

	SysTick_Init();

	// This is SPI2 GPIO Setup
	GPIO_Handle_t GPIO_SPI_NSS;
		GPIO_SPI_NSS.pGPIOx  = GPIOB;
		GPIO_SPI_NSS.GPIO_PIN_Config.GPIO_AltFunMode = 5;
		GPIO_SPI_NSS.GPIO_PIN_Config.GPIO_OPType  = GPIO_OP_TYPE_PP;
		GPIO_SPI_NSS.GPIO_PIN_Config.GPIO_PinMode = GPIO_MODE_ALTFN;
		GPIO_SPI_NSS.GPIO_PIN_Config.GPIO_PinPuPdControl = GPIO_PIN_PU;
		GPIO_SPI_NSS.GPIO_PIN_Config.GPIO_PinNumber      = GPIO_PIN_12;

	GPIO_Handle_t GPIO_SPI_SCK;
		GPIO_SPI_SCK.pGPIOx  = GPIOB;
		GPIO_SPI_SCK.GPIO_PIN_Config.GPIO_AltFunMode = 5;
		GPIO_SPI_SCK.GPIO_PIN_Config.GPIO_OPType  = GPIO_OP_TYPE_PP;
		GPIO_SPI_SCK.GPIO_PIN_Config.GPIO_PinMode = GPIO_MODE_ALTFN;
		GPIO_SPI_SCK.GPIO_PIN_Config.GPIO_PinPuPdControl = GPIO_PIN_PU;
		GPIO_SPI_SCK.GPIO_PIN_Config.GPIO_PinNumber      = GPIO_PIN_13;


	GPIO_Handle_t GPIO_SPI_MISO;
		GPIO_SPI_MISO.pGPIOx  = GPIOB;
		GPIO_SPI_MISO.GPIO_PIN_Config.GPIO_AltFunMode = 5;
		GPIO_SPI_MISO.GPIO_PIN_Config.GPIO_PinMode = GPIO_MODE_ALTFN;
		GPIO_SPI_MISO.GPIO_PIN_Config.GPIO_PinPuPdControl = GPIO_PIN_PU;
		GPIO_SPI_MISO.GPIO_PIN_Config.GPIO_PinNumber      = GPIO_PIN_14;

	GPIO_Handle_t GPIO_SPI_MOSI;
		GPIO_SPI_MOSI.pGPIOx  = GPIOB;
		GPIO_SPI_MOSI.GPIO_PIN_Config.GPIO_AltFunMode = 5;
		GPIO_SPI_MOSI.GPIO_PIN_Config.GPIO_OPType  = GPIO_OP_TYPE_PP;
		GPIO_SPI_MOSI.GPIO_PIN_Config.GPIO_PinMode = GPIO_MODE_ALTFN;
		GPIO_SPI_MOSI.GPIO_PIN_Config.GPIO_PinPuPdControl = GPIO_PIN_PU;
		GPIO_SPI_MOSI.GPIO_PIN_Config.GPIO_PinNumber      = GPIO_PIN_15;


	// Ik i can do change only the gpio pin number and re-init per gpio
		GPIO_Init(&GPIO_SPI_NSS);
		GPIO_Init(&GPIO_SPI_SCK);
//		GPIO_Init(&GPIO_SPI_MISO);
		GPIO_Init(&GPIO_SPI_MOSI);


	SPI_Handle_t SPI2_Master;
	SPI2_Master.pSPIx  =  SPI2;
	SPI2_Master.SPI_Config.SPI_CPHA  = SPI_CPHA_LOW;
	SPI2_Master.SPI_Config.SPI_CPOL  = SPI_CPOL_LOW;
	SPI2_Master.SPI_Config.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2_Master.SPI_Config.SPI_DFF  = SPI_DFF_8BITS;
	SPI2_Master.SPI_Config.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2_Master.SPI_Config.SPI_SclkSpeed	= SPI_SCLK_SPEED_DIV8;
	SPI2_Master.SPI_Config.SPI_SSM          = SPI_SSM_DI;


	SPI_Init(&SPI2_Master);


	// Software Slave select enabled
	SPI_SSOEConfig(SPI2, ENABLE);

	SPI_PeripheralControl(SPI2, ENABLE);

	while(1){
	    SPI_SendData(SPI2, (uint8_t*)"Hello World", 11);
//	    SPI_PeripheralControl(SPI2, DISABLE);
	    delay_ms(100);
	}

	return 0;
}

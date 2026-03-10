/*
 * stm32l476xx_spi_driver.h
 *
 *  Created on: Sep 9, 2025
 *      Author: matth_ik1hap6
 */

#ifndef INC_STM32L476XX_SPI_DRIVER_H_
#define INC_STM32L476XX_SPI_DRIVER_H_

#include "stm32l476xx.h"



typedef struct{
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_SclkSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;
}SPI_Config_t;


typedef struct{
	SPI_RegDef_t *pSPIx;
	SPI_Config_t SPI_Config;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t TxState;
	uint8_t RxState;
}SPI_Handle_t;

static void spi_txe_interrupt_handle(SPI_Handle_t *pHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pHandle);

/* @SPI_DeviceMode*/

#define SPI_DEVICE_MODE_SLAVE						0
#define SPI_DEVICE_MODE_MASTER 						1

/* @SPI_BusConfig  */

#define SPI_BUS_CONFIG_FD							1
#define SPI_BUS_CONFIG_HD							2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY				3


/* @SPI_CLOCK SPEED*/

#define SPI_SCLK_SPEED_DIV2							0
#define SPI_SCLK_SPEED_DIV4							1
#define SPI_SCLK_SPEED_DIV8							2
#define SPI_SCLK_SPEED_DIV16						3
#define SPI_SCLK_SPEED_DIV32						4
#define SPI_SCLK_SPEED_DIV64						5
#define SPI_SCLK_SPEED_DIV128						6
#define SPI_SCLK_SPEED_DIV256						7


/* @SPI_Data Frame Format*/

#define SPI_DFF_8BITS								0
#define SPI_DFF_16BITS								1

/* @SPI_CPOL*/
#define SPI_CPOL_HIGH								1
#define SPI_CPOL_LOW								0

/* @SPI_CPHA*/
#define SPI_CPHA_HIGH								1
#define SPI_CPHA_LOW								0

/* @SPI_SSM*/

#define SPI_SSM_EN									1
#define SPI_SSM_DI									0



/*
 *	SPI Flags
 */
#define SPI_TXE_FLAG								(1 << SPI_SR_TXE)
#define SPI_RXNE_FLAG								(1 << SPI_SR_RXNE)
#define SPI_CRCERR_FLAG								(1 << SPI_SR_CRCERR)
#define SPI_MODF_FLAG								(1 << SPI_SR_MODF)
#define SPI_OVR_FLAG								(1 << SPI_SR_OVR)
#define SPI_BSY_FLAG								(1 << SPI_SR_BSY)
#define SPI_FRE_FLAG								(1 << SPI_SR_FRE)
#define SPI_FRLVL_FLAG								(1 << SPI_SR_FRLVL)
#define SPI_FtLVL_FLAG								(1 << SPI_SR_FTLVL)


/* *****************/
#define SPI_READY									0
#define SPI_BUSY_IN_RX								1
#define SPI_BUSY_IN_TX								2

/* -----------------------------API Prototypes--------------------------------*/


/*----------------------------- peripheral clock setup-------------------------*/

void SPI_Periph_ClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/*------------------------------SPI initialization and de-initialization------*/

void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);   // Peripheral clock Reset in one go


/*-----------------------------Send and Receive Data-----------------------------*/

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_RecieveData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);


uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len);
uint8_t SPI_RecieveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len);

/*------------------------------IRQ and ISR Handling-----------------------------*/

void SPI_IRQInterrupt_Config(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriority_Config(uint8_t IRQNumber,uint32_t IRQPriority);
void SPI_IRQHandler(SPI_Handle_t *pHandle);

/*------------------------------Other Useful API's------------------------------*/

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

void SPI_CloseTransmisson(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);

void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t AppEv);

uint8_t SPI_Get_FlagStatus(SPI_RegDef_t *pSPIx, uint32_t Flag_Name);
#endif /* INC_STM32L476XX_SPI_DRIVER_H_ */

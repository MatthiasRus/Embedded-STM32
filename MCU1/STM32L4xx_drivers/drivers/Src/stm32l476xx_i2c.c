#include "stm32l476xx_i2c_driver.h"


void I2C_Periph_ClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi){
	if (EnorDi == ENABLE){
		if (pI2Cx == I2C1){
			I2C1_PCLK_EN();
		}else if(pI2Cx == I2C2 ){
			I2C2_PCLK_EN();
		}else if(pI2Cx == I2C3 ){
			I2C3_PCLK_EN();
		}
	}else{
		if (pI2Cx == I2C1){
					I2C1_PCLK_DI()();
				}else if(pI2Cx == I2C2 ){
					I2C2_PCLK_DI();
				}else if(pI2Cx == I2C3 ){
					I2C3_PCLK_DI();
				}
	}
}


void I2C_DeInit(I2C_RegDef_t *pI2Cx){
	// Peripheral clock Reset in one go
	if (pI2Cx == I2C1){
		I2C1_REG_RESET();
		}else if(pI2Cx == I2C2){
			I2C2_REG_RESET();
		}else if(pI2Cx == I2C3){
			I2C3_REG_RESET();
		}
}



void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi){
	if (EnOrDi == ENABLE){
				pI2Cx->CR1 |= (1 << PE);
		}else{
			pI2Cx->CR1 &= ~( 1 << PE);
		}
}

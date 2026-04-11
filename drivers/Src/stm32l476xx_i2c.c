#include "stm32l476xx.h"

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
					I2C1_PCLK_DI();
				}else if(pI2Cx == I2C2 ){
					I2C2_PCLK_DI();
				}else if(pI2Cx == I2C3 ){
					I2C3_PCLK_DI();
				}
	}

}

void I2C_Init(I2C_Handle_t *I2C_handle){
		I2C_Periph_ClockControl(I2C_handle->I2Cx, ENABLE);
		// We have to release the i2c lines by disabling the PE bit
		I2C_PeripheralControl(I2C_handle->I2Cx, DISABLE);

		// Now we can configure the TIMNGR
		I2C_handle->I2Cx->TIMINGR  = (0x00300617);

		// We enable the PE
		I2C_PeripheralControl(I2C_handle->I2Cx, ENABLE);
	GPIO_Handle_t GPIO_I2C3_SDA;
	GPIO_I2C3_SDA.pGPIOx = GPIOC;
	GPIO_I2C3_SDA.GPIO_PIN_Config.GPIO_AltFunMode   			=  4;
	GPIO_I2C3_SDA.GPIO_PIN_Config.GPIO_PinNumber    			=  1;
	GPIO_I2C3_SDA.GPIO_PIN_Config.GPIO_PinMode      			=  GPIO_MODE_ALTFN;
	GPIO_I2C3_SDA.GPIO_PIN_Config.GPIO_PinPuPdControl 			=  GPIO_PIN_PU;
	GPIO_I2C3_SDA.GPIO_PIN_Config.GPIO_PinSpeed       			=  GPIO_SP_HIGH;
	GPIO_I2C3_SDA.GPIO_PIN_Config.GPIO_OPType		 			=  GPIO_OP_TYPE_OD;

	GPIO_Handle_t GPIO_I2C3_SCL;
	GPIO_I2C3_SCL.pGPIOx = GPIOC;
	GPIO_I2C3_SCL.GPIO_PIN_Config.GPIO_AltFunMode   			=  4;
	GPIO_I2C3_SCL.GPIO_PIN_Config.GPIO_PinNumber    			=  0;
	GPIO_I2C3_SCL.GPIO_PIN_Config.GPIO_PinMode      			=  GPIO_MODE_ALTFN;
	GPIO_I2C3_SCL.GPIO_PIN_Config.GPIO_PinPuPdControl 			=  GPIO_PIN_PU;
	GPIO_I2C3_SCL.GPIO_PIN_Config.GPIO_PinSpeed       			=  GPIO_SP_HIGH;
	GPIO_I2C3_SCL.GPIO_PIN_Config.GPIO_OPType		 			=  GPIO_OP_TYPE_OD;



	GPIO_Init(&GPIO_I2C3_SDA);
	GPIO_Init(&GPIO_I2C3_SCL);


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

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr){
		/*
		 *
		 clear CR2 before setting new values. If you call this function twice, old SADD and NBYTES get ORed on top:*/
		// clear CR2 first just saying
		pI2CHandle->I2Cx->CR2 = 0;

		// setting the 7 or 10 bit slave address in Master mode
		pI2CHandle->I2Cx->CR2  |= (SlaveAddr << 1); // considering 7-bit and don't care bits

		//Master Transferring request enable
		pI2CHandle->I2Cx->CR2  &= ~(1 << 10);

		// Writing Number of bits of the address
		pI2CHandle->I2Cx->CR2  &= ~(1 << 11); // considering 7-bit data address


		// setting NBYTES to tell cr2 no of bytes
		pI2CHandle->I2Cx->CR2  |= (Len  << 16);

		// Generating Start Condition
		pI2CHandle->I2Cx->CR2  |= (1 << 13);



		for (uint32_t i = 0; i< Len; i++){

			//Wait until TXIS is set by the hardware
			uint32_t timeout = 100000;
			while(!(pI2CHandle->I2Cx->ISR & (1 << 1))) {
			    if (--timeout == 0) return;  // bus hung — bail out
			}
			// Write to TXDR
			pI2CHandle->I2Cx->TXDR  = (pTxBuffer[i]);
		}

		// Wait while the transfer complete
		uint32_t timeout = 100000;
		while(!(pI2CHandle->I2Cx->ISR & (1 << 6))) {
		    if (--timeout == 0) {
		        pI2CHandle->I2Cx->CR2 |= (1 << 14);  // generate STOP before bailing
		        return;
		    }
		}

}


#ifndef INC_STM32L476XX_I2C_DRIVER_H_
#define INC_STM32L476XX_I2C_DRIVER_H_

#include "stm32l476xx.h"


typedef struct{
	uint32_t  I2C_SCLSpeed;
	uint8_t   I2C_DeviceAddress;

}I2C_Config_t;


typedef struct{
	I2C_RegDef_t *I2Cx;
	I2C_Config_t I2C_Config;
}I2C_Handle_t;


/*
 * @SCL_Speed
 */

#define SCLSpeed_SM 		100000
#define SCLSpeed_FM2K		200000
#define SCLSpeed_FM4K		400000


/* -----------------------------API Prototypes--------------------------------*/


/*----------------------------- peripheral clock setup-------------------------*/

void I2C_Periph_ClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/*------------------------------I2C initialization and de-initialization------*/

void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);   // Peripheral clock Reset in one go



/*------------------------------IRQ and ISR Handling-----------------------------*/

void I2C_IRQInterrupt_Config(uint8_t IRQNumber, uint8_t EnorDi);
void I2C_IRQPriority_Config(uint8_t IRQNumber,uint32_t IRQPriority);

/*------------------------------Other Useful API's------------------------------*/

void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);

uint8_t I2C_Get_FlagStatus(I2C_RegDef_t *pI2Cx, uint32_t Flag_Name);


void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv);

#endif /* INC_STM32L476XX_I2C_DRIVER_H_ */

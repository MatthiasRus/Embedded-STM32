/*
 * stm32l476xx_adc_driver.h
 *
 *  Created on: Mar 10, 2026
 *      Author: mat-ros
 */

#ifndef INC_STM32L476XX_ADC_DRIVER_H_
#define INC_STM32L476XX_ADC_DRIVER_H_

#include "stm32l476xx.h"
#define SINGLE_MODE              0
#define CONTINUOUS_MODE 		 1
typedef struct {
		uint8_t  ADC_Resolution;      // 12, 10, 8, or 6 bit
	    uint8_t  ADC_Channel;         // which channel (0-18)
	    uint8_t  ADC_SamplingTime;    // SMPx value (0-7)
	    uint8_t  ADC_mode;         // (Single or continuous Mode)
	    uint8_t  ADC_DataAlignment;  // Right or left edge alignment


}ADC_Config_t;


typedef struct {
	ADC_RegDef_t *ADCx;
	ADC_Config_t ADC_configuration;
}ADC_Handle_t;



void ADC_Init(ADC_Handle_t *PADCx_Handle);
void ADC_DeInit(ADC_RegDef_t *ADCx);
uint16_t ADC_ReadData(ADC_Handle_t *pADCx_Handle);


#endif /* INC_STM32L476XX_ADC_DRIVER_H_ */

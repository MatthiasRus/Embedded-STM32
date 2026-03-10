/*
 * ADC_InternalTemp_Reading.c
 *
 *  Created on: Mar 10, 2026
 *      Author: mat-ros
 */


#include "stm32l476xx.h"

#define  TS_CAL1  					(*(uint16_t*)0x1FFF75A8U)
#define  TS_CAL2 					(*(uint16_t*)0x1FFF75CAU)

int main(void){
	// Freeze WWDG during debug halt
	*((volatile uint32_t*)0xE0042008) |= (1 << 9);  // DBGMCU_APB1FZR1, freeze WWDG
	SysTick_Init();
	ADC_Handle_t ADC ;

	ADC.ADCx = ADC1;
	ADC.ADC_configuration.ADC_Channel = 17;
	ADC.ADC_configuration.ADC_Resolution = 12;
	ADC.ADC_configuration.ADC_mode = 0;
	ADC.ADC_configuration.ADC_SamplingTime = 7;


	ADC_Init(&ADC);

	// enable the temperature sensor
		ADC1_COMMON->CCR |= (1 << 23);
		for (volatile int i = 0; i < 80000; i++);  // temp sensor startup ~10us minimum

	/*
	 * temp = ((110 - 30) / (TS_CAL2 - TS_CAL1)) * (adc_value - TS_CAL1) + 30
	 */
	while (1){
		uint16_t cal1 = TS_CAL1, cal2 = TS_CAL2;
		uint16_t adcValue = ADC_ReadData(&ADC);

		uint16_t temperature = ((110 - 30) / (cal2 - cal1))
		                    * (adcValue - cal1) + 30;

	}


	return 0;
}

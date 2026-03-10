/*
 * stm32l476xx_adc_driver.c
 *
 *  Created on: Mar 10, 2026
 *      Author: mat-ros
 */


#include "stm32l476xx_adc_driver.h"


void ADC_Init(ADC_Handle_t *pADCHandle){
		// Step 1: Enable ADC peripheral clock

	    ADC1_PCLK_EN();

	    // CKMODE = 01: AHB clock / 1
	    ADC1_COMMON->CCR &= ~(3U << 16);
	    ADC1_COMMON->CCR |=  (1U << 16);
	    // Step 2: Exit deep power down, enable voltage regulator
	    pADCHandle->ADCx->CR   &= ~(1 << 29);
	    pADCHandle->ADCx->CR   |= (1 << 28);
	    // Step 3: Wait for regulator startup
	    for (volatile int i = 0; i < 1600000; i++);
	    // Step 4: Run calibration
	    //1st step check CALDIF  is it single or Diff mode
	    if (pADCHandle->ADC_configuration.ADC_mode  == SINGLE_MODE){
	    	pADCHandle->ADCx->CR  &= ~(1 << 30);
	    }else {
	    	pADCHandle->ADCx->CR  |= (1  << 30);
	    }
	    // 2nd step Set the ADCAL
	    pADCHandle->ADCx->CR |= (1 << 31);

	    // wait with plain counter
	    volatile uint32_t timeout = 1000000;
	    while((pADCHandle->ADCx->CR & (1 << 31)) && timeout--);
	    if(timeout == 0) {
	        // trap here so we can see it in debugger
	        while(1);
	    }

	    // Step 5: Configure resolution
	    uint8_t resolution = pADCHandle->ADC_configuration.ADC_Resolution;
	    if (resolution == 12){
	    	pADCHandle->ADCx->CFGR &= ~(1 << 3);
	        pADCHandle->ADCx->CFGR &= ~(1 << 4);
	    }else if (resolution == 10){
	    	pADCHandle->ADCx->CFGR |= (1 << 3);
	    	pADCHandle->ADCx->CFGR &= ~(1 << 4);
	    }else if (resolution == 8){
	    	pADCHandle->ADCx->CFGR |= (1 << 4);
	    	pADCHandle->ADCx->CFGR &= ~(1 << 3);
	    }else {
	    	pADCHandle->ADCx->CFGR |= (1 << 3);
	    	pADCHandle->ADCx->CFGR |= (1 << 4);
	    }


	    // Step 6: Configure channel  and sampling time
	    uint8_t channel = pADCHandle->ADC_configuration.ADC_Channel;
	    uint8_t samplingTime = pADCHandle->ADC_configuration.ADC_SamplingTime;

	    if (channel <=9){
	    	pADCHandle->ADCx->SMPR1 |= (samplingTime << (3 * channel));
	    }else {
	    	pADCHandle->ADCx->SMPR2 |= (samplingTime << (3 * (channel - 10)));

	    }

	    pADCHandle->ADCx->SQR1 = (channel << 6);

	    // Step 7: Enable ADC, wait ADRDY
	    // 1st step to enable ADC which is ADEN all ADC_CR regs must be 0 WOW
	    pADCHandle->ADCx->CR &= ~(1 << 1);   // clear the ADDIS
	    pADCHandle->ADCx->CR |= (1 << 0);
	    volatile uint32_t timeout2 = 1000000;
	    while(!(pADCHandle->ADCx->ISR & (1 << 0)) && timeout2--);
	    if(timeout2 == 0) while(1);  // breakpoint here
};


void ADC_DeInit(ADC_RegDef_t *pADCx){

};


uint16_t ADC_ReadData(ADC_Handle_t *pADC_Handle){
	    // 1. Start conversion — which bit?
	    pADC_Handle->ADCx->CR |= (1 << 2);
	    // 2. Wait for EOC — which bit in ISR?
	    while (!(pADC_Handle->ADCx->ISR & (1 << 3))) Delay_ms(1);
	    // 3. Read and return DR
	    return (uint16_t)(pADC_Handle->ADCx->DR & 0x0FFF);
};

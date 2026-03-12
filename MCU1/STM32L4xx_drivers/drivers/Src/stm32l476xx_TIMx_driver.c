/*
 * stm32l476xx_TIMx_driver.c
 *
 *  Created on: Mar 11, 2026
 *      Author: mat-ros
 */


#include "stm32l476xx.h"
#include <stdint.h>


void PWM_Init(TIM_Handle_t *pTIMx_Handle){
		TIM2_PCLK_EN();

		// PA0  AF7 Serve as TIM2_CH1 from the pin config table in the DS
		// ALternate GPIO COnfiguration
		GPIO_Handle_t GPIO_TIM;
		GPIO_TIM.pGPIOx = GPIOA;
		GPIO_TIM.GPIO_PIN_Config.GPIO_AltFunMode   		=  1;
		GPIO_TIM.GPIO_PIN_Config.GPIO_PinNumber    		=  0;
		GPIO_TIM.GPIO_PIN_Config.GPIO_PinMode      		=  GPIO_MODE_ALTFN;
		GPIO_TIM.GPIO_PIN_Config.GPIO_PinPuPdControl 	=  GPIO_NO_PUPD;
		GPIO_TIM.GPIO_PIN_Config.GPIO_PinSpeed       	=  GPIO_SP_HIGH;
		GPIO_TIM.GPIO_PIN_Config.GPIO_OPType		 	=  GPIO_OP_TYPE_PP;

		GPIO_Init(&GPIO_TIM);
		// Setting PSC to 15
		pTIMx_Handle->pTIMx->PSC = (0xF);

		//Enable Auto reload pre-load enable ARPE
		pTIMx_Handle->pTIMx->CR1 |= (1 << 7);

		// setting ARR to 19999
		pTIMx_Handle->pTIMx->ARR = (0x4E1F);

		// Enable Pre-scaler
		pTIMx_Handle->pTIMx->CCMR1 |= (1 << 3);
		//setting CCR1  to 1500
		pTIMx_Handle->pTIMx->CCR1 = (0x5DC);

		// Setting CMR1 to PWM mode 1 channel 1
		pTIMx_Handle->pTIMx->CCMR1 |= (0x6 << 4);

		// Enable Channel 1 output
		pTIMx_Handle->pTIMx->CCER  |= (1 << 0);

		// Enable Counter
		pTIMx_Handle->pTIMx->CR1   |= (1 << 0);
}

/*
 * stm32l476xx_TIMx_driver.c
 *
 *  Created on: Mar 11, 2026
 *      Author: mat-ros
 */


#include "stm32l476xx.h"
#include <stdint.h>


void PWM_TIM2_Init(TIM_Handle_t *pTIMx_Handle){
		TIM2_PCLK_EN();

		// PA0  AF7 Serve as TIM2_CH1 from the pin config table in the DS
		// ALternate GPIO COnfiguration for CH1 for TIM2
		GPIO_Handle_t GPIO_TIM_CH1;
		GPIO_TIM_CH1.pGPIOx = GPIOA;
		GPIO_TIM_CH1.GPIO_PIN_Config.GPIO_AltFunMode   			=  1;
		GPIO_TIM_CH1.GPIO_PIN_Config.GPIO_PinNumber    			=  0;
		GPIO_TIM_CH1.GPIO_PIN_Config.GPIO_PinMode      			=  GPIO_MODE_ALTFN;
		GPIO_TIM_CH1.GPIO_PIN_Config.GPIO_PinPuPdControl 		=  GPIO_NO_PUPD;
		GPIO_TIM_CH1.GPIO_PIN_Config.GPIO_PinSpeed       		=  GPIO_SP_HIGH;
		GPIO_TIM_CH1.GPIO_PIN_Config.GPIO_OPType		 		=  GPIO_OP_TYPE_PP;

		GPIO_Handle_t GPIO_TIM_CH2;
		GPIO_TIM_CH2.pGPIOx = GPIOA;
		GPIO_TIM_CH2.GPIO_PIN_Config.GPIO_AltFunMode   			=  1;
		GPIO_TIM_CH2.GPIO_PIN_Config.GPIO_PinNumber    			=  1;
		GPIO_TIM_CH2.GPIO_PIN_Config.GPIO_PinMode      			=  GPIO_MODE_ALTFN;
		GPIO_TIM_CH2.GPIO_PIN_Config.GPIO_PinPuPdControl 		=  GPIO_NO_PUPD;
		GPIO_TIM_CH2.GPIO_PIN_Config.GPIO_PinSpeed       		=  GPIO_SP_HIGH;
		GPIO_TIM_CH2.GPIO_PIN_Config.GPIO_OPType		 		=  GPIO_OP_TYPE_PP;



		GPIO_Init(&GPIO_TIM_CH1);
		GPIO_Init(&GPIO_TIM_CH2);

		// Setting PSC to 15
		pTIMx_Handle->pTIMx->PSC = (0xF);

		//Enable Auto reload pre-load enable ARPE
		pTIMx_Handle->pTIMx->CR1 |= (1 << 7);

		// setting ARR to 19999
		pTIMx_Handle->pTIMx->ARR = (0x4E1F);

		// Enable Pre-scaler
		pTIMx_Handle->pTIMx->CCMR1 |= (1 << 3);  // CH1
		pTIMx_Handle->pTIMx->CCMR1 |= (1 << 11); // CH2

		//setting CCR1  CCR2  to 1500
		pTIMx_Handle->pTIMx->CCR1 = (0x5DC);
		pTIMx_Handle->pTIMx->CCR2 = (0x5DC);

		// Setting CMR1 to PWM mode 1 channel 1
		pTIMx_Handle->pTIMx->CCMR1 |= (0x6 << 4);

		//setting CCMR1 to PWM mode 1 channel 2
		pTIMx_Handle->pTIMx->CCMR1 |= (0x6 << 12);

		// Enable Channel 1 output
		pTIMx_Handle->pTIMx->CCER  |= (1 << 0);

		// Enable Channel 2 output
		pTIMx_Handle->pTIMx->CCER  |= (1 << 4);

		// Enable Counter
		pTIMx_Handle->pTIMx->CR1   |= (1 << 0);
}


void PWM_TIM3_Init(TIM_Handle_t *pTIMx_Handle){
		TIM3_PCLK_EN();


		// TIM3 CH1  CH2  CH3 CH4
		GPIO_Handle_t GPIO_TIM3_CH1;
		GPIO_TIM3_CH1.pGPIOx = GPIOA;
		GPIO_TIM3_CH1.GPIO_PIN_Config.GPIO_AltFunMode   		=  2;
		GPIO_TIM3_CH1.GPIO_PIN_Config.GPIO_PinNumber    		=  6;
		GPIO_TIM3_CH1.GPIO_PIN_Config.GPIO_PinMode      		=  GPIO_MODE_ALTFN;
		GPIO_TIM3_CH1.GPIO_PIN_Config.GPIO_PinPuPdControl 		=  GPIO_NO_PUPD;
		GPIO_TIM3_CH1.GPIO_PIN_Config.GPIO_PinSpeed       		=  GPIO_SP_HIGH;
		GPIO_TIM3_CH1.GPIO_PIN_Config.GPIO_OPType		 		=  GPIO_OP_TYPE_PP;

		GPIO_Handle_t GPIO_TIM3_CH2;
		GPIO_TIM3_CH2.pGPIOx = GPIOA;
		GPIO_TIM3_CH2.GPIO_PIN_Config.GPIO_AltFunMode   			=  2;
		GPIO_TIM3_CH2.GPIO_PIN_Config.GPIO_PinNumber    			=  7;
		GPIO_TIM3_CH2.GPIO_PIN_Config.GPIO_PinMode      			=  GPIO_MODE_ALTFN;
		GPIO_TIM3_CH2.GPIO_PIN_Config.GPIO_PinPuPdControl 			=  GPIO_NO_PUPD;
		GPIO_TIM3_CH2.GPIO_PIN_Config.GPIO_PinSpeed       			=  GPIO_SP_HIGH;
		GPIO_TIM3_CH2.GPIO_PIN_Config.GPIO_OPType		 			=  GPIO_OP_TYPE_PP;

		GPIO_Handle_t GPIO_TIM3_CH3;
		GPIO_TIM3_CH3.pGPIOx = GPIOB;
		GPIO_TIM3_CH3.GPIO_PIN_Config.GPIO_AltFunMode   			=  2;
		GPIO_TIM3_CH3.GPIO_PIN_Config.GPIO_PinNumber    			=  0;
		GPIO_TIM3_CH3.GPIO_PIN_Config.GPIO_PinMode      			=  GPIO_MODE_ALTFN;
		GPIO_TIM3_CH3.GPIO_PIN_Config.GPIO_PinPuPdControl 			=  GPIO_NO_PUPD;
		GPIO_TIM3_CH3.GPIO_PIN_Config.GPIO_PinSpeed       			=  GPIO_SP_HIGH;
		GPIO_TIM3_CH3.GPIO_PIN_Config.GPIO_OPType		 			=  GPIO_OP_TYPE_PP;

		GPIO_Handle_t GPIO_TIM3_CH4;
		GPIO_TIM3_CH4.pGPIOx = GPIOB;
		GPIO_TIM3_CH4.GPIO_PIN_Config.GPIO_AltFunMode   			=  2;
		GPIO_TIM3_CH4.GPIO_PIN_Config.GPIO_PinNumber    			=  1;
		GPIO_TIM3_CH4.GPIO_PIN_Config.GPIO_PinMode      			=  GPIO_MODE_ALTFN;
		GPIO_TIM3_CH4.GPIO_PIN_Config.GPIO_PinPuPdControl 			=  GPIO_NO_PUPD;
		GPIO_TIM3_CH4.GPIO_PIN_Config.GPIO_PinSpeed       			=  GPIO_SP_HIGH;
		GPIO_TIM3_CH4.GPIO_PIN_Config.GPIO_OPType		 			=  GPIO_OP_TYPE_PP;


		GPIO_Init(&GPIO_TIM3_CH1);
		GPIO_Init(&GPIO_TIM3_CH2);
		GPIO_Init(&GPIO_TIM3_CH3);
		GPIO_Init(&GPIO_TIM3_CH4);

		// Setting PSC to 15
				pTIMx_Handle->pTIMx->PSC = (0xF);

				//Enable Auto reload pre-load enable ARPE
				pTIMx_Handle->pTIMx->CR1 |= (1 << 7);

				// setting ARR to 19999
				pTIMx_Handle->pTIMx->ARR = (0x4E1F);

				// Enable Pre-scaler
				pTIMx_Handle->pTIMx->CCMR1 |= (1 << 3);  // CH1
				pTIMx_Handle->pTIMx->CCMR1 |= (1 << 11); // CH2
				pTIMx_Handle->pTIMx->CCMR2 |= (1 << 3);  // CH3
				pTIMx_Handle->pTIMx->CCMR2 |= (1 << 11); // CH4

				//setting CCR1  CCR2  to 1500
				pTIMx_Handle->pTIMx->CCR1 = (0x5DC);  // CH1
				pTIMx_Handle->pTIMx->CCR2 = (0x5DC);  // CH2
				pTIMx_Handle->pTIMx->CCR3 = (0x5DC);  // CH3
				pTIMx_Handle->pTIMx->CCR4 = (0x5DC);  // CH4

				// Setting CCMR1 to PWM mode 1 channel 1 and channel 2
				pTIMx_Handle->pTIMx->CCMR1 |= (0x6 << 4);
				pTIMx_Handle->pTIMx->CCMR1 |= (0x6 << 12);

				// Setting CCMR2 to PWM mode 1 channel 3 and channel 4
				pTIMx_Handle->pTIMx->CCMR2 |= (0x6 << 4);
				pTIMx_Handle->pTIMx->CCMR2 |= (0x6 << 12);

				// Enable Channel 1 output
				pTIMx_Handle->pTIMx->CCER  |= (1 << 0);

				// Enable Channel 2 output
				pTIMx_Handle->pTIMx->CCER  |= (1 << 4);

				// Enable Channel 3 output
				pTIMx_Handle->pTIMx->CCER  |= (1 << 8);

				// Enable Channel 2 output
				pTIMx_Handle->pTIMx->CCER  |= (1 << 12);

				// Enable Counter
				pTIMx_Handle->pTIMx->CR1   |= (1 << 0);
}

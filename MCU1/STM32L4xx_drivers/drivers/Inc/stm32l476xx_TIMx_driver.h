/*
 * stm32l477xx_TIMx_driver.h
 *
 *  Created on: Mar 11, 2026
 *      Author: mat-ros
 */

#ifndef INC_STM32L476XX_TIMX_DRIVER_H_
#define INC_STM32L476XX_TIMX_DRIVER_H_

#include "stm32l476xx.h"

typedef struct{

}TIM_Config_T;

typedef struct{
	TIM_RegDef_t* pTIMx;
	TIM_Config_T  TIM_Configuration;
}TIM_Handle_t;


void PWM_Init(TIM_Handle_t *pTIM_Handle);
void PWM_DeInit(TIM_RegDef_t* pTIMx);

#endif /* INC_STM32L476XX_TIMX_DRIVER_H_ */

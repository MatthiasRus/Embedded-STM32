/*
 * Servo_Drive_pca9685.c
 *
 *  Created on: Mar 13, 2026
 *      Author: mat-ros
 */


#include "stm32l476xx.h"
#include "pca9685_driver.h"
#include <stdint.h>


int main(void){
					// clock setup
					RCC->CR |= (1 << 8);
					while(!(RCC->CR & (1 << 10)));
					RCC->CFGR &= ~(3 << 0);
					RCC->CFGR |= (1 << 0);
					while((RCC->CFGR & (3 << 2)) != (1 << 2));
	                SysTick_Init();

	                I2C_Handle_t I2C_Master;
	                I2C_Master.I2Cx = I2C3;
	                I2C_Master.I2C_Config.I2C_SCLSpeed = SCLSpeed_FM4K;

	                // Bus recovery — release SCL before I2C init
	                GPIOC_PCLK_EN();
	                GPIOC->MODER &= ~(3 << 0);
	                GPIOC->MODER |= (1 << 0);   // PC0 output
	                GPIOC->ODR |= (1 << 0);     // drive HIGH
	                for(volatile int i = 0; i < 10000; i++);

	                // Reset I2C3
	                I2C3_REG_RESET();
	                for(volatile int i = 0; i < 1000; i++);

	                // Normal init
	                I2C_Init(&I2C_Master);
	                PCA9685_Init(&I2C_Master);

	                while(1){
	                	PCA9685_SetPulse(&I2C_Master, 1,2000);
	                	Delay_ms(500);
	                	PCA9685_SetPulse(&I2C_Master, 1, 1000);
	                	Delay_ms(500);
	                	PCA9685_SetPulse(&I2C_Master, 1,1000);
	                	Delay_ms(500);
	                }

}

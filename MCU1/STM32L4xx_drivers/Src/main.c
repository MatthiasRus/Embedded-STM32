/*
 * main.c
 *
 *  Created on: Aug 31, 2025
 *      Author: matth_ik1hap6
 */
#include "stm32l476xx.h"

int main(void){
	return 0;
}

void EXTI0_IRQHandler(void){
	GPIO_IRQHandler(0);
}

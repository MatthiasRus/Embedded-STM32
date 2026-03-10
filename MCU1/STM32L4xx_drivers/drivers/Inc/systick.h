/*
 * systick.h
 *
 *  Created on: Mar 10, 2026
 *      Author: mat-ros
 */

#ifndef INC_SYSTICK_H_
#define INC_SYSTICK_H_

void SysTick_Init(void);
void SysTick_Handler(void);
uint32_t GetTick(void);
void Delay_ms(uint32_t ms);


#endif /* INC_SYSTICK_H_ */

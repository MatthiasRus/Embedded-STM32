/*
 * pca9685_driver.h
 *
 *  Created on: Mar 13, 2026
 *      Author: mat-ros
 */

#ifndef INC_PCA9685_DRIVER_H_
#define INC_PCA9685_DRIVER_H_
#include <stdint.h>
#include "stm32l476xx.h"
// Macros for PCA9685


#define PCA9685_ADDR    0x40
#define MODE1_REG       0x00
#define PRE_SCALE_REG   0xFE

#define   LED_ON_L			0x00   // (low byte of ON time)
#define   LED_ON_H			0x01   // (high byte)
#define   LED_OFF_L			0x02  // (low byte of OFF time)
#define   LED_OFF_H			0x03  // (high byte)

//N = 0x06 + (4 * N)
#define   GET_CHANNEL(N)	(0x06 + (4 * N))


void PCA9685_Init(I2C_Handle_t *hi2c);
void PCA9685_SetPulse(I2C_Handle_t *hi2c, uint8_t channel, uint16_t pulse_us);


#endif /* INC_PCA9685_DRIVER_H_ */

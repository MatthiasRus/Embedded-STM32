/*
 * pca9685_driver.c
 *
 *  Created on: Mar 13, 2026
 *      Author: mat-ros
 */

#include "pca9685_driver.h"
#include "stm32l476xx.h"

void PCA9685_Init(I2C_Handle_t *hi2c){
    uint8_t data[2];

    // set sleep mode bit 4
    data[0] = MODE1_REG;
    data[1] = 0x10;
    I2C_MasterSendData(hi2c, data, 2, PCA9685_ADDR);

    // set prescale for 50Hz
    data[0] = PRE_SCALE_REG; data[1] = 0x79;
    I2C_MasterSendData(hi2c, data, 2, PCA9685_ADDR);

    // wake up
    data[0] = MODE1_REG; data[1] = 0x00;
    I2C_MasterSendData(hi2c, data, 2, PCA9685_ADDR);

    // wait 500us
    for(volatile int i = 0; i < 8000; i++);

    // enable auto increment
    data[0] = MODE1_REG; data[1] = 0xA0;
    I2C_MasterSendData(hi2c, data, 2, PCA9685_ADDR);
}


void PCA9685_SetPulse(I2C_Handle_t *hi2c, uint8_t channel, uint16_t pulse_us){
	uint16_t ticks = pulse_us * 4096 / 20000;  // $50 Hz
	uint8_t data[5];
	data[0] = GET_CHANNEL(channel);  // register address
	data[1] = 0x00;  // ON_L
	data[2] = 0x00;  // ON_H
	data[3] = ticks & 0xFF;   // OFF_L
	data[4] = (ticks >> 8) & 0x0F;  // OFF_H
	I2C_MasterSendData(hi2c, data, 5, PCA9685_ADDR);
}

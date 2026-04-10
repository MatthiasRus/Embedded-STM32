/*
 * servoFreeRTOS_RX.c
 *
 *  Created on: Apr 10, 2026
 *      Author: mat-ros
 */
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"


#include "stm32l476xx.h"
#include "stm32l476xx_usart_driver.h"

static QueueHandle_t  servo_queue = NULL;
static SemaphoreHandle_t dma_semp = NULL;

static GPIO_Handle_t GpioLed;
static USART_Handle_t Usart2;
static Servo_command servo_cmd = {0};
static DMAx_Handle_t DMA_RX;
static char buffer[50];
volatile uint16_t bufferSize = 50;
volatile int data_received = 0;
volatile uint8_t  channel = USART2_RX_DMA_CHANNEL_NO;

void SystemClock_Config(void){
	// Switch to HSI16 (16MHz) — simple and fast
	RCC->CR |= (1 << 8);              // HSION
	while(!(RCC->CR & (1 << 10)));    // wait HSIRDY
	RCC->CFGR &= ~(3 << 0);          // clear SW
	RCC->CFGR |= (1 << 0);           // SW = HSI16
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    (void)xTask;
    (void)pcTaskName;

    while(1);
}

void vApplicationMallocFailedHook(void) {
    while(1);
}

void servo_parse_task(void* pvParameter){
	 while(1){
		 xSemaphoreTake(dma_semp, portMAX_DELAY);
		 data_received = 1;
		 xQueueSendToBack(servo_queue, &servo_cmd, 0);
	 }
}

void servo_update_task(void* pvParameter){
	USART_Handle_t* usart = (USART_Handle_t*) pvParameter;
	Servo_command local_cmd = {0};
	BaseType_t queueReceiver;
	char local_buffer[50];

	while(1){
		queueReceiver = xQueueReceive(servo_queue, &local_cmd, portMAX_DELAY);
		snprintf(local_buffer, sizeof(local_buffer), "ch:%u pulse:%u\r\n", local_cmd.channel, local_cmd.pulse_us);
		USART_SendString(usart, local_buffer);
	}

}


void LED_confirm_task(void* pvParameter) {
    while(1) {
        if (data_received) {
            data_received = 0;
            // flash fast 3 times
            for (int i = 0; i < 3; i++) {
                GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_5, 1);
                vTaskDelay(pdMS_TO_TICKS(50));
                GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_5, 0);
                vTaskDelay(pdMS_TO_TICKS(50));
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


int main(void){

	SystemClock_Config();

    GpioLed.pGPIOx = GPIOA;			// LED


    // GPIO LED Definition
    GpioLed.GPIO_PIN_Config.GPIO_PinNumber = GPIO_PIN_5;
    GpioLed.GPIO_PIN_Config.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PIN_Config.GPIO_PinSpeed = GPIO_SP_HIGH;
    GpioLed.GPIO_PIN_Config.GPIO_OPType = GPIO_OP_TYPE_PP;
    GpioLed.GPIO_PIN_Config.GPIO_PinPuPdControl = GPIO_NO_PUPD;


    Usart2.USARTx = USART2;

    Usart2.USARTx_Config.USART_BaudRate = USART_DIV_115200;
    Usart2.USARTx_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
    Usart2.USARTx_Config.USART_Mode = USART_MODE_TX_RX;
    Usart2.USARTx_Config.USART_OverSampling = OVERSAMPLING_16;
    Usart2.USARTx_Config.USART_Parity = NO_PARITY;
    Usart2.USARTx_Config.USART_StopBit = ONE_STOP_BIT;
    Usart2.USARTx_Config.USART_WordLength = EIGHT_BIT_WL;


    GPIO_Init(&GpioLed);

    USART_Init(&Usart2);

	DMAx_Intrrupt_Config(IRQ_NO_DMA1_CH6, ENABLE);
	DMAx_Priority_Config(IRQ_NO_DMA1_CH6, NVIC_IRQ_PRI15);

	Usart2.USARTx->CR1 |= (1 << USART_IDLE_BIT);

	USART2_DMA_RX_Enable(&Usart2, &DMA_RX, (void*)buffer, bufferSize);

	// Clear IDLE flag before enabling interrupt, in case it's already set
	Usart2.USARTx->ICR |= (1 << USART_IDLE_CLEAR_BIT);

	USART2_Interrupt_Config(IRQ_NO_USART2, ENABLE);   // now safe
	USART2_Priority_Config(IRQ_NO_USART2, NVIC_IRQ_PRI15);

    BaseType_t queue_depth = 10;
    dma_semp = xSemaphoreCreateBinary();
    servo_queue = xQueueCreate(queue_depth, sizeof(servo_cmd));


    xTaskCreate(servo_parse_task, "Servo_cmd_parse", 256, (void*)&Usart2, 3, NULL);
    xTaskCreate(servo_update_task, "Servo_Update", 256, (void*)&Usart2, 2, NULL);
    xTaskCreate(LED_confirm_task, "Led_confirmation", 128, NULL, 1, NULL);

    vTaskStartScheduler();
    while(1);
    return 0;
}

void USART2_IRQHandler(void){
    if(Usart2.USARTx->ISR & (1 << USART_IDLE_BIT)){
        Usart2.USARTx->ICR |= (1 << USART_IDLE_CLEAR_BIT);

        DMAx_Stop(&DMA_RX);


        uint32_t receivedLen = bufferSize - DMA_RX.DMAx->CH[channel - 1].CNDTR;

        if (receivedLen > 0 && buffer[0] == '$'){
            buffer[receivedLen] = '\0';
            USART_ParseData(buffer, &servo_cmd);
             // give semaphore — wake parse task
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			xSemaphoreGiveFromISR(dma_semp, &xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }

        DMAx_Start(&DMA_RX,bufferSize);


    }
}

void DMA1_Channel6_IRQHandler(void){

}

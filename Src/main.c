#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "stm32l476xx.h"
#include "stm32l476xx_usart_driver.h"

#define LOW 			0
#define BTN_PRESSED 	LOW

static GPIO_Handle_t GpioLed;
static USART_Handle_t Usart2;

static Servo_command servo_cmd = {0};
static QueueHandle_t servo_queue = NULL;


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
    // stack overflow detected — halt and catch fire
    // in production: log the task name, trigger safe shutdown
    while(1);
}

void vApplicationMallocFailedHook(void) {
    // heap exhausted — xTaskCreate or queue create failed
    while(1);
}

void USART_HB(void* pvParameter){
//	USART_Handle_t* usart = (USART_Handle_t*) pvParameter;
	TickType_t xLastWakeTick = xTaskGetTickCount();
	BaseType_t xQueueSender;
	volatile size_t i = 1000;
	volatile size_t ch = 0;
	while(1){
//		USART_SendString(usart, "ALIVE\r\n");
		while(i <= 2000){
			servo_cmd.channel= ch % 6 + 1;
			servo_cmd.pulse_us = i;

			xQueueSender = xQueueSendToBack(servo_queue, &servo_cmd, 0);
			xTaskDelayUntil(&xLastWakeTick, pdMS_TO_TICKS(500));
			i += 100;
			ch += 1;
		}
		ch = 1;
		i = 1000;
	}
}

void LED_task(void* pvParameter){
	GPIO_Handle_t* gpioLED = (GPIO_Handle_t*) pvParameter;

	while(1){

		GPIO_ToggleOutputPin(gpioLED->pGPIOx, gpioLED->GPIO_PIN_Config.GPIO_PinNumber);
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

void servo_update_task(void* pvParameter){
	USART_Handle_t* usart  = (USART_Handle_t*) pvParameter;
	Servo_command local_servo_cmd  = {0};
	BaseType_t xQueueReceiver;
	char data[50];

	while(1){
				xQueueReceiver = xQueueReceive(servo_queue, &local_servo_cmd, portMAX_DELAY);
				snprintf(data, sizeof(data), "ch:%u Pulse:%u\r\n", local_servo_cmd.channel, local_servo_cmd.pulse_us);
				USART_SendString(usart, data);
	}
}
int main(void)
{

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
    Usart2.USARTx_Config.USART_Mode = USART_MODE_TX_ONLY;
    Usart2.USARTx_Config.USART_OverSampling = OVERSAMPLING_16;
    Usart2.USARTx_Config.USART_Parity = NO_PARITY;
    Usart2.USARTx_Config.USART_StopBit = ONE_STOP_BIT;
    Usart2.USARTx_Config.USART_WordLength = EIGHT_BIT_WL;


    GPIO_Init(&GpioLed);

    USART_Init(&Usart2);

    BaseType_t queue_depth = 10;
    servo_queue = xQueueCreate(queue_depth, sizeof(servo_cmd));

    if (servo_queue == NULL){
    	while(1);
    }

    xTaskCreate(USART_HB, "Heart_Beat", 256, (void*)&Usart2, 2, NULL);

    xTaskCreate(servo_update_task, "Servo_update", 256, (void*)&Usart2, 3, NULL);
    xTaskCreate(LED_task, "LED", 128, (void*)&GpioLed, 1, NULL);

    vTaskStartScheduler();

    while(1);
	return 0;
}

#include "stm32l476xx.h"


#define LOW 			0
#define BTN_PRESSED 	LOW


void delay(void){
	for (int i = 0; i < 500000/2; i++);
}
int main(void)
{
    GPIO_Handle_t GpioLed,GpioButton;

    GpioButton.pGPIOx = GPIOB;		// Button
    GpioLed.pGPIOx = GPIOA;			// LED

    // GPIO LED Definition
    GpioLed.GPIO_PIN_Config.GPIO_PinNumber = GPIO_PIN_8;
    GpioLed.GPIO_PIN_Config.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PIN_Config.GPIO_PinSpeed = GPIO_SP_HIGH;
    GpioLed.GPIO_PIN_Config.GPIO_OPType = GPIO_OP_TYPE_PP;
    GpioLed.GPIO_PIN_Config.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    // GPIO Button Definition
    GpioButton.GPIO_PIN_Config.GPIO_PinNumber = GPIO_PIN_12;
    GpioButton.GPIO_PIN_Config.GPIO_PinMode = GPIO_MODE_IN;
    GpioButton.GPIO_PIN_Config.GPIO_PinSpeed = GPIO_SP_HIGH;
    GpioButton.GPIO_PIN_Config.GPIO_PinPuPdControl = GPIO_PIN_PU;   // pull up is available on the board itself

    // Clock Enable Disable Control
    GPIO_Periph_ClockControl(GPIOA, ENABLE);
    GPIO_Periph_ClockControl(GPIOB, ENABLE);


    GPIO_Init(&GpioLed);
    GPIO_Init(&GpioButton);

    while(1){
    	if (GPIO_ReadFromInputPin(GPIOB, GPIO_PIN_12)  == BTN_PRESSED){
    		delay();			// Helped us to avoid button de-bouncing
    		GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_8);
    	}
    }
	return 0;
}

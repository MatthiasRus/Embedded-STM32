################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/Src/pca9685_driver.c \
../drivers/Src/stm32l476xx_TIMx_driver.c \
../drivers/Src/stm32l476xx_adc_driver.c \
../drivers/Src/stm32l476xx_gpio_driver.c \
../drivers/Src/stm32l476xx_i2c.c \
../drivers/Src/stm32l476xx_usart_driver.c \
../drivers/Src/systick.c 

OBJS += \
./drivers/Src/pca9685_driver.o \
./drivers/Src/stm32l476xx_TIMx_driver.o \
./drivers/Src/stm32l476xx_adc_driver.o \
./drivers/Src/stm32l476xx_gpio_driver.o \
./drivers/Src/stm32l476xx_i2c.o \
./drivers/Src/stm32l476xx_usart_driver.o \
./drivers/Src/systick.o 

C_DEPS += \
./drivers/Src/pca9685_driver.d \
./drivers/Src/stm32l476xx_TIMx_driver.d \
./drivers/Src/stm32l476xx_adc_driver.d \
./drivers/Src/stm32l476xx_gpio_driver.d \
./drivers/Src/stm32l476xx_i2c.d \
./drivers/Src/stm32l476xx_usart_driver.d \
./drivers/Src/systick.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/Src/%.o drivers/Src/%.su drivers/Src/%.cyclo: ../drivers/Src/%.c drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32L4 -DSTM32 -DNUCLEO_L476RG -DSTM32L476RGTx -c -I../Inc -I"/home/mat-ros/Desktop/Embedded/STM32L4xx_drivers/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-drivers-2f-Src

clean-drivers-2f-Src:
	-$(RM) ./drivers/Src/pca9685_driver.cyclo ./drivers/Src/pca9685_driver.d ./drivers/Src/pca9685_driver.o ./drivers/Src/pca9685_driver.su ./drivers/Src/stm32l476xx_TIMx_driver.cyclo ./drivers/Src/stm32l476xx_TIMx_driver.d ./drivers/Src/stm32l476xx_TIMx_driver.o ./drivers/Src/stm32l476xx_TIMx_driver.su ./drivers/Src/stm32l476xx_adc_driver.cyclo ./drivers/Src/stm32l476xx_adc_driver.d ./drivers/Src/stm32l476xx_adc_driver.o ./drivers/Src/stm32l476xx_adc_driver.su ./drivers/Src/stm32l476xx_gpio_driver.cyclo ./drivers/Src/stm32l476xx_gpio_driver.d ./drivers/Src/stm32l476xx_gpio_driver.o ./drivers/Src/stm32l476xx_gpio_driver.su ./drivers/Src/stm32l476xx_i2c.cyclo ./drivers/Src/stm32l476xx_i2c.d ./drivers/Src/stm32l476xx_i2c.o ./drivers/Src/stm32l476xx_i2c.su ./drivers/Src/stm32l476xx_usart_driver.cyclo ./drivers/Src/stm32l476xx_usart_driver.d ./drivers/Src/stm32l476xx_usart_driver.o ./drivers/Src/stm32l476xx_usart_driver.su ./drivers/Src/systick.cyclo ./drivers/Src/systick.d ./drivers/Src/systick.o ./drivers/Src/systick.su

.PHONY: clean-drivers-2f-Src


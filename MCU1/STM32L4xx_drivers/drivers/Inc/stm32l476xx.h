/*
 * stm32l476xx.h
 *
 *  Created on: Aug 27, 2025
 *      Author: matth_ik1hap6
 */
#include <stdint.h>

#ifndef INC_STM32L476XX_H_
#define INC_STM32L476XX_H_
#define __vo    								volatile


/* ==================================Processor (M4) Specific Headers=====================================>*/
#define NVIC_ISER0								((__vo	uint32_t*)0xE000E100)
#define NVIC_ISER1								((__vo	uint32_t*)0xE000E104)
#define NVIC_ISER2								((__vo	uint32_t*)0xE000E108)
#define NVIC_ISER3								((__vo	uint32_t*)0xE000E10C)

#define NVIC_ICER0								((__vo	uint32_t*)0XE000E180)
#define NVIC_ICER1								((__vo	uint32_t*)0XE000E184)
#define NVIC_ICER2								((__vo	uint32_t*)0XE000E188)
#define NVIC_ICER4								((__vo	uint32_t*)0XE000E18C)


#define NVIC_IPR_BASE_ADDR						((__vo	uint32_t*)0xE000E400)

#define NO_PR_BITS_IMPLEMENTED					4				// ST case 4 TI case 3

/*=====================================================================================================*/

#define FLASH_BASE_ADDR              			0x08000000U		// Main memory address
#define SRAM1_BASE_ADDR				 			0x20000000U		// SRAM1 Embedded Memory
#define SRAM2_BASE_ADDR							0x20018000U		// SRAM2 = SRAM1   +   96kb * 1024
#define ROM_BASE_ADDR                			0x1FFF0000U		// System Memory address
#define SRAM						 			SRAM1_BASE_ADDR

#define PERIPH_BASE 							0x40000000U
#define AHB1_PERIPH_BASE_ADDR					0x40020000U
#define AHB2_PERIPH_BASE_ADDR					0x48000000U
#define APB1_PERIPH_BASE_ADDR					PERIPH_BASE
#define APB2_PERIPH_BASE_ADDR					0x40010000U



/* GPIO PERIPHERAL ADDRESSES*/

#define GPIOA_BASE_ADDR 						(AHB2_PERIPH_BASE_ADDR + 0x0000)
#define GPIOB_BASE_ADDR 						(AHB2_PERIPH_BASE_ADDR + 0x0400)
#define GPIOC_BASE_ADDR 						(AHB2_PERIPH_BASE_ADDR + 0x0800)
#define GPIOD_BASE_ADDR 						(AHB2_PERIPH_BASE_ADDR + 0x0C00)
#define GPIOE_BASE_ADDR 						(AHB2_PERIPH_BASE_ADDR + 0x1000)
#define GPIOF_BASE_ADDR 						(AHB2_PERIPH_BASE_ADDR + 0x1400)
#define GPIOG_BASE_ADDR 						(AHB2_PERIPH_BASE_ADDR + 0x1800)
#define GPIOH_BASE_ADDR 						(AHB2_PERIPH_BASE_ADDR + 0x1C00)


#define RCC_BASE_ADDR							(AHB1_PERIPH_BASE_ADDR + 0x1000)

#define DMA1_BASE_ADDR							(AHB1_PERIPH_BASE_ADDR + 0x0000)
#define DMA2_BASE_ADDR							(AHB1_PERIPH_BASE_ADDR + 0x0400)
/* APB1 BASED PERIPHERAL ADDRESSES */

#define SPI2_BASE_ADDR							(APB1_PERIPH_BASE_ADDR + 0X3800)
#define SPI3_BASE_ADDR							(APB1_PERIPH_BASE_ADDR + 0X3C00)

#define USART2_BASE_ADDR						(APB1_PERIPH_BASE_ADDR + 0X4400)
#define USART3_BASE_ADDR						(APB1_PERIPH_BASE_ADDR + 0X4800)

#define UART4_BASE_ADDR							(APB1_PERIPH_BASE_ADDR + 0X4C00)
#define UART5_BASE_ADDR							(APB1_PERIPH_BASE_ADDR + 0X5000)

#define I2C1_BASE_ADDR							(APB1_PERIPH_BASE_ADDR + 0X5400)
#define I2C2_BASE_ADDR							(APB1_PERIPH_BASE_ADDR + 0X5800)
#define I2C3_BASE_ADDR							(APB1_PERIPH_BASE_ADDR + 0X5C00)


/*  APB2 BASED PERIPHERAL ADDRESSES */

#define EXTI_BASE_ADDR							(APB2_PERIPH_BASE_ADDR + 0X0400)
#define SPI1_BASE_ADDR							(APB2_PERIPH_BASE_ADDR + 0X3000)
#define USART1_BASE_ADDR						(APB2_PERIPH_BASE_ADDR + 0X3800)
#define SYSCFG_BASE_ADDR						(APB2_PERIPH_BASE_ADDR + 0X0000)


/*  ADC offsets For master and slave */
#define MASTER_ADC_OFFSET                            0x00
#define SLAVE_ADC_OFFSET						     0x100
#define MS_COMMON_ADC_OFFSET					 	 0x300

#define ADC1_BASE_ADDR    						(0x50040000U)
#define ADC_COMM_ADDR     						(0x50040300U)

/*----------------------------------RIMx Addresses------------------------------------------*/

#define TIM2_BASE_ADDR							(PERIPH_BASE  + 0x0000)
#define TIM3_BASE_ADDR							(PERIPH_BASE  + 0x0400)

/* ---------------------peripheral register definition structures------------------------*/

typedef struct{
				__vo uint32_t MODER;			//	GPIO port mode register
				__vo uint32_t OTYPER;			//	GPIO port output type register
				__vo uint32_t OSPEEDR;			//	GPIO port output speed register
				__vo uint32_t PUPDR;			//	GPIO port pull-up/pull-down register
				__vo uint32_t IDR;				//	GPIO port input data register
				__vo uint32_t ODR;				//	GPIO port output data register
				__vo uint32_t BSRR;				//	GPIO port bit set/reset register
				__vo uint32_t LCKR;				//	GPIO port configuration lock register
				__vo uint32_t AFR[2];			//	GPIO alternate function LOW[0] (0X20) & HIGH[1] (0X24) register
				__vo uint32_t BRR;		 		//	GPIO port bit reset register
				__vo uint32_t ASCR;       		//	GPIO port analog switch control register
}GPIO_RegDef_t;


typedef struct {
			    __vo uint32_t CR;
			    __vo uint32_t ICSCR;
			    __vo uint32_t CFGR;
			    __vo uint32_t PLLCFGR;
			    __vo uint32_t PLLSAI1CFGR;
			    __vo uint32_t PLLSAI2CFGR;
			    __vo uint32_t CIER;
			    __vo uint32_t CIFR;
			    __vo uint32_t CICR;
			    	 uint32_t reserved1;
			    __vo uint32_t AHB1RSTR;
			    __vo uint32_t AHB2RSTR;
			    __vo uint32_t AHB3RSTR;
			    	 uint32_t reserved2;
			    __vo uint32_t APB1RSTR1;
			    __vo uint32_t APB1RSTR2;
			    __vo uint32_t APB2RSTR;
		    	 	 uint32_t reserved3;
			    __vo uint32_t AHB1ENR;
			    __vo uint32_t AHB2ENR;
			    __vo uint32_t AHB3ENR;
			         uint32_t reserved9;
			    __vo uint32_t APB1ENR1;
			    __vo uint32_t APB1ENR2;
			    __vo uint32_t APB2ENR;
		    	 	 uint32_t reserved5;
			    __vo uint32_t AHB1SMENR;
			    __vo uint32_t AHB2SMENR;
			    __vo uint32_t AHB3SMENR;
		    	 	 uint32_t reserved6;
			    __vo uint32_t APB1SMENR1;
			    __vo uint32_t APB1SMENR2;
			    __vo uint32_t APB2SMENR;
		    	 	 uint32_t reserved7;
			    __vo uint32_t CCIPR;
		    	 	 uint32_t reserved8;
			    __vo uint32_t BDCR;
			    __vo uint32_t CSR;
			    __vo uint32_t CRRCR;
			    __vo uint32_t CCIPR2;

}RCC_RegDef_t;

// Peripheral register definition for EXTI

typedef struct {
	__vo uint32_t IMR1;     // offset 0x00
	__vo uint32_t EMR1;		// 0x04
	__vo uint32_t RTSR1;	// 0x08
	__vo uint32_t FTSR1;	// 0x0C
	__vo uint32_t SWIER1;	// 0x10
	__vo uint32_t PR1;
		 uint32_t reserved1;
	     uint32_t reserved2;
	__vo uint32_t IMR2;
	__vo uint32_t EMR2;
	__vo uint32_t RTSR2;
	__vo uint32_t FTSR2;
	__vo uint32_t SWIER2;
	__vo uint32_t PR2;
}EXTI_RegDef_t;


// Peripheral Register definition for SYSCFG

typedef struct {

		__vo uint32_t MEMRMP; // offset 0x00
		__vo uint32_t CFGR1;	// 0x04
		__vo uint32_t EXTICR[4];	// 0x08 - 0x1C
		__vo uint32_t SCSR;
		__vo uint32_t CFGR2;
		__vo uint32_t SWPR;
		__vo uint32_t SKR;

}SYSCFG_RegDef_t;


typedef struct{
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t SR;
	__vo uint32_t DR;
	__vo uint32_t CRCPR;
	__vo uint32_t RXCRCR;
	__vo uint32_t TXCRCR;
}SPI_RegDef_t;


typedef struct{
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t OAR1;
	__vo uint32_t OAR2;
	__vo uint32_t TIMINGR;
	__vo uint32_t TIMEOUTR;
	__vo uint32_t ISR;
	__vo uint32_t ICR;
	__vo uint32_t PECR;
	__vo uint32_t RXDR;
	__vo uint32_t TXDR;

}I2C_RegDef_t;



typedef struct {
	 __vo uint32_t ISR;      // 0x00
	 __vo uint32_t IER;      // 0x04
	 __vo uint32_t CR;      // 0x08
	 __vo uint32_t CFGR;      // 0x0C
	 __vo uint32_t CFGR2;      // 0x10
	 __vo uint32_t SMPR1;      // 0x14
	 __vo uint32_t SMPR2;      // 0x18
	 	   uint32_t Reserved1;      // 0x1C
	 __vo uint32_t TR1;      // 0x20
	 __vo uint32_t TR2;      // 0x24
	 __vo uint32_t TR3;      // 0x28
	       uint32_t Reserved2;      // 0x2C
	 __vo uint32_t SQR1;      // 0x30
	 __vo uint32_t SQR2;      // 0x34
	 __vo uint32_t SQR3;      // 0x38
	 __vo uint32_t SQR4;      // 0x3C
	 __vo uint32_t DR;      // 0x40
	       uint32_t Reserved4;      // 0x44
	       uint32_t Reserved19;      // 0x48
	 __vo uint32_t JSQR;      // 0x4C
	       uint32_t Reserved5;      // 0x50
	       uint32_t Reserved6;      // 0x54
	       uint32_t Reserved7;      // 0x58
	       uint32_t Reserved8;      // 0x5C
	 __vo uint32_t OFR1;      // 0x60
	 __vo uint32_t OFR2;      // 0x64
	 __vo uint32_t OFR3;      // 0x68
	 __vo uint32_t OFR4;      // 0x6C
	       uint32_t Reserved12;      // 0x70
	 	   uint32_t Reserved9;      // 0x74
		   uint32_t Reserved10;      // 0x78
		   uint32_t Reserved11;      // 0x7C
	 __vo uint32_t JDR1;      // 0x80
	 __vo uint32_t JDR2;      // 0x84
	 __vo uint32_t JDR3;      // 0x88
	 __vo uint32_t JDR4;      // 0x8C
			 uint32_t Reserved13;      // 0x90
			 uint32_t Reserved14;      // 0x94
			 uint32_t Reserved15;      // 0x98
			 uint32_t Reserved16;      // 0x9c
	 __vo uint32_t AWD2CR;      // 0xA0
	 __vo uint32_t AWD3CR;      // 0xA4
			 uint32_t Reserved17;      // 0xA8
			 uint32_t Reserved18;      // 0xAC
		 __vo uint32_t DIFSEL;      // 0xB0
		 __vo uint32_t CALFACT;      // 0xB4

}ADC_RegDef_t;



typedef struct {
	 	 	 __vo uint32_t CSR;      // 0x00
	 	 	 	  uint32_t Reserved20;      // 0x04 _
			 __vo uint32_t CCR;      // 0x08
			 __vo uint32_t CDR;      // 0x0C
}ADC_ComRegDef_t;

typedef struct {
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t CR3;
	__vo uint32_t BRR;
	__vo uint32_t GTPR;
	__vo uint32_t RTOR;
	__vo uint32_t RQR;
	__vo uint32_t ISR;
	__vo uint32_t ICR;
	__vo uint32_t RDR;
	__vo uint32_t TDR;
} USART_RegDef_t;



typedef struct {
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t SMCR;
	__vo uint32_t DIER;
	__vo uint32_t SR;
	__vo uint32_t EGR;
	__vo uint32_t CCMR1;
	__vo uint32_t CCMR2;
	__vo uint32_t CCER;
	__vo uint32_t CNT;
	__vo uint32_t PSC;
	__vo uint32_t ARR;
		 uint32_t Reserved;
	__vo uint32_t CCR1;
	__vo uint32_t CCR2;
	__vo uint32_t CCR3;
	__vo uint32_t CCR4;
	 	 uint32_t Reserved2;
	__vo uint32_t DCR;
	__vo uint32_t DMAR;
	__vo uint32_t OR1;
	__vo uint32_t OR2;
} TIM_RegDef_t;

typedef struct {

		__vo uint32_t ISR;
		__vo uint32_t IFCR;

		struct{
			__vo uint32_t CCR;
			__vo uint32_t CNDTR;
			__vo uint32_t CPAR;
			__vo uint32_t CMAR;
				 uint32_t Reserved;
				}CH[7];

			 uint32_t Reserved_Gap[5];
		__vo uint32_t CSELR;

}DMA_RegDef_t;
/* ----------------------------Peripheral addresses defined (GPIO_RegDef)------------------------*/

#define GPIOA									((GPIO_RegDef_t*)GPIOA_BASE_ADDR)
#define GPIOB									((GPIO_RegDef_t*)GPIOB_BASE_ADDR)
#define GPIOC									((GPIO_RegDef_t*)GPIOC_BASE_ADDR)
#define GPIOD									((GPIO_RegDef_t*)GPIOD_BASE_ADDR)
#define GPIOE									((GPIO_RegDef_t*)GPIOE_BASE_ADDR)
#define GPIOF									((GPIO_RegDef_t*)GPIOF_BASE_ADDR)
#define GPIOG									((GPIO_RegDef_t*)GPIOG_BASE_ADDR)
#define GPIOH									((GPIO_RegDef_t*)GPIOH_BASE_ADDR)


/* ----------------------------Peripheral addresses defined (SPI_RegDef)------------------------*/

#define SPI1									((SPI_RegDef_t*)SPI1_BASE_ADDR)
#define SPI2									((SPI_RegDef_t*)SPI2_BASE_ADDR)
#define SPI3									((SPI_RegDef_t*)SPI3_BASE_ADDR)

/*=============================Peripheral addresses for I2C(I2C RegDef==========================*/

#define I2C1 									((I2C_RegDef_t*)I2C1_BASE_ADDR)
#define I2C2 									((I2C_RegDef_t*)I2C2_BASE_ADDR)
#define I2C3 									((I2C_RegDef_t*)I2C3_BASE_ADDR)


#define RCC										((RCC_RegDef_t*)RCC_BASE_ADDR)

#define EXTI									((EXTI_RegDef_t*)EXTI_BASE_ADDR)

#define SYSCFG									((SYSCFG_RegDef_t*)SYSCFG_BASE_ADDR)

#define DMA1									((DMA_RegDef_t*)DMA1_BASE_ADDR)
#define DMA2									((DMA_RegDef_t*)DMA2_BASE_ADDR)

// ===========================ADC Pointer Initialization=========================================
#define ADC1              						((ADC_RegDef_t*)ADC1_BASE_ADDR)
#define ADC1_COMMON       						((ADC_ComRegDef_t*)ADC_COMM_ADDR)


#define ADC1_PCLK_EN()    						(RCC->AHB2ENR |= (1 << 13))
#define ADC1_REG_RESET()  						do { (RCC->AHB2RSTR |= (1 << 13));  RCC->AHB2RSTR &= ~(1 << 13); } while(0)

/*=============================USART ADDRESS REgion=====================================*/

#define USART2              					((USART_RegDef_t*)USART2_BASE_ADDR)
#define USART3 									((USART_RegDef_t*)USART3_BASE_ADDR)

#define USART2_RX_REQUEST_NUMBER				2
#define USART2_TX_REQUEST_NUMBER				2

// ==================================TIM Pointer Initialization======================================
#define TIM2_PCLK_EN()							(RCC->APB1ENR1 |= (1 << 0))
#define TIM2									((TIM_RegDef_t*)TIM2_BASE_ADDR)

#define TIM3_PCLK_EN()							(RCC->APB1ENR1  |= (1 << 1))
#define TIM3									((TIM_RegDef_t*)TIM3_BASE_ADDR)
/*	-------------------------clock enable macros for GPIOx------------------------------------*/

#define GPIOA_PCLK_EN()					(RCC->AHB2ENR |= (1 << 0))
#define GPIOB_PCLK_EN()					(RCC->AHB2ENR |= (1 << 1))
#define GPIOC_PCLK_EN()					(RCC->AHB2ENR |= (1 << 2))
#define GPIOD_PCLK_EN()					(RCC->AHB2ENR |= (1 << 3))
#define GPIOE_PCLK_EN()					(RCC->AHB2ENR |= (1 << 4))
#define GPIOF_PCLK_EN()					(RCC->AHB2ENR |= (1 << 5))
#define GPIOG_PCLK_EN()					(RCC->AHB2ENR |= (1 << 6))
#define GPIOH_PCLK_EN()					(RCC->AHB2ENR |= (1 << 7))


/*--------------------------clock Enable MACROS for I2Cx---------------------------------------*/

#define I2C1_PCLK_EN()					(RCC->APB1ENR1 |= (1 << 21))
#define I2C2_PCLK_EN()					(RCC->APB1ENR1 |= (1 << 22))
#define I2C3_PCLK_EN()					(RCC->APB1ENR1 |= (1 << 23))

/*--------------------------clock Enable MACROS for SPIx---------------------------------------*/

#define SPI2_PCLK_EN()					(RCC->APB1ENR1 |= (1 << 14))
#define SPI3_PCLK_EN()					(RCC->APB1ENR1 |= (1 << 15))
#define SPI1_PCLK_EN()					(RCC->APB2ENR  |= (1 << 12))

/*--------------------------clock Enable MACROS for USARTX---------------------------------------*/

#define USART2_PCLK_EN()					(RCC->APB1ENR1 |= (1 << 17))
#define USART3_PCLK_EN()					(RCC->APB1ENR1 |= (1 << 18))
#define USART1_PCLK_EN()					(RCC->APB2ENR  |= (1 << 14))

#define UART4_PCLK_EN()						(RCC->APB1ENR1 |= (1 << 19))
#define UART5_PCLK_EN()						(RCC->APB1ENR1 |= (1 << 20))

/*--------------------------clock Enable MACROS for SYSCFG---------------------------------------*/

#define SYSCFG_PCLK_EN()					(RCC->APB2ENR |= (1 << 0))
/*======================================DMA part============================= */

#define DMA1_PCLK_EN()						(RCC->AHB1ENR  |= (1 << 0))
#define DMA2_PCLK_EN()						(RCC->AHB1ENR  |= (1 << 1))

#define DMA1_PCLK_RST()						(RCC->AHB1RSTR |= (1 << 0))
#define DMA2_PCLK_RST()						(RCC->AHB1RSTR |= (1 << 1))

#define DMA1_PCLK_DI()						(RCC->AHB1ENR  &= ~(1 << 0))
#define DMA2_PCLK_DI()						(RCC->AHB1ENR  &= ~(1 << 1))

/*	-------------------------clock disable MACROS for GPIOx------------------------------------*/

#define GPIOA_PCLK_DI()					(RCC->AHB2ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()					(RCC->AHB2ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()					(RCC->AHB2ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()					(RCC->AHB2ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()					(RCC->AHB2ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI()					(RCC->AHB2ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI()					(RCC->AHB2ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI()					(RCC->AHB2ENR &= ~(1 << 7))


/*--------------------------clock disable MACROS for I2Cx---------------------------------------*/

#define I2C1_PCLK_DI()						(RCC->APB1ENR1 &= ~(1 << 21))
#define I2C2_PCLK_DI()						(RCC->APB1ENR1 &= ~(1 << 22))
#define I2C3_PCLK_DI()						(RCC->APB1ENR1 &= ~(1 << 23))

/*--------------------------clock disable MACROS for SPIx---------------------------------------*/

#define SPI2_PCLK_DI()						(RCC->APB1ENR1 &= ~(1 << 14))
#define SPI3_PCLK_DI()						(RCC->APB1ENR1 &= ~(1 << 15))
#define SPI1_PCLK_DI()						(RCC->APB2ENR  &= ~(1 << 12))

/*--------------------------clock disable MACROS for USARTX---------------------------------------*/

#define USART2_PCLK_DI()					(RCC->APB1ENR1 &= ~(1 << 17))
#define USART3_PCLK_DI()					(RCC->APB1ENR1 &= ~(1 << 18))
#define USART1_PCLK_DI()					(RCC->APB2ENR  &= ~(1 << 14))

#define UART4_PCLK_DI()						(RCC->APB1ENR1 &= ~(1 << 19))
#define UART5_PCLK_DI()						(RCC->APB1ENR1 &= ~(1 << 20))

/*--------------------------clock disable MACROS for SYSCFG---------------------------------------*/

#define SYSCFG_PCLK_DI()					(RCC->APB2ENR &= ~(1 << 0))

/*-------------------------------------------------------------------------------------------------*/
/*-------------------------------GPIO REGISTER RESET-----------------------------------------------*/

#define GPIOA_REG_RESET()					do { (RCC->AHB2RSTR |= (1 << 0)); (RCC->AHB2RSTR &= ~(1 << 0)); } while (0)
#define GPIOB_REG_RESET()					do { (RCC->AHB2RSTR |= (1 << 1)); (RCC->AHB2RSTR &= ~(1 << 1)); } while (0)
#define GPIOC_REG_RESET()					do { (RCC->AHB2RSTR |= (1 << 2)); (RCC->AHB2RSTR &= ~(1 << 2)); } while (0)
#define GPIOD_REG_RESET()					do { (RCC->AHB2RSTR |= (1 << 3)); (RCC->AHB2RSTR &= ~(1 << 3)); } while (0)
#define GPIOE_REG_RESET()					do { (RCC->AHB2RSTR |= (1 << 4)); (RCC->AHB2RSTR &= ~(1 << 4)); } while (0)
#define GPIOF_REG_RESET()					do { (RCC->AHB2RSTR |= (1 << 5)); (RCC->AHB2RSTR &= ~(1 << 5)); } while (0)
#define GPIOG_REG_RESET()					do { (RCC->AHB2RSTR |= (1 << 6)); (RCC->AHB2RSTR &= ~(1 << 6)); } while (0)
#define GPIOH_REG_RESET()					do { (RCC->AHB2RSTR |= (1 << 7)); (RCC->AHB2RSTR &= ~(1 << 7)); } while (0)


/*-------------------------------SPI REGISTER RESET-----------------------------------------------*/

#define SPI1_REG_RESET()					do { (RCC->APB2RSTR |= (1 << 12)); (RCC->APB2RSTR &= ~(1 << 12)); } while (0)
#define SPI2_REG_RESET()					do { (RCC->APB1RSTR1 |= (1 << 14)); (RCC->APB1RSTR1 &= ~(1 << 14)); } while (0)
#define SPI3_REG_RESET()					do { (RCC->APB1RSTR1 |= (1 << 15)); (RCC->APB1RSTR1 &= ~(1 << 15)); } while (0)


/*-------------------------------I2C REGISTER RESET-----------------------------------------------*/

#define I2C1_REG_RESET()					do { (RCC->APB1RSTR1 |= (1 << 21)); (RCC->APB1RSTR1 &= ~(1 << 21)); } while (0)
#define I2C2_REG_RESET()					do { (RCC->APB1RSTR1 |= (1 << 22)); (RCC->APB1RSTR1 &= ~(1 << 22)); } while (0)
#define I2C3_REG_RESET()					do { (RCC->APB1RSTR1 |= (1 << 23)); (RCC->APB1RSTR1 &= ~(1 << 23)); } while (0)


// @GPIO PIN SYSCFG_EXTIx pin configuration


#define PORTCODE_BASEADDR(x)	 (  		(x == GPIOA) ? 0 :\
											(x == GPIOB) ? 1 :\
											(x == GPIOC) ? 2 :\
											(x == GPIOD) ? 3 :\
											(x == GPIOE) ? 4 :\
											(x == GPIOF) ? 5 :\
											(x == GPIOG) ? 6 : 0)

/*==================================DMA===============================================================================*/
#define USART2_RX_DMA_NO				1
#define USART2_TX_DMA_NO				1

#define USART2_TX_DMA_CHANNEL_NO		7
#define USART2_RX_DMA_CHANNEL_NO		6

/* --------------------------------Interrupt Request Number Positions for EXTIx on VECTRO Table-----------------------*/
#define IRQ_NO_EXTI0				6
#define IRQ_NO_EXTI1				7
#define IRQ_NO_EXTI2				8
#define IRQ_NO_EXTI3				9
#define IRQ_NO_EXTI4				10
#define IRQ_NO_EXTI9_5				23
#define IRQ_NO_EXTI15_10			40

/*---------------------------------Interrupt Request Number for SPI---------------------------------------------------*/
#define IRQ_NO_SPI1					35
#define IRQ_NO_SPI2					36
#define IRQ_NO_SPI3					51

/*==================================Intrrupt Request Number for USART/UART============================================*/
#define IRQ_NO_USART1				37
#define IRQ_NO_USART2				38
#define IRQ_NO_USART3				39

/*==================================NVIC Interrupt Request Priority Numbers==============================================*/

#define NVIC_IRQ_PRI0				0
#define NVIC_IRQ_PRI1				1
#define NVIC_IRQ_PRI2				2
#define NVIC_IRQ_PRI3				3
#define NVIC_IRQ_PRI4				4
#define NVIC_IRQ_PRI5				5
#define NVIC_IRQ_PRI6				6
#define NVIC_IRQ_PRI7				7
#define NVIC_IRQ_PRI8				8
#define NVIC_IRQ_PRI9				9
#define NVIC_IRQ_PRI10				10
#define NVIC_IRQ_PRI11				11
#define NVIC_IRQ_PRI12				12
#define NVIC_IRQ_PRI13				13
#define NVIC_IRQ_PRI14				14
#define NVIC_IRQ_PRI15				15


// General Macros
#define ENABLE 								1
#define DISABLE 							0
#define NULL								0
#define SET									ENABLE
#define RESET								DISABLE


#define GPIO_PIN_SET						SET
#define GPIO_PIN_RESET						RESET

#define FLAG_SET							SET
#define FLAG_RESET							RESET

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////    Bit POsition Definitions for SPI Peripheral   //////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

#define SPI_CR1_CPHA						0
#define SPI_CR1_CPOL						1
#define SPI_CR1_DEVICE_MODE					2
#define SPI_CR1_SCLK						3
#define SPI_CR1_SPE  						6
#define SPI_CR1_LSBFIRST					7
#define SPI_CR1_SSI 						8
#define SPI_CR1_SSM						    9
#define SPI_CR1_RX						    10
#define SPI_CR1_DFF							11
#define SPI_CR1_CRCNEXT						12
#define SPI_CR1_CRCEN						13
#define SPI_CR1_BIDIOE						14
#define SPI_CR1_BIDI						15

////////////////////////////////////// Bit Positions for CR2///////////////////////////////////
#define SPI_CR2_RXDMAEN						0
#define SPI_CR2_TXDMAEN						1
#define SPI_CR2_SSOE						2
#define SPI_CR2_NSSP  						3
#define SPI_CR2_FRF							4
#define SPI_CR2_ERRIE						5
#define SPI_CR2_RXNEIE						6
#define SPI_CR2_TXEIE						7
#define SPI_CR2_DATASIZE					8
#define SPI_CR2_FRXTH						12
#define SPI_CR2_LDMA_RX						13
#define SPI_CR2_LDMA_TX						14


////////////////////////////////////// Bit Positions for SR///////////////////////////////////
#define SPI_SR_RXNE						0
#define SPI_SR_TXE						1
#define SPI_SR_CRCERR					4
#define SPI_SR_MODF						5
#define SPI_SR_OVR						6
#define SPI_SR_BSY						7
#define SPI_SR_FRE						8
#define SPI_SR_FRLVL					9
#define SPI_SR_FTLVL					11


/* -------------------------------------Bit Position for CR1 I2C----------------------------------*/

#define PECEN							23
#define ALERTEN							22
#define SMBDEN							21
#define SMBHEN							20
#define GCEN							19
#define WUPEN							18
#define NOSTRETCH						17
#define SBC								16
#define RXDMAEN							15
#define TXDMAEN							14
#define ANFOF							12
#define ERRIE							7
#define TCIE							6
#define STOPIE							5
#define NACKIE							4
#define ADDRIE							3
#define RXIE							2
#define TXIE							1
#define PE								0

/* -------------------------------------Bit Position for CR2 I2C----------------------------------*/
#define PECBYTE							26
#define AUTOEND							25
#define RELOAD							24
#define NACK							15
#define STOP							14
#define START							13
#define HEAD10R							12
#define ADD10							11
#define RD_WRN							10

/* -------------------------------------Bit Position for ISR I2C----------------------------------*/
#define DIR								16
#define BUSY							15
#define ALERT							13
#define TIMEOUT							12
#define PECERR							11
#define OVR								10
#define ARLO							9
#define BERR							8
#define TCR								7
#define TC								6
#define STOPF							5
#define NACKF							4
#define ADDR							3
#define RXNE							2
#define TXIS							1
#define TXE								0

/* -------------------------------------Bit Position for ICR I2C----------------------------------*/

#define RTCF							13
#define TIMOUTCF						12
#define PECCF							11
#define OVRCF							10
#define ARLOCF							9
#define BERRCF							8
#define STOPCF							5
#define NACKCF							4
#define ADDRCF							3

#include "stm32l476xx_gpio_driver.h"
#include "stm32l476xx_spi_driver.h"
#include "stm32l476xx_i2c_driver.h"
#include "stm32l476xx_adc_driver.h"
#include "stm32l476xx_usart_driver.h"
#include "stm32l476xx_TIMx_driver.h"
#include "stm32l476xx_DMA_driver.h"
#include "systick.h"
#endif /* INC_STM32L476XX_H_ */

/*
 * Ucglib.c
 *
 *  Created on: Apr 4, 2024
 *      Author: lalig
 */


#include "Ucglib.h"
#include "stm32f401re_rcc.h"
#include "stm32f401re_gpio.h"

#define SPI1_CS_PORT		GPIOB
#define SPI1_CS_PIN			GPIO_Pin_6
#define SPI1_RST_PORT		GPIOC
#define SPI1_RST_PIN		eGPIO_Pin_7
#define SPI1_MOSI_PORT		GPIOA
#define SPI1_MOSI_PIN		GPIO_Pin_7
#define SPI1_SCK_PORT		GPIOA
#define SPI1_SCK_PIN		GPIO_Pin_5
#define SPI1_RS_PORT		GPIOA
#define SPI1_RT_PIN			GPIO_Pin_9
#define SPI1_ENABLE_PORT	GPIOB
#define SPI1_ENABLE_PIN		GPIO_Pin_10
#define SPI1_MODE_PORT		GPIOA
#define SPI1_MODE_PIN		GPIO_Pin_8

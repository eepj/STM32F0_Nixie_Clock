#ifndef NIXIE_H
#define NIXIE_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include "main.h"

#define NIXIE_DEBUG	1

#define OE_PORT 	GPIOA
#define OE_PIN		GPIO_PIN_1
#define SHDN_PORT 	GPIOA
#define SHDN_PIN	GPIO_PIN_7
#define DATA_PORT	GPIOA
#define DATA_PIN	GPIO_PIN_13
#define CLK_PORT	GPIOA
#define CLK_PIN		GPIO_PIN_14

#define NIXIE_POWER_UP			SHDN_PORT->ODR &= ~SHDN_PIN
#define NIXIE_POWER_DOWN 		SHDN_PORT->ODR |= SHDN_PIN
#define NIXIE_OUTPUT_ENABLED	OE_PORT->ODR |= OE_PIN
#define NIXIE_OUTPUT_DISABLED	OE_PORT->ODR &= ~OE_PIN
#define NIXIE_DATA_LOW			DATA_PORT->ODR &= ~DATA_PIN
#define NIXIE_CLK_LOW			CLK_PORT->ODR &= ~CLK_PIN
#define NIXIE_DATA_HIGH			DATA_PORT->ODR |= DATA_PIN
#define NIXIE_CLK_HIGH			CLK_PORT->ODR |= CLK_PIN

#define NIXIE_HOUR_10			5
#define NIXIE_HOUR_1			4
#define NIXIE_MINUTE_10			3
#define NIXIE_MINUTE_1			2
#define NIXIE_SECOND_10			1
#define NIXIE_SECOND_1			0

void Nixie_Init(I2C_HandleTypeDef *ui2c, TIM_HandleTypeDef * utim);
void Nixie_InterruptHandler(void);

void Nixie_Shift(uint64_t sr);
void Nixie_PrintShift(uint64_t sr);

void Nixie_Timer3DelayUs(uint16_t us);

void Nixie_SetDebugUART(UART_HandleTypeDef* uuart);

extern TIM_HandleTypeDef* nixie_utim;
extern I2C_HandleTypeDef* nixie_ui2c;
extern UART_HandleTypeDef* nixie_uuart;

#ifdef __cplusplus
}
#endif

#endif

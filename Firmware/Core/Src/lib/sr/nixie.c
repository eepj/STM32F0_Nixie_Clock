#include "nixie.h"
#include "../rtc/ds3231_for_stm32_hal.h"
#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif

TIM_HandleTypeDef *_nixie_utim;
I2C_HandleTypeDef *_nixie_ui2c;
UART_HandleTypeDef *_nixie_uuart;

__weak int __io_putchar(int ch) {
#if NIXIE_DEBUG
	uint8_t temp = ch;
	HAL_UART_Transmit(_nixie_uuart, &temp, 1, HAL_MAX_DELAY);
#endif
	return ch;
}

void Nixie_Init(I2C_HandleTypeDef *ui2c, TIM_HandleTypeDef *utim) {
	_nixie_utim = utim;
	_nixie_ui2c = ui2c;
	DS3231_Init(_nixie_ui2c);
	DS3231_SetInterruptMode(DS3231_ALARM_INTERRUPT);
	DS3231_EnableAlarm1(DS3231_ENABLED);
	DS3231_EnableAlarm2(DS3231_DISABLED);
	DS3231_SetAlarm1Mode(DS3231_A1_EVERY_S);
	HAL_TIM_Base_Start(_nixie_utim);
}

void Nixie_SetDebugUART(UART_HandleTypeDef *uuart) {
	_nixie_uuart = uuart;
}

void Nixie_Shift(uint64_t sr) {
	for (uint8_t i = 0; i < 64; i++) {
		NIXIE_CLK_LOW;
		uint64_t bit = ((uint64_t) sr & ((uint64_t) 1 << (63 - i))) >> (63 - i);
		if (bit) {
			NIXIE_DATA_HIGH;
		} else {
			NIXIE_DATA_LOW;
		}
		Nixie_TimerDelayMicrosecond(10);
		NIXIE_CLK_HIGH;
		Nixie_TimerDelayMicrosecond(10);
	}
}

void Nixie_InterruptHandler(void) {
	if (DS3231_IsAlarm1Triggered()) {
		uint8_t nixie_h, nixie_m, nixie_s;
		uint64_t nixie_sr;

		DS3231_ClearAlarm1Flag();

		nixie_h = DS3231_GetHour();
		nixie_m = DS3231_GetMinute();
		nixie_s = DS3231_GetSecond();

		nixie_sr = 0;
		nixie_sr |= ((uint64_t) 1 << (nixie_s / 10 + (NIXIE_SECOND_10 * 10) + 1));
		nixie_sr |= ((uint64_t) 1 << (nixie_s % 10 + (NIXIE_SECOND_1 * 10) + 1));
		nixie_sr |= ((uint64_t) 1 << (nixie_m / 10 + (NIXIE_MINUTE_10 * 10) + 1));
		nixie_sr |= ((uint64_t) 1 << (nixie_m % 10 + (NIXIE_MINUTE_1 * 10) + 1));
		nixie_sr |= ((uint64_t) 1 << (nixie_h / 10 + (NIXIE_HOUR_10 * 10) + 1));
		nixie_sr |= ((uint64_t) 1 << (nixie_h % 10 + (NIXIE_HOUR_1 * 10) + 1));

		Nixie_PrintShift(nixie_sr);
		NIXIE_OUTPUT_DISABLED;
		Nixie_Shift(nixie_sr);
		NIXIE_OUTPUT_ENABLED;
	}
	if (DS3231_IsAlarm2Triggered()) {
		DS3231_ClearAlarm2Flag();
	}
}

void Nixie_PrintShift(uint64_t sr) {
#if NIXIE_DEBUG
	for (uint8_t i = 0; i < 64; i++) {
		char str[2] = { };
		uint8_t bit = 1 & ((sr >> (63 - i)));
		sprintf(str, "%d", (int) bit);
		printf(str);
		if ((63 - i) % 10 == 0 && i != 0)
			printf(" ");
	}
	printf("\n");
#endif
}

void Nixie_TimerDelayMicrosecond(uint16_t us) {
	__HAL_TIM_SET_COUNTER(_nixie_utim, 0);
	while (__HAL_TIM_GET_COUNTER(_nixie_utim) < us);
}

#ifdef __cplusplus
}
#endif

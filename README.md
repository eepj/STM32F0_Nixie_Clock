# STM32F0_Nixie_Clock
An STM32 nixie clock project.

## Disclaimer
High DC voltages can cause severe injuries or death. Proceed at your own risk.

## Hardware 
### Files
* [KiCad project](./Hardware/STM32F0_Nixie_Clock.pro)
* [Schematics](./Hardware/STM32F0_Nixie_Clock.pdf)
* [Gerber](./main/Hardware/Gerber)

### Parts
Refer to [BOM](./Hardware/STM32F0_Nixie_Clock_BOM.xlsx):
* [STM32F042F6 core](https://www.st.com/resource/en/datasheet/stm32f042f6.pdf)
* [MAX1771 step-up driver](https://datasheets.maximintegrated.com/en/ds/MAX1771.pdf)
* [DS3231 I2C RTC](https://www.maximintegrated.com/en/products/analog/real-time-clocks/DS3231.html)
* [HV5222 32-channel open-drain serial-to-parallel converter](https://ww1.microchip.com/downloads/en/DeviceDoc/20005418B.pdf)

## Firmware
### Programs
* Main program: [`main.c`](./Firmware/Core/Src/main.c)
* Interrupts: [`stm32f0xx_it.c`](/Firmware/Core/Src/stm32f0xx_it.c)

### Custom libraries
* HV5222 driver: [`nixie.h`](./Firmware/Core/Src/lib/sr)
* DS3231 driver: [`ds3231_for_stm32_hal.h`](https://github.com/eepj/ds3231_for_stm32_hal)

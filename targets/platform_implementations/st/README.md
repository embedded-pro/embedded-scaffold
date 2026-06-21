# platform_implementations/st

STM32 implementation of `platform::Platform`, targeting the **STM32F429I-DISC1**.

- **StatusLed** — LD3 (green) on PG13.
- **Communication** — `hal::UartStm` on USART1 (TX = PA9, RX = PA10), 115200 8N1.
  The on-board ST-LINK has no virtual COM port on this board; connect a USB-UART
  adapter to PA9/PA10/GND.
- **Tracer** — `services::TracerToStream` over the UART.
- **Run** — runs `main_::StmEventInfrastructure`. A first member (`ClockInit`) calls
  `HAL_Init()` + `ConfigureDefaultClockNucleoF429ZI()` before any peripheral is
  constructed (8 MHz HSE).

Selected by the `STM32F429I-DISC1` preset. To support another STM32 board, copy this
folder, adjust the LED/UART pins and clock config, and add a preset.

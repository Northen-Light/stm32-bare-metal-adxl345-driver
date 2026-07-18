# STM32 Bare-Metal ADXL345 Driver

A bare-metal driver for the Analog Devices ADXL345 3-axis accelerometer, written for the STM32F103C8T6 Blue Pill.

## Highlights

- Built entirely without STM32 HAL or LL
- Developed and validated on STM32F103 hardware
- Implements STM32F1 I²C state machine using polling
- Supports interrupt-driven data acquisition
- Uses a layered driver architecture

## Features

### STM32 I²C driver

- Register-level STM32F103 I²C implementation
- Single-byte register writes
- Single-byte register reads
- Multi-byte burst register reads
- Repeated START generation for register reads
- ACK/NACK handling
- Polling-based STM32 I²C state-machine handling using `SB`, `ADDR`, `TXE`, `RXNE`, and `BTF`

### ADXL345 driver

- Device ID verification
- Configurable measurement range
- Configurable output data rate
- Raw X/Y/Z acceleration reads
- Conversion from raw samples to acceleration in `g`
- Explicit start and stop measurement control
- Configurable interrupt enable
- Interrupt routing to `INT1` or `INT2`
- Encapsulated ADXL345 register access
- Read-back verification after configuration writes

### Interrupt-driven acquisition

- ADXL345 `DATA_READY` interrupt support
- STM32 AFIO and EXTI configuration
- NVIC interrupt handling
- Minimal interrupt service routine
- Acceleration reads performed from the main loop

## Hardware

- STM32F103C8T6 Blue Pill
- ADXL345 accelerometer module
- ST-Link V2
- I²C bus configured for 100 kHz

## Connections

| STM32F103 | ADXL345 | Purpose |
|---|---|---|
| PB6 | SCL | I²C clock |
| PB7 | SDA | I²C data |
| PA0 | INT1 | Data-ready interrupt |
| 3.3 V | VCC | Power |
| GND | GND | Ground |

## Software architecture

```text
Application
    |
    v
ADXL345 driver
    |
    v
STM32 I²C driver
    |
    v
STM32F103 I²C peripheral
    |
    v
ADXL345
```

The application communicates with the sensor through the public ADXL345 API. It does not access ADXL345 registers directly.

## Project structure

```text
.
├── app/
│   └── main.c
├── drivers/
│   ├── adxl345.c
│   └── i2c.c
├── include/
│   ├── adxl345.h
│   ├── i2c.h
│   └── stm32f103xx.h
├── linker/
│   └── main.ld
├── startup/
│   └── startup.c
├── Makefile
├── README.md
└── DESIGN.md
```

## Initialization

`adxl345_init()`:

- Verifies the device ID
- Places the sensor in standby mode
- Configures the measurement range
- Enables full-resolution mode
- Configures the output data rate

Measurement is started explicitly using:

```c
adxl345_start_measurement();
```

## Public API

The driver provides functions to:

- Initialize the sensor
- Read raw acceleration
- Read acceleration in `g`
- Configure measurement range
- Configure output data rate
- Start/stop measurement
- Configure interrupt routing

## Register-read transaction

A register read uses a repeated START:

```text
START → Address+Write → Register → Repeated START → Address+Read → Data → STOP
```

The acceleration-read function retrieves `DATAX0` through `DATAZ1` in one six-byte burst transaction.

## Interrupt flow

```text
DATA_READY → PA0 → EXTI0 → NVIC → ISR → Main loop
```

The blocking I²C transaction is intentionally kept outside the interrupt handler.


## Build

```bash
make
```

## Flash

```bash
make flash
```

## Development tools

- arm-none-eabi-gcc
- gdb
- GNU Make
- st-util
- ST-Link V2

## Current limitations

- I²C operations use polling
- No timeout handling
- No detailed I²C error reporting
- No bus-recovery logic
- No interrupt- or DMA-driven I²C transfers
- ADXL345 FIFO is not used
- A Boolean data-ready flag may combine multiple pending samples into one event

## Future improvements

- Add polling timeouts
- Add detailed I²C error codes
- Handle address NACK, bus error, arbitration loss, and overrun
- Add I²C bus recovery
- Add ADXL345 FIFO support
- Add FIFO watermark interrupt support
- Add activity, inactivity, tap, and double-tap detection
- Capture and document I²C and interrupt timing using a logic analyzer

## References

- STM32F103 Reference Manual, RM0008
- STM32F103 datasheet
- Analog Devices ADXL345 datasheet
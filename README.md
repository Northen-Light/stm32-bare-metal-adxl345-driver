# STM32 Bare-Metal ADXL345 Driver

A register-level, bare-metal firmware driver for the Analog Devices ADXL345 3-axis accelerometer written for the STM32F103 (Blue Pill).

The project implements both a generic I²C driver and a reusable ADXL345 device driver without using STM32 HAL or LL libraries.

The focus of the project is understanding the STM32F1 I²C peripheral, embedded driver architecture, and interrupt-driven sensor communication.

---

## Features

### I²C Driver

- Register-level STM32F103 I²C implementation
- Master transmit
- Master receive
- Single-byte register reads
- Multi-byte burst reads
- Repeated START support
- ACK/NACK handling
- Polling-based transfers

### ADXL345 Driver

- Device ID verification
- Register read/write abstraction
- Register write verification
- Measurement mode control
- Configurable output data rate
- Configurable measurement range
- Raw acceleration readings
- Acceleration conversion to g

### Interrupt Support

- DATA_READY interrupt
- Configurable interrupt routing (INT1 / INT2)
- EXTI configuration
- NVIC interrupt handling
- Interrupt-driven data acquisition

---

## Hardware

- STM32F103C8T6 (Blue Pill)
- ADXL345 Accelerometer

### I²C Connections

| STM32 | ADXL345 |
|--------|----------|
| PB6 | SCL |
| PB7 | SDA |
| PA0 | INT1 |
| 3.3V | VCC |
| GND | GND |
| CS | 3.3V (I²C Mode) |
| SDO | GND (Address = 0x53) |

I²C Bus Speed: **100 kHz**

---

## Driver Architecture

```
Application
      │
      ▼
 ADXL345 Driver
      │
      ▼
    I²C Driver
      │
      ▼
 STM32 I²C Peripheral
      │
      ▼
    ADXL345
```

The application never accesses device registers directly.

The ADXL345 driver is completely isolated from the STM32 peripheral implementation and communicates only through the I²C driver.

---

## Project Structure

```
.
├── app/
├── drivers/
│   ├── adxl345.c
│   └── i2c.c
├── include/
├── linker/
├── startup/
├── Makefile
├── README.md
└── LEARNING.md
```

---

## Driver API

```c
bool adxl345_init(void);

bool adxl345_read_raw(adxl345_raw_data_t *data);

bool adxl345_read_acceleration(adxl345_acceleration_t *data);

bool adxl345_start_measurement(void);

bool adxl345_stop_measurement(void);

bool adxl345_set_range(adxl345_range_t range);

bool adxl345_set_data_rate(adxl345_data_rate_t rate);

bool adxl345_set_interrupt_bit(
    adxl345_interrupt_bit_t interrupt,
    adxl345_interrupt_channel_t channel);
```

---

## Initialization Sequence

```
Read Device ID

↓

Enter Standby Mode

↓

Configure DATA_FORMAT

↓

Configure Output Data Rate

↓

Initialization Complete
```

Measurement mode is enabled separately by calling:

```c
adxl345_start_measurement();
```

---

## Interrupt Flow

```
ADXL345

↓

DATA_READY

↓

INT1

↓

PA0

↓

EXTI0

↓

NVIC

↓

ISR

↓

Application reads acceleration
```

---

## I²C Read Sequence

The STM32F1 I²C peripheral is implemented using the polling state machine described in RM0008.

Implemented sequences include:

- START generation
- Address phase
- Register addressing
- Repeated START
- Single-byte receive
- Multi-byte receive
- ACK/NACK management
- STOP generation

---

## Build

```bash
make
```

Flash

```bash
make flash
```

---

## Development Tools

- arm-none-eabi-gcc
- GNU Make
- arm-none-eabi-gdb
- st-util
- ST-Link V2

---

## References

- STM32F103 Reference Manual (RM0008)
- STM32F103 Datasheet
- ADXL345 Datasheet
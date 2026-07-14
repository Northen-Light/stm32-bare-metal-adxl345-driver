# STM32 Bare-Metal ADXL345 Driver

A register-level, bare-metal driver for the Analog Devices ADXL345 3-axis accelerometer written for the STM32F103 (Blue Pill).

This project implements both a generic I²C driver and a reusable ADXL345 device driver without using STM32 HAL or LL libraries.

---

## Features

- Bare-metal STM32F103 firmware
- Register-level I²C driver
- ADXL345 device driver
- Device ID verification
- Register read/write
- Multi-byte burst reads
- Raw X/Y/Z acceleration readings
- Acceleration conversion to **g**
- Configurable measurement range
- Configurable output data rate
- Measurement mode control
- Read-back verification after configuration writes

---

## Hardware

- STM32F103C8T6 (Blue Pill)
- ADXL345 3-Axis Accelerometer
- I²C @ 100 kHz

Connections

| STM32 | ADXL345 |
|-------|----------|
| PB6 | SCL |
| PB7 | SDA |
| 3.3V | VCC |
| GND | GND |
| CS | 3.3V (I²C Mode) |
| SDO | GND (Address = 0x53) |

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

The application never directly accesses device registers.
All register access is encapsulated inside the ADXL345 driver.

---

## Project Structure

```
STM32-BARE-METAL-ADXL345-DRIVER
│
├── app/
│   └── main.c
│
├── drivers/
│   ├── adxl345.c
│   └── i2c.c
│
├── include/
│   ├── adxl345.h
│   ├── i2c.h
│   ├── stm32f103xx.h
│   └── stdint.h
│
├── linker/
|   └── main.ld
│
├── startup/
|   └── startup.c
│
├── Makefile
└── ...
```

---

## Public Driver API

```c
bool adxl345_init(void);

uint8_t adxl345_read_device_id(void);

bool adxl345_read_raw(adxl345_raw_data_t *data);

bool adxl345_read_acceleration(
    adxl345_acceleration_t *data);

bool adxl345_set_range(
    adxl345_range_t range);

bool adxl345_set_data_rate(
    adxl345_data_rate_t rate);

bool adxl345_start_measurement(void);

bool adxl345_stop_measurement(void);
```

---

## Initialization Sequence

```
Read Device ID

↓

Enter Standby

↓

Configure FULL_RES

↓

Configure ±2 g Range

↓

Configure 100 Hz Output Data Rate

↓

Enter Measurement Mode
```

---

## Burst Read

The driver reads all three axes using a single I²C burst transaction.

```
START

↓

0x53 + Write

↓

DATAX0

↓

Repeated START

↓

0x53 + Read

↓

DATAX0
DATAX1
DATAY0
DATAY1
DATAZ0
DATAZ1

↓

STOP
```

---

## Example

Raw acceleration:

```
x = 262
y = 7
z = 45
```

Converted acceleration:

```
x = 1.02 g
y = 0.03 g
z = 0.18 g
```

---

## Building

```
make
```

Flash

```
make flash
```

---

## Development & Debugging Tools

- arm-none-eabi-gcc
- GNU Make
- arm-none-eabi-gdb
- st-util
- ST-Link V2

---

## Development workflow

```
arm-none-eabi-gcc
        │
        ▼
     main.elf
        │
        ▼
arm-none-eabi-gdb
        │
        ▼
      st-util
        │
        ▼
     ST-Link V2
        │
        ▼
   STM32F103 (SWD)

```

---

## References

- STM32F103 Reference Manual (RM0008)
- STM32F103 Datasheet
- ADXL345 Datasheet
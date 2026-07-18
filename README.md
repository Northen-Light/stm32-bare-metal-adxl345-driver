# STM32 Bare-Metal ADXL345 Driver

A bare-metal ADXL345 accelerometer driver for the STM32F103 written in C using register-level programming.

This project interfaces an STM32F103 microcontroller with an Analog Devices ADXL345 3-axis accelerometer over I²C. It supports sensor configuration, interrupt configuration, measurement control, raw and converted acceleration readings and data-ready interrupt handling.

---

## Features

### STM32 I²C Driver

- Bare-metal I²C1 driver
- Register-level STM32F103 programming
- Single-byte register writes
- Single-byte register reads
- Multi-byte burst register reads
- Repeated START generation
- ACK/NACK handling
- Polling-based STM32 I²C state machine

### ADXL345 Driver

- Device ID verification
- Measurement range configuration
- Output data rate configuration
- Raw acceleration reads
- Acceleration conversion to `g`
- Start/Stop measurement
- Interrupt configuration
- Interrupt routing (INT1 / INT2)
- Register read-back verification
- Modular driver architecture

### Interrupt Support

- DATA_READY interrupt
- STM32 EXTI and NVIC configuration
- Interrupt-driven data acquisition

---

## Hardware Used

- STM32F103C8T6 Blue Pill
- ADXL345 accelerometer module
- ST-Link V2
- Breadboard and jumper wires

---

## Pin Connections

| STM32F103 | ADXL345 | Description |
|------------|----------|-------------|
| PB6 | SCL | I²C Clock |
| PB7 | SDA | I²C Data |
| PA0 | INT1 | Data Ready Interrupt |
| 3.3V | VCC | Power |
| GND | GND | Ground |
| 3.3V | CS | Select I²C Mode |
| GND | SDO | I²C Address = 0x53 |

---

## Project Structure

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
└── README.md
```

---

## Build

```bash
make
```

---

## Flash

```bash
make flash
```

---

## Future Improvements

- ADXL345 FIFO support
- I²C timeout and error handling
- Activity/Tap detection

---

## References

- STM32F103 Reference Manual (RM0008)
- STM32F103 Datasheet
- ADXL345 Datasheet
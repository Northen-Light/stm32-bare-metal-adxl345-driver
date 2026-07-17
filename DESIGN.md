# Learning Notes

This document summarizes the major firmware concepts explored while building this project.

---

# Driver Design

## Layered Architecture

The project separates device-specific logic from bus communication.

```
Application
      ↓
ADXL345 Driver
      ↓
I²C Driver
      ↓
STM32 Peripheral
```

Only the I²C driver depends on STM32 registers.

---

## Read-Modify-Write

Configuration registers are updated using read-modify-write to preserve unrelated bits.

Example:

- DATA_FORMAT
- BW_RATE
- INT_ENABLE
- INT_MAP

---

## Register Verification

Configuration writes are immediately read back and verified.

Benefits:

- Detect communication failures
- Simplify debugging

---

## Burst Reads

The driver reads all six data registers in a single transaction.

Advantages:

- Lower bus overhead
- Consistent X/Y/Z sample
- Reduced software complexity

---

# STM32 I²C

Topics explored:

- START condition
- STOP condition
- Repeated START
- ACK vs NACK
- RXNE
- BTF
- Clock stretching
- Single-byte receive
- Multi-byte receive
- Address phase
- Event sequencing

---

# Interrupts

Implemented:

- EXTI configuration
- AFIO interrupt routing
- NVIC configuration
- Interrupt-driven data acquisition

Concepts explored:

- Pending bits
- Write-one-to-clear registers
- volatile variables
- Interrupt latency

---

# Embedded C

Topics covered:

- Static functions
- Layered APIs
- Enumerations
- Bit masks
- Read-modify-write operations
- Register abstraction
- Defensive parameter checking

---

# Toolchain

- GCC
- Linker scripts
- Startup code
- Vector table
- ELF generation
- ST-Link
- GDB

---

# Debugging

Used throughout the project:

- GDB register inspection
- Memory inspection
- Read-back verification
- Hardware validation
- Interrupt debugging

---

# Future Improvements

- FIFO support
- FIFO watermark interrupt
- Activity / Inactivity detection
- Tap / Double-tap detection
- DMA-based I²C transfers
- Timeout handling
- Error reporting
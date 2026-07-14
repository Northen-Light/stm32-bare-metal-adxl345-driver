## Design Decisions

### Layered Driver Design

The project separates bus communication from device-specific logic.

```
Application
      ↓
ADXL345 Driver
      ↓
I²C Driver
      ↓
STM32 Peripheral
```

This allows the ADXL345 driver to be reused with another MCU by replacing only the I²C layer.

### Register Verification

Configuration writes are immediately read back and verified to detect communication failures or incorrect configuration.

### Burst Reads

The driver reads all six data registers using a single burst transaction rather than six independent register reads, reducing bus overhead and ensuring all axis samples belong to the same measurement.

## Future Improvements

- Interrupt-driven data ready
- FIFO support
- Activity/Inactivity detection
- Tap/Double-tap detection
- DMA-based I²C transfers

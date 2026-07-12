#ifndef ADXL345_H
#define ADXL345_H

#include "stdint.h"
#define ADXL345_ADDRESS  0x53U
#define ADXL345_DEVID    0x00U
#define ADXL345_REG_POWER_CTL 0x2DU

uint8_t adxl345_init(void);
uint8_t adxl345_read_device_id(void);

#endif
#ifndef ADXL345_H
#define ADXL345_H

#include <stdbool.h>
#include "stdint.h"

#define ADXL345_ADDRESS                   0x53U  
#define ADXL345_DEVID                     0xE5

#define ADXL345_REG_DEVID                 0x00U
#define ADXL345_REG_BW_RATE               0x2C
#define ADXL345_REG_POWER_CTL             0x2DU
#define ADXL345_REG_DATA_FORMAT           0x31U

#define ADXL345_REG_DATAX0                0x32U
#define ADXL345_REG_DATAX1                0x33U
#define ADXL345_REG_DATAY0                0x34U
#define ADXL345_REG_DATAY1                0x35U
#define ADXL345_REG_DATAZ0                0x36U
#define ADXL345_REG_DATAZ1                0x37U

#define ADXL345_MEASURE_BIT               (1U << 3U)

typedef struct {
  int16_t x;
  int16_t y;
  int16_t z;
} adxl345_raw_data_t;

typedef struct {
  float x;
  float y;
  float z;
} adxl345_acceleration_t;

bool adxl345_init(void);
uint8_t adxl345_read_device_id(void);
bool adxl345_read_raw(adxl345_raw_data_t *data);
bool adxl345_read_accleration(adxl345_acceleration_t *accle);

#endif
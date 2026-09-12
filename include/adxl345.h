#ifndef ADXL345_H
#define ADXL345_H

#include <stdint.h>

typedef struct {
  float x;
  float y;
  float z;
} accleration_t;

void adxl345_read_device_id(uint8_t *byte);
void adxl345_set_data_format(uint8_t data_format);
void adxl345_set_bw_rate(uint8_t bw_rate);
void adxl345_set_power_ctl(uint8_t power_ctl);
void adxl345_read_accleration(accleration_t *accleration);

#endif
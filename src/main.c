#include "i2c.h"

uint8_t device_id_reg;
uint8_t power_ctl_reg;
uint8_t data_format_reg;
uint8_t bw_rate_reg;
uint8_t raw_accl[6];

typedef struct {
  float x;
  float y;
  float z;
} accl_t;

accl_t acc;

void main(void) {
  i2c1_init();
  i2c1_slave_single_byte_read(0x1D, 0x00, &device_id_reg);
  
  i2c1_slave_single_byte_read(0x1D, 0x2C, &bw_rate_reg);

  i2c1_slave_single_byte_write(0x1D, 0x31, (1U << 3));
  i2c1_slave_single_byte_read(0x1D, 0x31, &data_format_reg);

  i2c1_slave_single_byte_write(0x1D, 0x2D, (1U << 3));
  i2c1_slave_single_byte_read(0x1D, 0x2D, &power_ctl_reg);

  i2c1_slave_multi_byte_read(0x1D, 0x32, raw_accl, 6);

  acc.x = ((int16_t)((uint16_t)(raw_accl[1] << 8) | raw_accl[0]) * 4.0f) / 1000.0f;
  acc.y = ((int16_t)((uint16_t)(raw_accl[3] << 8) | raw_accl[2]) * 4.0f) / 1000.0f;
  acc.z = ((int16_t)((uint16_t)(raw_accl[5] << 8) | raw_accl[4]) * 4.0f) / 1000.0f;

  while (1) {}
}
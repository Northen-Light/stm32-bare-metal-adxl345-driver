#include "adxl345.h"
#include "i2c.h"

static void adxl345_single_byte_read(uint8_t register_address, uint8_t *byte);
static void adxl345_single_byte_write(uint8_t register_address, uint8_t byte);
static void adxl345_multi_byte_read(uint8_t register_address, uint8_t *bytes, uint8_t length);

void adxl345_read_device_id(uint8_t *byte) {
  adxl345_single_byte_read(0x00, byte);
}

void adxl345_set_data_format(uint8_t data_format) {
  adxl345_single_byte_write(0x31, data_format);
}

void adxl345_set_bw_rate(uint8_t bw_rate) {
  adxl345_single_byte_write(0x2C, bw_rate);
}

void adxl345_set_power_ctl(uint8_t power_ctl) {
  adxl345_single_byte_write(0x2D, power_ctl);
}

void adxl345_read_accleration(accleration_t *accleration) {
  uint8_t length = 6;
  uint8_t raw_accleration[length];

  adxl345_multi_byte_read(0x32, raw_accleration, length);

  accleration -> x = ((int16_t)((uint16_t)(raw_accleration[1] << 8) | raw_accleration[0]) * 4.0f) / 1000.0f;
  accleration -> y = ((int16_t)((uint16_t)(raw_accleration[3] << 8) | raw_accleration[2]) * 4.0f) / 1000.0f;
  accleration -> z = ((int16_t)((uint16_t)(raw_accleration[5] << 8) | raw_accleration[4]) * 4.0f) / 1000.0f;
}

static void adxl345_single_byte_read(uint8_t register_address, uint8_t *byte) {
  i2c1_slave_single_byte_read(0x1D, register_address, byte);
}

static void adxl345_single_byte_write(uint8_t register_address, uint8_t byte) {
  i2c1_slave_single_byte_write(0x1D, register_address, byte);  
}

static void adxl345_multi_byte_read(uint8_t register_address, uint8_t *bytes, uint8_t length) {
  i2c1_slave_multi_byte_read(0x1D, register_address, bytes, length);  
}
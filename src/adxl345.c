#include "adxl345.h"
#include "adxl345_internal.h"
#include "i2c.h"

static void adxl345_single_byte_read(uint8_t register_address, uint8_t *byte);
static void adxl345_single_byte_write(uint8_t register_address, uint8_t byte);
static void adxl345_multi_byte_read(uint8_t register_address, uint8_t *bytes, uint8_t length);

void adxl345_read_device_id(uint8_t *byte) {
  adxl345_single_byte_read(ADXL345_REGISTER_DEVID_ID, byte);
}

void adxl345_set_data_format(uint8_t data_format) {
  adxl345_single_byte_write(ADXL345_REGISTER_DATA_FORMAT, data_format);
}

void adxl345_set_bw_rate(uint8_t bw_rate) {
  adxl345_single_byte_write(ADXL345_REGISTER_BW_RATE, bw_rate);
}

void adxl345_set_power_ctl(uint8_t power_ctl) {
  adxl345_single_byte_write(ADXL345_REGISTER_POWER_CTL, power_ctl);
}

void adxl345_read_acceleration(acceleration_t *acceleration) {
  uint8_t raw_acceleration[6];
  uint8_t length = 6;
  float scale_factor = ADXL345_SCALE_FACTOR_FULL_RES;
  uint8_t data_format;
  uint8_t range_bits;

  adxl345_multi_byte_read(ADXL345_REGISTER_DATAX0, raw_acceleration, length);

  adxl345_single_byte_read(ADXL345_REGISTER_DATA_FORMAT, &data_format);

  if ((data_format & ADXL345_REGISTER_DATA_FORMAT_FULL_RES_BIT) == 0) {
    range_bits = data_format & ADXL345_REGISTER_DATA_FORMAT_RANGE_BITS_MASK;

    switch (range_bits) {
      case 0 : 
        scale_factor = ADXL345_SCALE_FACTOR_2G_10BIT_RES;
        break;
      case 1 : 
        scale_factor = ADXL345_SCALE_FACTOR_4G_10BIT_RES;
        break;
      case 2 : 
        scale_factor = ADXL345_SCALE_FACTOR_8G_10BIT_RES;
        break;
      case 3 : 
        scale_factor = ADXL345_SCALE_FACTOR_16G_10BIT_RES;
        break;
    }
  }
  

  acceleration -> x = ((int16_t)((uint16_t)(raw_acceleration[1] << 8) | raw_acceleration[0]) * scale_factor) / 1000.0f;
  acceleration -> y = ((int16_t)((uint16_t)(raw_acceleration[3] << 8) | raw_acceleration[2]) * scale_factor) / 1000.0f;
  acceleration -> z = ((int16_t)((uint16_t)(raw_acceleration[5] << 8) | raw_acceleration[4]) * scale_factor) / 1000.0f;
}

static void adxl345_single_byte_read(uint8_t register_address, uint8_t *byte) {
  i2c1_slave_single_byte_read(ADXL345_7BIT_I2C_ADDRESS, register_address, byte);
}

static void adxl345_single_byte_write(uint8_t register_address, uint8_t byte) {
  i2c1_slave_single_byte_write(ADXL345_7BIT_I2C_ADDRESS, register_address, byte);  
}

static void adxl345_multi_byte_read(uint8_t register_address, uint8_t *bytes, uint8_t length) {
  i2c1_slave_multi_byte_read(ADXL345_7BIT_I2C_ADDRESS, register_address, bytes, length);  
}
#include "adxl345.h"
#include "adxl345_internal.h"
#include "i2c.h"

extern uint8_t ADXL345_7BIT_I2C_ADDRESS;

float scale_factor = ADXL345_SCALE_FACTOR_FULL_RES;

static adxl345_status_t adxl345_single_byte_read(uint8_t register_address, uint8_t *byte);
static adxl345_status_t adxl345_single_byte_write(uint8_t register_address, uint8_t byte);
static adxl345_status_t adxl345_multi_byte_read(uint8_t register_address, uint8_t *bytes, uint8_t length);
static adxl345_status_t adxl345_convert_i2c_to_adxl345_status(i2c_status_t status);


adxl345_status_t adxl345_read_device_id(uint8_t *byte) {
  return adxl345_single_byte_read(ADXL345_REGISTER_DEVID_ID, byte);
}

adxl345_status_t adxl345_set_data_format(uint8_t data_format) {
  uint8_t range_bits;
  uint8_t data_format_value;
  adxl345_status_t status;

  status = adxl345_single_byte_write(ADXL345_REGISTER_DATA_FORMAT, data_format);
  if (status != ADXL345_STATUS_OK) return status;

  status = adxl345_single_byte_read(ADXL345_REGISTER_DATA_FORMAT, &data_format_value);
  if (status != ADXL345_STATUS_OK) return status;

  if ((data_format_value & ADXL345_REGISTER_DATA_FORMAT_FULL_RES_BIT) == 0) {
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
  } else {
    scale_factor = ADXL345_SCALE_FACTOR_FULL_RES;
  }

  return status;
}

adxl345_status_t adxl345_set_bw_rate(uint8_t bw_rate) {
  return adxl345_single_byte_write(ADXL345_REGISTER_BW_RATE, bw_rate);
}

adxl345_status_t adxl345_set_power_ctl(uint8_t power_ctl) {
  return adxl345_single_byte_write(ADXL345_REGISTER_POWER_CTL, power_ctl);
}

adxl345_status_t adxl345_read_acceleration(acceleration_t *acceleration) {
  uint8_t raw_acceleration[6];
  uint8_t length = 6;
  adxl345_status_t status;
 
  status = adxl345_multi_byte_read(ADXL345_REGISTER_DATAX0, raw_acceleration, length);
  if (status != ADXL345_STATUS_OK) return status;

  acceleration -> x = ((int16_t)((uint16_t)(raw_acceleration[1] << 8) | raw_acceleration[0]) * scale_factor) / 1000.0f;
  acceleration -> y = ((int16_t)((uint16_t)(raw_acceleration[3] << 8) | raw_acceleration[2]) * scale_factor) / 1000.0f;
  acceleration -> z = ((int16_t)((uint16_t)(raw_acceleration[5] << 8) | raw_acceleration[4]) * scale_factor) / 1000.0f;

  return status;
}

static adxl345_status_t adxl345_single_byte_read(uint8_t register_address, uint8_t *byte) {
  i2c_status_t status = i2c1_slave_single_byte_read(ADXL345_7BIT_I2C_ADDRESS, register_address, byte);
  return adxl345_convert_i2c_to_adxl345_status(status);
}

static adxl345_status_t adxl345_single_byte_write(uint8_t register_address, uint8_t byte) {
  i2c_status_t status = i2c1_slave_single_byte_write(ADXL345_7BIT_I2C_ADDRESS, register_address, byte); 
  return adxl345_convert_i2c_to_adxl345_status(status);
}

static adxl345_status_t adxl345_multi_byte_read(uint8_t register_address, uint8_t *bytes, uint8_t length) {
  i2c_status_t status = i2c1_slave_multi_byte_read(ADXL345_7BIT_I2C_ADDRESS, register_address, bytes, length); 
  return adxl345_convert_i2c_to_adxl345_status(status);
}

static adxl345_status_t adxl345_convert_i2c_to_adxl345_status(i2c_status_t status) {
  switch (status) {
    case I2C_STATUS_OK:
      return ADXL345_STATUS_OK;
    case I2C_STATUS_BUS_ERROR :
      return ADXL345_STATUS_I2C_BUS_ERROR;
    case I2C_STATUS_ARBITRATION_LOST : 
      return ADXL345_STATUS_I2C_ARBITRATION_LOST;
    case I2C_STATUS_NACK :
      return ADXL345_STATUS_I2C_NACK;
    case I2C_STATUS_TIMEOUT : 
      return ADXL345_STATUS_I2C_NACK;
    case I2C_STATUS_INVALID_ARGUMENT :
      return ADXL345_STATUS_I2C_INVALID_ARGUMENT;
    case I2C_STATUS_RECOVERY_FAILED : 
      return ADXL345_STATUS_I2C_RECOVERY_FAILED;
  }

  return ADXL345_STATUS_UNKNOWN;
}
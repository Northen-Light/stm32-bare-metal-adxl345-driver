#include <stddef.h>
#include "adxl345.h"
#include "i2c.h"

static uint8_t adxl345_read_register(uint8_t register_address);
static void adxl345_write_register(uint8_t register_address, uint8_t value);
static void adxl345_read_registers(uint8_t register_address, uint8_t *buffer, uint32_t length);

bool adxl345_init(void) {
  uint8_t device_id = adxl345_read_device_id();

  if (device_id != ADXL345_DEVID) {
    return false;
  }

  adxl345_write_register(ADXL345_REG_POWER_CTL, 0x00U);
  adxl345_write_register(ADXL345_REG_DATA_FORMAT, (1U << 3U));
  adxl345_write_register(ADXL345_REG_BW_RATE, 0x0AU);
  adxl345_write_register(ADXL345_REG_POWER_CTL, ADXL345_MEASURE_BIT);

  return true;
}

uint8_t adxl345_read_device_id(void) {
  return adxl345_read_register(ADXL345_REG_DEVID);
}

bool adxl345_read_raw(adxl345_raw_data_t *data) {
  uint8_t buffer[6];

  if (data == NULL) {
    return false;
  }

  adxl345_read_registers(ADXL345_REG_DATAX0, buffer, 6U);
  data -> x = (int16_t)((uint8_t)buffer[1] << 8 | buffer[0]);
  data -> y = (int16_t)((uint8_t)buffer[3] << 8 | buffer[2]);
  data -> z = (int16_t)((uint8_t)buffer[5] << 8 | buffer[4]);

  return true;
}

bool adxl345_read_accleration(adxl345_acceleration_t *accle) {
  adxl345_raw_data_t raw;

  if (accle == 0) {
    return false;
  }

  if (!adxl345_read_raw(&raw)) {
    return false;
  }

  accle -> x = raw.x * 0.0039f;
  accle -> y = raw.y * 0.0039f;
  accle -> z = raw.z * 0.0039f;

  return true;
}

static uint8_t adxl345_read_register(uint8_t register_address) {
  return i2c1_read_regsiter(ADXL345_ADDRESS, register_address);
}

static void adxl345_write_register(uint8_t register_address, uint8_t value) {
  i2c1_write_register(ADXL345_ADDRESS, register_address, value);
}

static void adxl345_read_registers(uint8_t register_address, uint8_t *buffer, uint32_t length) {
  i2c1_read_registers(ADXL345_ADDRESS, register_address, buffer, length);
}
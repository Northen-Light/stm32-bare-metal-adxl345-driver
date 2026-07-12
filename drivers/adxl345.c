#include "adxl345.h"
#include "i2c.h"

static uint8_t adxl345_read_register(uint8_t register_address);
static void adxl345_write_register(uint8_t register_address, uint8_t value);

uint8_t adxl345_init(void) {
  uint8_t device_id = adxl345_read_device_id();
  if (device_id != ADXL345_DEVID) {
    return 1;
  }

  return 0;
}

uint8_t adxl345_read_device_id(void) {
  return adxl345_read_register(ADXL345_DEVID);
}

static uint8_t adxl345_read_register(uint8_t register_address) {
  return i2c1_read_regsiter(ADXL345_ADDRESS, register_address);
}

static void adxl345_write_register(uint8_t register_address, uint8_t value) {
  i2c1_write_register(ADXL345_ADDRESS, register_address, value);
}
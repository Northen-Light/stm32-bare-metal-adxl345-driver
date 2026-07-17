#include <stddef.h>
#include "adxl345.h"
#include "i2c.h"

static uint8_t adxl345_read_register(uint8_t register_address);
static void adxl345_write_register(uint8_t register_address, uint8_t value);
static void adxl345_read_registers(uint8_t register_address, uint8_t *buffer, uint32_t length);
static bool verify_register_value(uint8_t register_address, uint8_t expected_value);

bool adxl345_init(void) {
  uint8_t data_format_reg;
  uint8_t device_id;

  device_id = adxl345_read_device_id();
  if (device_id != ADXL345_DEVID) 
    return false;

  adxl345_write_register(ADXL345_REG_POWER_CTL, ADXL345_POWER_CTL_RESET);
  
  if (!verify_register_value(ADXL345_REG_POWER_CTL, ADXL345_POWER_CTL_RESET))
    return false;

  data_format_reg = ADXL345_FULL_RES_BIT | ADXL345_RANGE_2G;
  adxl345_write_register(ADXL345_REG_DATA_FORMAT, data_format_reg);

  if (!verify_register_value(ADXL345_REG_DATA_FORMAT, data_format_reg))
    return false;

  adxl345_write_register(ADXL345_REG_DATA_RATE, ADXL345_DATA_RATE_100Hz);

  if (!verify_register_value(ADXL345_REG_DATA_RATE, ADXL345_DATA_RATE_100Hz))
    return false;

  return true;
}

uint8_t adxl345_read_device_id(void) {
  return adxl345_read_register(ADXL345_REG_DEVID);
}

bool adxl345_read_raw(adxl345_raw_data_t *data) {
  uint8_t buffer[6];

  if (data == NULL) 
    return false;

  adxl345_read_registers(ADXL345_REG_DATAX0, buffer, 6U);
  data -> x = (int16_t)((uint16_t)buffer[1] << 8 | buffer[0]);
  data -> y = (int16_t)((uint16_t)buffer[3] << 8 | buffer[2]);
  data -> z = (int16_t)((uint16_t)buffer[5] << 8 | buffer[4]);

  return true;
}

bool adxl345_read_acceleration(adxl345_acceleration_t *accele) {
  adxl345_raw_data_t raw;

  if (accele == 0)
    return false;

  if (!adxl345_read_raw(&raw)) 
    return false;

  accele -> x = raw.x * ADXL345_SCALE_FACTOR;
  accele -> y = raw.y * ADXL345_SCALE_FACTOR;
  accele -> z = raw.z * ADXL345_SCALE_FACTOR;

  return true;
}

bool adxl345_start_measurement(void) {
  uint8_t power_ctl_reg;

  power_ctl_reg = adxl345_read_register(ADXL345_REG_POWER_CTL);
  power_ctl_reg |= ADXL345_MEASURE_BIT;
  adxl345_write_register(ADXL345_REG_POWER_CTL, power_ctl_reg);

  if (!verify_register_value(ADXL345_REG_POWER_CTL, power_ctl_reg))
    return false;

  return true;
}

bool adxl345_stop_measurement(void) {
  uint8_t power_ctl_reg;

  power_ctl_reg = adxl345_read_register(ADXL345_REG_POWER_CTL);
  power_ctl_reg &= ~ADXL345_MEASURE_BIT;
  adxl345_write_register(ADXL345_REG_POWER_CTL, power_ctl_reg);

  if (!verify_register_value(ADXL345_REG_POWER_CTL, power_ctl_reg))
    return false;

  return true;
}

bool adxl345_set_range(adxl345_range_t range) {
  uint8_t data_format_reg;

  if (range > ADXL345_RANGE_16G)
    return false;

  data_format_reg = adxl345_read_register(ADXL345_REG_DATA_FORMAT);
  data_format_reg &= ~ADXL345_RANGE_MASK;
  data_format_reg |= range;
  data_format_reg &= ~ADXL345_DATA_FORMAT_RESERVED_BIT;
  adxl345_write_register(ADXL345_REG_DATA_FORMAT, data_format_reg);

  if (!verify_register_value(ADXL345_REG_DATA_FORMAT, data_format_reg)) 
    return false;

  return true;
}

bool adxl345_set_data_rate(adxl345_data_rate_t data_rate) {
  uint8_t data_rate_reg;

  if (data_rate > ADXL345_DATA_RATE_3200Hz)
    return false;
  
  data_rate_reg = adxl345_read_register(ADXL345_REG_DATA_RATE);
  data_rate_reg &= ~ADXL345_DATA_RATE_MASK;
  data_rate_reg |= data_rate;
  data_rate_reg &= ~ADXL345_DATA_RATE_RESERVED_MASK;
  adxl345_write_register(ADXL345_REG_DATA_RATE, data_rate_reg);

  if (!verify_register_value(ADXL345_REG_DATA_RATE, data_rate_reg)) 
    return false;
  
  return true;
}

bool adxl345_set_interrupt_bit(adxl345_interrupt_bit_t interrupt_bit, adxl345_interrupt_channel_t interrupt_channel) {
  uint8_t interrupt_enable_reg = adxl345_read_register(ADXL345_REG_INTERRUPT_ENABLE);
  uint8_t interrupt_map_reg = adxl345_read_register(ADXL345_REG_INTERRUPT_MAP);

  if (interrupt_channel == ADXL345_INT1)
    interrupt_map_reg &= ~interrupt_bit;
  else if (interrupt_channel == ADXL345_INT2)
    interrupt_map_reg |= interrupt_bit;
  else 
    return false;

  adxl345_write_register(ADXL345_REG_INTERRUPT_MAP, interrupt_map_reg);

  if (!verify_register_value(ADXL345_REG_INTERRUPT_MAP, interrupt_map_reg))
    return false;
  
  interrupt_enable_reg |= interrupt_bit;

  adxl345_write_register(ADXL345_REG_INTERRUPT_ENABLE, interrupt_enable_reg);

  if (!verify_register_value(ADXL345_REG_INTERRUPT_ENABLE, interrupt_enable_reg))
    return false; 

  return true;
}

static uint8_t adxl345_read_register(uint8_t register_address) {
  return i2c1_read_register(ADXL345_ADDRESS, register_address);
}

static void adxl345_write_register(uint8_t register_address, uint8_t value) {
  i2c1_write_register(ADXL345_ADDRESS, register_address, value);
}

static void adxl345_read_registers(uint8_t register_address, uint8_t *buffer, uint32_t length) {
  i2c1_read_registers(ADXL345_ADDRESS, register_address, buffer, length);
}

static bool verify_register_value(uint8_t register_address, uint8_t expected_value) {
  uint8_t register_value = adxl345_read_register(register_address);
  
  if (register_value != expected_value)
    return false;

  return true;
}
#include "i2c.h"
#include "adxl345.h"

uint8_t device_id;
acceleration_t acceleration;

void main(void) {
  i2c1_init();

  adxl345_read_device_id(&device_id);
  adxl345_set_bw_rate(ADXL345_REGISTER_BW_RATE_OUTPUT_DATA_RATE_200HZ);
  adxl345_set_data_format(ADXL345_REGISTER_DATA_FORMAT_FULL_RES_BIT | ADXL345_REGISTER_DATA_FORMAT_RANGE_BITS_2G);
  adxl345_set_power_ctl(ADXL345_REGISTER_POWER_CTL_MEASURE_BIT);
  adxl345_read_acceleration(&acceleration);

  while (1) {}
}
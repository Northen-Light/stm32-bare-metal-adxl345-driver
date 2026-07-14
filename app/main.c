#include "i2c.h"
#include "adxl345.h"

adxl345_acceleration_t acceleration;

int main(void) {
  i2c1_init();

  adxl345_init();

  adxl345_stop_measurement();
  adxl345_set_range(ADXL345_RANGE_4G);
  adxl345_set_data_rate(ADXL345_DATA_RATE_200Hz);
  adxl345_start_measurement();
  
  adxl345_read_acceleration(&acceleration);
  
  while (1);
  return 0;
}
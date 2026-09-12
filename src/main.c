#include "i2c.h"
#include "adxl345.h"

uint8_t device_id;
accleration_t accleration;

void main(void) {
  i2c1_init();

  adxl345_read_device_id(&device_id);
  adxl345_set_data_format((1U << 3));
  adxl345_set_power_ctl((1U << 3));
  adxl345_read_accleration(&accleration);

  while (1) {}
}
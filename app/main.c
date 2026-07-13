#include "i2c.h"
#include "adxl345.h"

adxl345_acceleration_t accleration;

int main(void) {
  i2c1_init();
  adxl345_init();
  adxl345_read_accleration(&accleration);
  while (1);
  return 0;
}
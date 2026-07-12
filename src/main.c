#include "i2c.h"
#include "adxl345.h"

int main(void) {
  i2c1_init();
  adxl345_init();
  while (1);
  return 0;
}
#include "i2c.h"

uint8_t byte;

void main(void) {
  i2c1_init();
  i2c1_single_byte_slave_read(0x1D, 0x00, &byte);

  while (1) {}
}
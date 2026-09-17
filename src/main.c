#include "i2c.h"
#include "adxl345.h"
#include "adxl345_internal.h"

uint8_t device_id;
acceleration_t acceleration;
adxl345_status_t status;

void main(void) {
  i2c1_init();

  status = adxl345_read_device_id(&device_id);

  if (status == ADXL345_STATUS_OK) { 
    status =  adxl345_set_bw_rate(ADXL345_REGISTER_BW_RATE_OUTPUT_DATA_RATE_200HZ);
    
    if (status == ADXL345_STATUS_OK) {
      status = adxl345_set_data_format(ADXL345_REGISTER_DATA_FORMAT_FULL_RES_BIT | ADXL345_REGISTER_DATA_FORMAT_RANGE_BITS_2G);
      
      if (status == ADXL345_STATUS_OK) {
        status = adxl345_set_power_ctl(ADXL345_REGISTER_POWER_CTL_MEASURE_BIT);

        if (status == ADXL345_STATUS_OK) {
          adxl345_read_acceleration(&acceleration);
        }
      }
    }
  }

  while (1) {}
}
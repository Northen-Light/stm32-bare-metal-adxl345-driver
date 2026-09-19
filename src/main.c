#include "i2c.h"
#include "adxl345.h"
#include "exti.h"

uint8_t device_id;
acceleration_t acceleration;
adxl345_status_t status = ADXL345_STATUS_UNKNOWN;

void data_ready_interrupt_callback(void);
uint32_t data_ready_counts = 0;

void main(void) {
  i2c1_init();
  exti_init();

  status = adxl345_read_register_device_id(&device_id);

  if (status == ADXL345_STATUS_OK) { 
    status = adxl345_setup_interrupt(
      ADXL345_REGISTER_INT_ENABLE_DATA_READY, 
      (uint8_t)~ADXL345_REGISTER_INT_MAP_DATA_READY, 
      data_ready_interrupt_callback
    );

    if (status == ADXL345_STATUS_OK) {
      status =  adxl345_set_register_bandwidth_rate(ADXL345_REGISTER_BW_RATE_OUTPUT_DATA_RATE_200HZ);
      
      if (status == ADXL345_STATUS_OK) {
        status = adxl345_set_register_data_format(
          ADXL345_REGISTER_DATA_FORMAT_FULL_RES_BIT | 
          ADXL345_REGISTER_DATA_FORMAT_RANGE_BITS_2G |
          ADXL345_REGISTER_DATA_FORMAT_SELF_TEST_BIT
        );
        
        if (status == ADXL345_STATUS_OK) {
          status = adxl345_set_register_power_control(ADXL345_REGISTER_POWER_CTL_MEASURE_BIT);

          if (status == ADXL345_STATUS_OK) {
            adxl345_read_acceleration(&acceleration);
          }
        }
      }
    }
  }

  while (1) {}
}

void data_ready_interrupt_callback(void) {
  if (status == ADXL345_STATUS_OK) {
    status = adxl345_read_acceleration(&acceleration);
    data_ready_counts++;
  }
}
#include "i2c.h"
#include "adxl345.h"
#include "exti.h"

uint8_t device_id;
acceleration_t acceleration;
adxl345_status_t status = ADXL345_STATUS_UNKNOWN;

void data_ready_interrupt_callback(void);
uint32_t samples_captured = 0;
volatile bool data_ready = false;

void main(void) {
  i2c1_init();
  exti_init();

  status = adxl345_read_device_id_register(&device_id);

  if (status == ADXL345_STATUS_OK) { 
    status = adxl345_setup_interrupt(
      ADXL345_REGISTER_INT_ENABLE_DATA_READY, 
      (uint8_t)~ADXL345_REGISTER_INT_MAP_DATA_READY, 
      data_ready_interrupt_callback
    );

    if (status == ADXL345_STATUS_OK) {
      status =  adxl345_set_bandwidth_rate_register(ADXL345_REGISTER_BW_RATE_OUTPUT_DATA_RATE_200HZ);
      
      if (status == ADXL345_STATUS_OK) {
        status = adxl345_set_data_format_register(
          ADXL345_REGISTER_DATA_FORMAT_FULL_RES_BIT | 
          ADXL345_REGISTER_DATA_FORMAT_RANGE_BITS_2G
        );
        
        if (status == ADXL345_STATUS_OK) {
          status = adxl345_set_power_control_register(ADXL345_REGISTER_POWER_CTL_MEASURE_BIT);

          if (status == ADXL345_STATUS_OK) {
            status = adxl345_read_acceleration(&acceleration);

            if (status == ADXL345_STATUS_OK) {
              samples_captured++;

              while (1) {
                if (data_ready) {
                  data_ready = false;
                  status = adxl345_read_acceleration(&acceleration);
                  
                  if (status == ADXL345_STATUS_OK) {
                    samples_captured++;
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  while (1) {}
}

void data_ready_interrupt_callback(void) {
  data_ready = true;
}
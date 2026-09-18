#ifndef ADXL345_H
#define ADXL345_H

#include <stdint.h>

typedef struct {
  float x;
  float y;
  float z;
} acceleration_t;

typedef enum {
  ADXL345_STATUS_OK,
  ADXL345_STATUS_I2C_BUS_ERROR,
  ADXL345_STATUS_I2C_ARBITRATION_LOST,
  ADXL345_STATUS_I2C_NACK,
  ADXL345_STATUS_I2C_INVALID_ARGUMENT,
  ADXL345_STATUS_I2C_TIMEOUT,
  ADXL345_STATUS_I2C_RECOVERY_FAILED,
  ADXL345_STATUS_UNKNOWN,
} adxl345_status_t;

typedef void (*adxl345_interrupt_callback_t)(void);

adxl345_status_t adxl345_read_register_device_id(uint8_t *byte);
adxl345_status_t adxl345_set_register_data_format(uint8_t data_format);
adxl345_status_t adxl345_set_register_bandwidth_rate(uint8_t bw_rate);
adxl345_status_t adxl345_set_register_power_control(uint8_t power_ctl);
adxl345_status_t adxl345_read_acceleration(acceleration_t *acceleration);
adxl345_status_t adxl345_setup_interrupt(uint8_t interrupt_enable, uint8_t interrupt_map, adxl345_interrupt_callback_t callback);
adxl345_status_t adxl345_read_register_interrupt_source(void);

#define ADXL345_REGISTER_DATA_FORMAT_RANGE_BITS_2G                   (0x0U << 0)
#define ADXL345_REGISTER_DATA_FORMAT_RANGE_BITS_4G                   (0x1U << 0)
#define ADXL345_REGISTER_DATA_FORMAT_RANGE_BITS_8G                   (0x2U << 0)
#define ADXL345_REGISTER_DATA_FORMAT_RANGE_BITS_16G                  (0x3U << 0)
#define ADXL345_REGISTER_DATA_FORMAT_FULL_RES_BIT                    (1U << 3)
#define ADXL345_REGISTER_DATA_FORMAT_INT_INVERT_BIT                  (1U << 5)
#define ADXL345_REGISTER_DATA_FORMAT_SELF_TEST_BIT                   (1U << 7)

#define ADXL345_REGISTER_POWER_CTL_WAKEUP_BITS_8HZ                   (0x0U << 0)
#define ADXL345_REGISTER_POWER_CTL_WAKEUP_BITS_4HZ                   (0x1U << 0)
#define ADXL345_REGISTER_POWER_CTL_WAKEUP_BITS_2HZ                   (0x2U << 0)
#define ADXL345_REGISTER_POWER_CTL_WAKEUP_BITS_1HZ                   (0x3U << 0)
#define ADXL345_REGISTER_POWER_CTL_SLEEP_BIT                         (1U << 2)
#define ADXL345_REGISTER_POWER_CTL_MEASURE_BIT                       (1U << 3)
#define ADXL345_REGISTER_POWER_CTL_AUTOSLEEP_BIT                     (1U << 4)
#define ADXL345_REGISTER_POWER_CTL_LINK_BIT                          (1U << 5)

#define ADXL345_REGISTER_BW_RATE_LOW_POWER_BIT                       (1U << 4)
#define ADXL345_REGISTER_BW_RATE_OUTPUT_DATA_RATE_200HZ              (0xBU << 0)
#define ADXL345_REGISTER_BW_RATE_OUTPUT_DATA_RATE_100HZ              (0xAU << 0)
#define ADXL345_REGISTER_BW_RATE_OUTPUT_DATA_RATE_50HZ               (0x9U << 0)
#define ADXL345_REGISTER_BW_RATE_OUTPUT_DATA_RATE_25HZ               (0x8U << 0)
#define ADXL345_REGISTER_BW_RATE_OUTPUT_DATA_RATE_12_5HZ             (0x7U << 0)
#define ADXL345_REGISTER_BW_RATE_OUTPUT_DATA_RATE_6_25HZ             (0x6U << 0)
#define ADXL345_REGISTER_BW_RATE_OUTPUT_DATA_RATE_3_13HZ             (0x5U << 0)
#define ADXL345_REGISTER_BW_RATE_OUTPUT_DATA_RATE_1_56HZ             (0x4U << 0)
#define ADXL345_REGISTER_BW_RATE_OUTPUT_DATA_RATE_0_78HZ             (0x3U << 0)
#define ADXL345_REGISTER_BW_RATE_OUTPUT_DATA_RATE_0_39HZ             (0x2U << 0)
#define ADXL345_REGISTER_BW_RATE_OUTPUT_DATA_RATE_0_20HZ             (0x1U << 0)
#define ADXL345_REGISTER_BW_RATE_OUTPUT_DATA_RATE_0_10HZ             (0x0U << 0)

#define ADXL345_REGISTER_INT_ENABLE_DATA_READY                       (0x1U << 7)

#define ADXL345_REGISTER_INT_MAP_DATA_READY                          (0x1U << 7)

#define ADXL345_REGISTER_INT_SOURCE_DATA_READY                       (0x1U << 7)

#endif
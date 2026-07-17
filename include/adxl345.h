#ifndef ADXL345_H
#define ADXL345_H

#include <stdbool.h>
#include <stdint.h>

#define ADXL345_ADDRESS                     0x53U  
#define ADXL345_DEVID                       0xE5

#define ADXL345_REG_DEVID                   0x00U
#define ADXL345_REG_DATA_RATE               0x2CU
#define ADXL345_REG_POWER_CTL               0x2DU
#define ADXL345_REG_INTERRUPT_ENABLE        0x2EU
#define ADXL345_REG_INTERRUPT_MAP           0x2FU
#define ADXL345_REG_INTERRUPT_SOURCE        0x30U
#define ADXL345_REG_DATA_FORMAT             0x31U

#define ADXL345_REG_DATAX0                  0x32U
#define ADXL345_REG_DATAX1                  0x33U
#define ADXL345_REG_DATAY0                  0x34U
#define ADXL345_REG_DATAY1                  0x35U
#define ADXL345_REG_DATAZ0                  0x36U
#define ADXL345_REG_DATAZ1                  0x37U

#define ADXL345_MEASURE_BIT                 (1U << 3U)
#define ADXL345_FULL_RES_BIT                (1U << 3U)
#define ADXL345_DATA_FORMAT_RESERVED_BIT    (1U << 4U)

#define ADXL345_POWER_CTL_RESET             0x00U    

#define ADXL345_RANGE_MASK                  0x03U
#define ADXL345_DATA_RATE_MASK              0x0FU
#define ADXL345_FULL_RES_MASK               (1U << 3U)
#define ADXL345_DATA_RATE_RESERVED_MASK     ((1U << 6U) | (1U << 7U))

#define ADXL345_SCALE_FACTOR                0.0039f

typedef struct {
  int16_t x;
  int16_t y;
  int16_t z;
} adxl345_raw_data_t;

typedef struct {
  float x;
  float y;
  float z;
} adxl345_acceleration_t;

typedef enum {
  ADXL345_RANGE_2G                  = 0x00,
  ADXL345_RANGE_4G                  = 0x01,
  ADXL345_RANGE_8G                  = 0x02,
  ADXL345_RANGE_16G                 = 0x03
} adxl345_range_t;

typedef enum {
  ADXL345_DATA_RATE_25Hz            = 0x08,
  ADXL345_DATA_RATE_50Hz            = 0x09,
  ADXL345_DATA_RATE_100Hz           = 0x0A,
  ADXL345_DATA_RATE_200Hz           = 0x0B,
  ADXL345_DATA_RATE_400Hz           = 0x0C,
  ADXL345_DATA_RATE_800Hz           = 0x0D,
  ADXL345_DATA_RATE_1600Hz          = 0x0E,
  ADXL345_DATA_RATE_3200Hz          = 0x0F,
} adxl345_data_rate_t;

typedef enum {
  ADXL345_INTERRUPT_BIT_OVERRUN     = (1U << 0U),
  ADXL345_INTERRUPT_BIT_WATERMARK   = (1U << 1U),
  ADXL345_INTERRUPT_BIT_FREE_FALL   = (1U << 2U),
  ADXL345_INTERRUPT_BIT_INACTIVITY  = (1U << 3U),
  ADXL345_INTERRUPT_BIT_ACTIVITY    = (1U << 4U),
  ADXL345_INTERRUPT_BIT_DOUBLE_TAP  = (1U << 5U),
  ADXL345_INTERRUPT_BIT_SINGLE_TAP  = (1U << 6U),
  ADXL345_INTERRUPT_BIT_DATA_READY  = (1U << 7U),
} adxl345_interrupt_bit_t;

typedef enum {
  ADXL345_INT1                      = 0x0,
  ADXL345_INT2                      = 0x1
} adxl345_interrupt_channel_t;

bool adxl345_init(void);
uint8_t adxl345_read_device_id(void);
bool adxl345_read_raw(adxl345_raw_data_t *data);
bool adxl345_read_acceleration(adxl345_acceleration_t *accele);
bool adxl345_start_measurement(void);
bool adxl345_stop_measurement(void);
bool adxl345_set_range(adxl345_range_t range);
bool adxl345_set_data_rate(adxl345_data_rate_t rate);
bool adxl345_set_interrupt_bit(adxl345_interrupt_bit_t interrupt_bit, adxl345_interrupt_channel_t interrupt_channel);

#endif
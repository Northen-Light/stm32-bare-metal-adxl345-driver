#ifndef ADXL345_INTERNAL_H
#define ADXL345_INTERNAL_H

#define ADXL345_7BIT_I2C_ADDRESS                          0x1D                              
#define ADXL345_REGISTER_DEVID_ID                         0x00
#define ADXL345_REGISTER_DATA_FORMAT                      0x31
#define ADXL345_REGISTER_BW_RATE                          0x2C
#define ADXL345_REGISTER_POWER_CTL                        0x2D
#define ADXL345_REGISTER_DATAX0                           0x32

#define ADXL345_REGISTER_DATA_FORMAT_RANGE_BITS_MASK      (0x3U << 0)

#define ADXL345_SCALE_FACTOR_FULL_RES                     3.9f
#define ADXL345_SCALE_FACTOR_2G_10BIT_RES                 3.9f
#define ADXL345_SCALE_FACTOR_4G_10BIT_RES                 7.8f
#define ADXL345_SCALE_FACTOR_8G_10BIT_RES                 15.6f
#define ADXL345_SCALE_FACTOR_16G_10BIT_RES                31.2f

#endif
#ifndef BME280_H
#define BME280_H

#include "stm32u0xx_hal.h"

#define BME280_I2C_ADDRESS		0x76
#define BME280_CHIP_ID_REG		0xD0
#define BME280_CHIP_ID		0x60

HAL_StatusTypeDef BME280_Init(void);

#endif

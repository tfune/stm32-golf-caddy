#ifndef BME280_H
#define BME280_H

#include "stm32u0xx_hal.h"

#define BME280_I2C_ADDRESS		0x76

#define BME280_CHIP_ID_REG		0xD0
#define BME280_CHIP_ID			0x60
#define BME280_CTRL_HUM_REG		0xF2
#define BME280_CTRL_MEAS_REG	0xF4
#define BME280_CONFIG_REG		0xF5
#define BME280_DATA_REG			0xF7
#define BME280_CALIB00_REG		0x88

HAL_StatusTypeDef BME280_Init(void);
HAL_StatusTypeDef BME280_ReadRawData(void);
HAL_StatusTypeDef BME280_ReadCalibrationData(void);
HAL_StatusTypeDef BME280_CompensateTemperature(void);

#endif

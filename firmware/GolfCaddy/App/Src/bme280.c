#include "bme280.h"
#include "i2c.h"

static HAL_StatusTypeDef VerifyChipID(void);

HAL_StatusTypeDef BME280_Init(void)
{
	if (VerifyChipID() != HAL_OK)
	{
		return HAL_ERROR;
	}

	return HAL_OK;
}

static HAL_StatusTypeDef VerifyChipID(void)
{
	uint8_t chipID;

	if (HAL_I2C_Mem_Read(
		&hi2c1,
		BME280_I2C_ADDRESS << 1,
		BME280_CHIP_ID_REG,
		I2C_MEMADD_SIZE_8BIT,
		&chipID,
		1,
		100) != HAL_OK)
	{
		return HAL_ERROR;
	}

	if (chipID != BME280_CHIP_ID)
	{
		return HAL_ERROR;
	}

	return HAL_OK;
}

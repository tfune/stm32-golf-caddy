#include "bme280.h"
#include "i2c.h"

static uint8_t rawData[8];
static uint32_t rawPressure;
static uint32_t rawTemperature;
static uint16_t rawHumidity;

static uint8_t calibrationData[24];

static uint16_t dig_T1;
static int16_t dig_T2;
static int16_t dig_T3;

static uint16_t dig_P1;
static int16_t dig_P2;
static int16_t dig_P3;
static int16_t dig_P4;
static int16_t dig_P5;
static int16_t dig_P6;
static int16_t dig_P7;
static int16_t dig_P8;
static int16_t dig_P9;

static int32_t tFine;
static float temperature_C;
static float pressure_hPa;

HAL_StatusTypeDef BME280_Init(void)
{
	uint8_t chipID;
	uint8_t registerValue;

	// Verify Device is a BME280
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

	// Configure Humidity Oversampling (1x)
	registerValue = 0x01;

	if (HAL_I2C_Mem_Write(
			&hi2c1,
			BME280_I2C_ADDRESS << 1,
			BME280_CTRL_HUM_REG,
			I2C_MEMADD_SIZE_8BIT,
			&registerValue,
			1,
			100) != HAL_OK)
	{
		return HAL_ERROR;
	}

	// Configure Temperature and Pressure Oversampling (1x) and Mode
	registerValue = 0x27;

	if (HAL_I2C_Mem_Write(
			&hi2c1,
			BME280_I2C_ADDRESS << 1,
			BME280_CTRL_MEAS_REG,
			I2C_MEMADD_SIZE_8BIT,
			&registerValue,
			1,
			100) != HAL_OK)
	{
		return HAL_ERROR;
	}

	// Configure Standby Time (1000 ms), IIR Filter Off, and No SPI
	registerValue = 0xA0;

	if (HAL_I2C_Mem_Write(
			&hi2c1,
			BME280_I2C_ADDRESS << 1,
			BME280_CONFIG_REG,
			I2C_MEMADD_SIZE_8BIT,
			&registerValue,
			1,
			100) != HAL_OK)
	{
		return HAL_ERROR;
	}

	return HAL_OK;
}

HAL_StatusTypeDef BME280_ReadRawData(void)
{
	if (HAL_I2C_Mem_Read(
			&hi2c1,
			BME280_I2C_ADDRESS << 1,
			BME280_DATA_REG,
			I2C_MEMADD_SIZE_8BIT,
			rawData,
			8,
			100) != HAL_OK)
	{
		return HAL_ERROR;
	}

	rawPressure =
		((uint32_t)rawData[0] << 12) |
		((uint32_t)rawData[1] << 4) |
		((uint32_t)rawData[2] >> 4);

	rawTemperature =
		((uint32_t)rawData[3] << 12) |
		((uint32_t)rawData[4] << 4) |
		((uint32_t)rawData[5] >> 4);

	rawHumidity =
		((uint16_t)rawData[6] << 8) |
		((uint16_t)rawData[7]);

	return HAL_OK;
}

HAL_StatusTypeDef BME280_ReadCalibrationData(void)
{
	if (HAL_I2C_Mem_Read(
			&hi2c1,
			BME280_I2C_ADDRESS << 1,
			BME280_CALIB00_REG,
			I2C_MEMADD_SIZE_8BIT,
			calibrationData,
			24,
			100) != HAL_OK)
	{
		return HAL_ERROR;
	}

	dig_T1 = ((uint16_t)calibrationData[1] << 8) | calibrationData[0];
	dig_T2 = ((int16_t)calibrationData[3] << 8) | calibrationData[2];
	dig_T3 = ((int16_t)calibrationData[5] << 8) | calibrationData[4];

	dig_P1 = ((uint16_t)calibrationData[7]  << 8) | calibrationData[6];
	dig_P2 = ((int16_t) calibrationData[9]  << 8) | calibrationData[8];
	dig_P3 = ((int16_t) calibrationData[11] << 8) | calibrationData[10];
	dig_P4 = ((int16_t) calibrationData[13] << 8) | calibrationData[12];
	dig_P5 = ((int16_t) calibrationData[15] << 8) | calibrationData[14];
	dig_P6 = ((int16_t) calibrationData[17] << 8) | calibrationData[16];
	dig_P7 = ((int16_t) calibrationData[19] << 8) | calibrationData[18];
	dig_P8 = ((int16_t) calibrationData[21] << 8) | calibrationData[20];
	dig_P9 = ((int16_t) calibrationData[23] << 8) | calibrationData[22];

	return HAL_OK;
}

HAL_StatusTypeDef BME280_CompensateTemperature(void)
{
	int32_t var1;
	int32_t var2;

	var1 = ((((rawTemperature >> 3) - ((int32_t)dig_T1 << 1))) *
			((int32_t)dig_T2)) >> 11;

	var2 = (((((rawTemperature >> 4) - ((int32_t)dig_T1)) *
			((rawTemperature >> 4) - ((int32_t)dig_T1))) >> 12) *
			((int32_t)dig_T3)) >> 14;

	tFine = var1 + var2;

	temperature_C = ((tFine * 5 + 128) >> 8) / 100.0f;

	return HAL_OK;
}

HAL_StatusTypeDef BME280_CompensatePressure(void)
{
    int64_t var1;
    int64_t var2;
    int64_t pressureRaw;

    pressureRaw = (uint64_t)rawPressure;

    var1 = ((int64_t)tFine) - 128000;
    var2 = var1 * var1 * (int64_t)dig_P6;
    var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
    var2 = var2 + (((int64_t)dig_P4) << 35);

    var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) +
           ((var1 * (int64_t)dig_P2) << 12);

    var1 = (((((int64_t)1) << 47) + var1)) *
           ((int64_t)dig_P1) >> 33;

    if (var1 == 0)
    {
        return HAL_ERROR;
    }

    pressureRaw = 1048576 - pressureRaw;

    pressureRaw = (((pressureRaw << 31) - var2) * 3125) / var1;

    var1 = (((int64_t)dig_P9) *
           ((int64_t)(pressureRaw >> 13) * (pressureRaw >> 13))) >> 25;

    var2 = (((int64_t)dig_P8) * pressureRaw) >> 19;

    pressureRaw = ((pressureRaw + var1 + var2) >> 8) +
                  (((int64_t)dig_P7) << 4);

    pressure_hPa = pressureRaw / 25600.0f;

    return HAL_OK;
}

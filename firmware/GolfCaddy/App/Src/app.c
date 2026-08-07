#include "app.h"
#include "bme280.h"

void App_Init(void)
{
	if (BME280_Init() != HAL_OK)
	{
	    return;
	}

	if (BME280_ReadCalibrationData() != HAL_OK)
	{
	    return;
	}

	if (BME280_ReadRawData() != HAL_OK)
	{
	    return;
	}

	if (BME280_CompensateTemperature() != HAL_OK)
	{
	    return;
	}

	if (BME280_CompensatePressure() != HAL_OK)
	{
		return;
	}

	if (BME280_CompensateHumidity() != HAL_OK)
	{
		return;
	}
}

void App_Run(uint32_t currentTime)
{
	(void)currentTime;
}

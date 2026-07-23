#include "app.h"
#include "bme280.h"

volatile HAL_StatusTypeDef bme280Status;

void App_Init(void)
{
	bme280Status = BME280_Init();
}

void App_Run(uint32_t currentTime)
{
	(void)currentTime;
}

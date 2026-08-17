#include <ssd1309.h>
#include "app.h"
#include "bme280.h"
#include "display.h"

void App_Init(void)
{
	if (BME280_Init() != HAL_OK)
	{
	    return;
	}

	if (SSD1309_Init() != HAL_OK)
	{
		return;
	}

	Display_DrawGolfScreen();
}

void App_Run(uint32_t currentTime)
{
	(void)currentTime;
}

#include "ssd1309.h"
#include "i2c.h"

#define SSD1309_ADDRESS 	0x3C

#define SSD1309_CONTROL_COMMAND 	0x00
#define SSD1309_CONTROL_DATA		0x40

static const uint8_t SSD1309_INIT_COMMANDS[] =
{
    SSD1309_CONTROL_COMMAND,

    0xAE,               // Display OFF
    0xFD, 0x12,         // Unlock command interface
	0x20, 0x02,			// Page Addressing Mode
    0x00,               // Lower column start address = 0
    0x10,               // Higher column start address = 0
    0x40,               // Display Start Line = 0
    0x81, 0xBF,         // Contrast Control
    0xA1,               // Segment Re-map
    0xA6,               // Normal Display
    0xA8, 0x3F,         // Multiplex Ratio = 64
    0xC8,               // COM Scan Direction
    0xD3, 0x00,         // Display Offset = 0
    0xD5, 0xA0,         // Display Clock Divide Ratio / Oscillator Frequency
    0xD9, 0xF1,         // Pre-charge Period
    0xDA, 0x12,         // COM Pins Hardware Configuration
    0xDB, 0x34,         // VCOMH Deselect Level
    0xA4,               // Resume Display from GDDRAM
    0xA6,               // Normal Display
    0xAF                // Display ON
};

static uint8_t frameBuffer[SSD1309_WIDTH * SSD1309_HEIGHT / 8];

HAL_StatusTypeDef SSD1309_Init(void)
{
	HAL_Delay(200);

	return HAL_I2C_Master_Transmit(&hi2c1,
			SSD1309_ADDRESS << 1,
			(uint8_t *)SSD1309_INIT_COMMANDS,
			sizeof(SSD1309_INIT_COMMANDS),
			HAL_MAX_DELAY);
}

HAL_StatusTypeDef SSD1309_WriteCommand(uint8_t command)
{
	uint8_t commandPacket[2];

	commandPacket[0] = SSD1309_CONTROL_COMMAND;
	commandPacket[1] = command;

	return HAL_I2C_Master_Transmit(&hi2c1,
			SSD1309_ADDRESS << 1,
			commandPacket,
			sizeof(commandPacket),
			HAL_MAX_DELAY);
}

HAL_StatusTypeDef SSD1309_WriteData(uint8_t *data, uint16_t size)
{
	return HAL_I2C_Mem_Write(&hi2c1,
			SSD1309_ADDRESS << 1,
			SSD1309_CONTROL_DATA,
			I2C_MEMADD_SIZE_8BIT,
			data,
			size,
			HAL_MAX_DELAY);
}

void SSD1309_UpdateDisplay(void)
{
	for (uint8_t page = 0; page < SSD1309_HEIGHT / 8; page ++)
	{
		SSD1309_WriteCommand(0xB0 + page);
		SSD1309_WriteCommand(0x00);
		SSD1309_WriteCommand(0x10);

		SSD1309_WriteData(&frameBuffer[SSD1309_WIDTH * page], SSD1309_WIDTH);
	}
}

void SSD1309_DrawPixel(uint8_t x, uint8_t y)
{
	if (x >= SSD1309_WIDTH || y >= SSD1309_HEIGHT)
	{
		return;
	}

	frameBuffer[x + (y/8) * SSD1309_WIDTH] |= (1 << (y % 8));
}

void SSD1309_Clear(void)
{
	for (uint16_t i = 0; i < sizeof(frameBuffer); i++)
	{
		frameBuffer[i] = 0x00;
	}
}

void SSD1309_DrawHorizontalLine(uint8_t x, uint8_t y, uint8_t length)
{
	for (uint8_t i = 0; i < length && (x + i) < SSD1309_WIDTH; i++)
	{
		SSD1309_DrawPixel(x + i, y);
	}
}

void SSD1309_DrawVerticalLine(uint8_t x, uint8_t y, uint8_t length)
{
	for (uint8_t i = 0; i < length && (y + i) < SSD1309_HEIGHT; i++)
	{
		SSD1309_DrawPixel(x, y + i);
	}
}

void SSD1309_DrawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	if (width == 0 || height == 0)
	{
		return;
	}

	SSD1309_DrawHorizontalLine(x, y, width);
	SSD1309_DrawHorizontalLine(x, y + height - 1, width);

	SSD1309_DrawVerticalLine(x, y, height);
	SSD1309_DrawVerticalLine(x + width - 1, y, height);
}

void SSD1309_DrawChar(uint8_t x, uint8_t y, char character, const Font *font)
{
	for (uint8_t row = 0; row < font->height; row++)
	{
		uint16_t rowData = font->data[(character - 32) * font->height + row];

		for (uint8_t column = 0; column < font->width; column++)
		{
			if (rowData & (0x8000 >> column))
			{
				SSD1309_DrawPixel(x + column, y + row);
			}
		}
	}
}

void SSD1309_DrawString(uint8_t x, uint8_t y, const char *string, const Font *font)
{
	while (*string != '\0')
	{
		SSD1309_DrawChar(x, y, *string, font);

		x += font->width;
		string++;
	}
}

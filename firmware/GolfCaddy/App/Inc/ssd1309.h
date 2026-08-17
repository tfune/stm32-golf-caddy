#pragma once

#include <stdint.h>
#include "stm32u0xx_hal.h"
#include "font.h"

#define SSD1309_WIDTH		128
#define SSD1309_HEIGHT 		64

HAL_StatusTypeDef SSD1309_Init(void);
HAL_StatusTypeDef SSD1309_WriteCommand(uint8_t command);
HAL_StatusTypeDef SSD1309_WriteData(uint8_t *data, uint16_t size);
void SSD1309_UpdateDisplay(void);
void SSD1309_DrawPixel(uint8_t x, uint8_t y);
void SSD1309_Clear(void);
void SSD1309_DrawHorizontalLine(uint8_t x, uint8_t y, uint8_t length);
void SSD1309_DrawVerticalLine(uint8_t x, uint8_t y, uint8_t length);
void SSD1309_DrawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
void SSD1309_DrawChar(uint8_t x, uint8_t y, char character, const Font *font);
void SSD1309_DrawString(uint8_t x, uint8_t y, const char *string, const Font *font);

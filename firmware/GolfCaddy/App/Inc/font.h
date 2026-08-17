#pragma once

#include <stdint.h>

typedef struct
{
	uint8_t width;
	uint8_t height;
	const uint16_t *data;
} Font;

extern const Font FONT_7x10;

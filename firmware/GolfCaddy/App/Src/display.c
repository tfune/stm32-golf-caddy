#include "display.h"
#include "ssd1309.h"
#include "font.h"

void Display_DrawGolfScreen(void)
{
	SSD1309_Clear();

		SSD1309_DrawString(0, 0, "HOLE 7", &FONT_7x10);
		SSD1309_DrawString(93, 0, "PAR 4", &FONT_7x10);

		SSD1309_DrawHorizontalLine(0, 11, 128);

		SSD1309_DrawString(14, 20, "F", &FONT_7x10);
		SSD1309_DrawString(60, 20, "C", &FONT_7x10);
		SSD1309_DrawString(107, 20, "B", &FONT_7x10);

		SSD1309_DrawString(7, 32, "142", &FONT_7x10);
		SSD1309_DrawString(53, 32, "156", &FONT_7x10);
		SSD1309_DrawString(100, 32, "169", &FONT_7x10);

		SSD1309_DrawHorizontalLine(0, 45, 128);

		SSD1309_DrawString(0, 49, "STK 3", & FONT_7x10);
		SSD1309_DrawString(86, 49, "SCR +2", &FONT_7x10);

		SSD1309_UpdateDisplay();
}

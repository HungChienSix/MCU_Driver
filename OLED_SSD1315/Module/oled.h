#ifndef __OLED_H
#define __OLED_H 

#include "main.h"
#include "font.h"
#include "ssd1315.h"


// Font Regular font, italic, bold, underline
#define FONT_Regular 0 
#define FONT_Italic  1 
#define FONT_Bold    2 
#define FONT_Under   3 

extern const uint8_t Font_8x16_consolas[96][16];
extern const unsigned char Image_saber[512];

void OLED_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t SetPixel, uint8_t Type);
void OLED_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t SetPixel, uint8_t Type);
void OLED_DrawQuarterArc(uint16_t x0, uint16_t y0, uint16_t r, uint8_t quadrant_mask, uint8_t SetPixel, uint8_t Type);
void OLED_DrawQuarterSector(uint16_t x0, uint16_t y0, uint16_t r, uint8_t quadrant_mask, uint8_t SetPixel, uint8_t Type);
void OLED_DrawChar(uint16_t x, uint16_t y, char ch, const struFont *font, uint8_t font_t, uint8_t Type);
void OLED_DrawString(uint16_t x, uint16_t y, const char *str, const struFont *font, uint8_t font_t, uint8_t Type);
void OLED_DrawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *image, uint8_t Type);

#endif
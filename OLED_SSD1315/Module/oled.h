#ifndef __OLED_H
#define __OLED_H 

#include "main.h"
#include "font.h"

extern const uint8_t Font_8x16_consolas[96][16];

void OLED_DrawLine      (uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint8_t SetPixel);
void OLED_DrawRectangle (uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint8_t SetPixel);
void OLED_DrawQuarterArc(uint16_t x0, uint16_t y0, uint16_t r, uint8_t quadrant, uint8_t SetPixel);
void OLED_DrawQuarterSector(uint16_t x0, uint16_t y0, uint16_t r, uint8_t quadrant, uint8_t SetPixel);
void OLED_DrawChar(uint16_t x0, uint16_t y0, char ch, uint8_t SetPixel, const struFont *font, uint8_t type);
void OLED_DrawString(uint16_t x0, uint16_t y0, const char *str, uint8_t SetPixel, const struFont *font, uint8_t type);
void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *image);

#endif
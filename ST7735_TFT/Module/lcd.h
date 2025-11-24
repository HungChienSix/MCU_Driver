#ifndef __LCD_H
#define __LCD_H 

#include "main.h"
#include "fonts.h"
#include "st7735.h"

// Font Regular font, italic, bold, underline
#define FONT_Regular 0 
#define FONT_Italic  1 
#define FONT_Bold    2 
#define FONT_Under   3 

typedef enum LCD_Quarter{
	Quarter1 = 0x01,
	Quarter2 = 0x02,
	Quarter3 = 0x04,
	Quarter4 = 0x08
} LCD_Quarter_t;

extern const uint8_t Font_8x16_consolas[96][16];
extern const unsigned char Image_saber[512];

LCD_Event_t LCD_DrawLine(int16_t x0, int16_t x1, int16_t y0, int16_t y1, LCD_Pixel_t Pixel_Set, LCD_Type_t type);
LCD_Event_t LCD_DrawRectangle(int16_t x0, int16_t x1, int16_t y0, int16_t y1, LCD_Pixel_t Pixel_Set, LCD_Type_t type);
LCD_Event_t LCD_DrawQuarterArc(int16_t x0, int16_t y0, uint16_t r, LCD_Quarter_t quadrant_mask, LCD_Pixel_t Pixel_Set, LCD_Type_t type);
LCD_Event_t LCD_DrawQuarterSector(int16_t x0, int16_t y0, uint16_t r, LCD_Quarter_t quadrant_mask, LCD_Pixel_t Pixel_Set, LCD_Type_t type);
LCD_Event_t LCD_DrawChar(int16_t x0, int16_t y0, char ch, const struFont *font, uint8_t font_t, LCD_Pixel_t Pixel_Set, LCD_Type_t type);
LCD_Event_t LCD_DrawString(int16_t x0, int16_t y0, const char *str, const struFont *font, uint8_t font_t, LCD_Pixel_t Pixel_Set, LCD_Type_t type);
LCD_Event_t LCD_DrawImage(int16_t x0, int16_t y0, uint16_t width, uint16_t height, const uint8_t *image, LCD_Pixel_t Pixel_Set, LCD_Type_t type);

#endif
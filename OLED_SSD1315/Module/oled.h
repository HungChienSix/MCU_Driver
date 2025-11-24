#ifndef __OLED_H
#define __OLED_H 

#include "main.h"
#include "fonts.h"
#include "ssd1315.h"

// Font Regular font, italic, bold, underline
#define FONT_Regular 0 
#define FONT_Italic  1 
#define FONT_Bold    2 
#define FONT_Under   3 

typedef enum OLED_Quarter{
	Quarter1 = 0x01,
	Quarter2 = 0x02,
	Quarter3 = 0x04,
	Quarter4 = 0x08
} OLED_Quarter_t;

extern const struFont_type_t Font_8x16_consolas_type;
extern const struFont Font_8x16_consolas;
extern const unsigned char Image_saber[512];

OLED_Event_t OLED_DrawLine(int16_t x0, int16_t x1, int16_t y0, int16_t y1, OLED_Pixel_t Pixel_Set, OLED_Type_t type);
OLED_Event_t OLED_DrawRectangle(int16_t x0, int16_t x1, int16_t y0, int16_t y1, OLED_Pixel_t Pixel_Set, OLED_Type_t type);
OLED_Event_t OLED_DrawQuarterArc(int16_t x0, int16_t y0, uint16_t r, OLED_Quarter_t quadrant_mask, OLED_Pixel_t Pixel_Set, OLED_Type_t type);
OLED_Event_t OLED_DrawQuarterSector(int16_t x0, int16_t y0, uint16_t r, OLED_Quarter_t quadrant_mask, OLED_Pixel_t Pixel_Set, OLED_Type_t type);
OLED_Event_t OLED_DrawChar(int16_t x0, int16_t y0, char ch, const struFont *font, uint8_t font_t, OLED_Pixel_t Pixel_Set, OLED_Type_t type);
OLED_Event_t OLED_DrawString(int16_t x0, int16_t y0, const char *str, const struFont *font, uint8_t font_t, OLED_Pixel_t Pixel_Set, OLED_Type_t type);
OLED_Event_t OLED_DrawImage(int16_t x0, int16_t y0, uint16_t width, uint16_t height, const uint8_t *image, OLED_Pixel_t Pixel_Set, OLED_Type_t type);

#endif
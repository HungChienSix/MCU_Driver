#ifndef __OLED_H
#define __OLED_H 

#include "main.h"
#include "font.h"

// OLED_DrawImage Normal display, inverse display, overlay display, XOR display
#define IMG_Normal  0 //正常(1绘制,0透明)
#define IMG_Inverse 1 //反色(0绘制,1透明)  
#define IMG_Overlay 2 //覆盖(1绘制,0清除)
#define IMG_XOR     3 //异或模式

// Font Regular font, italic, bold, underline
#define FONT_Regular 0 
#define FONT_Italic  1 
#define FONT_Bold    2 
#define FONT_Under   3 

extern const uint8_t Font_8x16_consolas[96][16];
extern const unsigned char Image_saber[512];

void OLED_DrawLine      (uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint8_t SetPixel);
void OLED_DrawRectangle (uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint8_t SetPixel);
void OLED_DrawQuarterArc(uint16_t x0, uint16_t y0, uint16_t r, uint8_t quadrant, uint8_t SetPixel);
void OLED_DrawQuarterSector(uint16_t x0, uint16_t y0, uint16_t r, uint8_t quadrant, uint8_t SetPixel);
void OLED_DrawChar(uint16_t x0, uint16_t y0, char ch, uint8_t SetPixel, const struFont *font, uint8_t type);
void OLED_DrawString(uint16_t x0, uint16_t y0, const char *str, uint8_t SetPixel, const struFont *font, uint8_t type);
void OLED_DrawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *image, uint8_t mode);

#endif
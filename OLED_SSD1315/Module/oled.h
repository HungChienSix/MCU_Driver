#ifndef __OLED_H
#define __OLED_H 

#include "main.h"
#include "i2c.h"

#define SSD1315 

// IO pin
#define SSD1315_I2C hi2c1

// Address
#define SSD1315_Dev_Addr     0x78
#define SSD1315_MemCmd_Addr  0x00
#define SSD1315_MemData_Addr 0x40

// Screen Size
#define SSD1315_ColSTART 	  0x00
#define SSD1315_ColEND 		  0x7F
#define SSD1315_PageSTART 	0x00
#define SSD1315_PageEND 		0x07

// Screen Direction
#define SSD1315_ROTATION 	0x00

// Screen Brightness
#define SSD315_Brightness 0x7F

// Color Inverse: 0xA6=NO, 0xA7=YES
#define ST7735_INVERSE 0xA6

void OLED_DrawPoint(uint8_t x,uint8_t y);
void OLED_Refresh(void);
void OLED_Clear(void);

void SSD1315_Init(void);
void SSD1315_FillScreen(uint16_t color);
void SSD1315_DrawLine(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end,uint16_t color);
void SSD1315_DrawQuarterArc(uint16_t x0, uint16_t y0, uint16_t r, uint8_t quadrant, uint16_t color);
void SSD1315_DrawRectangle(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t color);
void SSD1315_DrawQuarterSector(uint16_t x0, uint16_t y0, uint16_t r, uint8_t quadrant, uint16_t color);
// void ST7735_DrawChar(uint16_t x0, uint16_t y0, char ch, uint16_t color, const struFont *font, uint8_t type);
// void ST7735_DrawString(uint16_t x0, uint16_t y0, const char *str, uint16_t color, const struFont *font, uint8_t type);
void ST7735_DrawFrame(void);

#endif

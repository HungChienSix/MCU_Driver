#ifndef __SSD1315_H
#define __SSD1315_H 

#include "main.h"
#include "i2c.h"

#define SSD1315 

// IO pin
#define SSD1315_I2C hi2c1

// Address
#define SSD1315_Dev_Addr        0x78
#define SSD1315_MemCmd_Addr     0x00
#define SSD1315_MemData_Addr    0x40

// Screen Size
#define SSD1315_WIDTH 	128
#define SSD1315_HEIGHT 	64
#define SSD1315_PAGES 	8

// Screen Direction
#define SSD1315_ROTATION    0x00

// Screen Brightness
#define SSD315_Brightness   0x7F

// Color Inverse: 0xA6=NO, 0xA7=YES
#define SSD1315_INVERSE     0xA6

void OLED_Init(void);
void OLED_DrawPixel (uint8_t x, uint8_t y, uint8_t SetPixel);
void OLED_FillScreen(uint8_t SetPixel);
void OLED_RefreshScreen_Force(void);
void OLED_RefreshScreen(void);
void OLED_RefreshArea(uint8_t start_page, uint8_t end_page, uint8_t start_col, uint8_t end_col);

#endif
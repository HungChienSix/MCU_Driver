#ifndef __SSD1315_H
#define __SSD1315_H 

#include "main.h"
#include "i2c.h"

#define SSD1315 

typedef enum OLED_Pixel
{
	OLED_OFF = 0,   // Low
  OLED_ON,     		// High
	OLED_ERROR
} OLED_Pixel_t;

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

// If you do not use partition refresh, please comment out this macro definition.
#define PartRefresh 

void OLED_Init(void);
uint8_t OLED_ReadPixel (uint16_t x0, uint16_t y0);
void OLED_DrawPixel (uint16_t x0, uint16_t y0, OLED_Pixel_t SetPixel);
void OLED_DrawHorizontalLine (int16_t x0, int16_t x1, int16_t y, OLED_Pixel_t SetPixel);
void OLED_FillArea  (uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, OLED_Pixel_t SetPixel);
void OLED_FillScreen(OLED_Pixel_t SetPixel);
void OLED_RefreshScreen(void);
void OLED_RefreshScreen_Force(void);

//void OLED_RefreshArea(uint8_t start_page, uint8_t end_page, uint8_t start_col, uint8_t end_col);

#endif
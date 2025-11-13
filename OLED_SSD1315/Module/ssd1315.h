#ifndef __SSD1315_H
#define __SSD1315_H 

#include "main.h"
#include "i2c.h"

#define SSD1315 

typedef enum OLED_Pixel
{
	OLED_OFF = 0,   // Low
  OLED_ON,     		// High
	OLED_IDLE,			// 未绘制
	OLED_ERROR			// 读取错误
} OLED_Pixel_t;

// 
#define OLED_Normal  	0x01 //正常(1:绘制为OLED_ON )
#define OLED_Inverse 	0x02 //反向(1:绘制为OLED_OFF)  
#define OLED_XOR    	0x04 //异或

#define OLED_nOver 		0x00 //不覆盖绘制
#define OLED_Over 	 	0x10 //覆盖绘制

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
//#define PartRefresh 

void OLED_Init(void);
OLED_Pixel_t OLED_ReadPixel	(uint16_t x0, uint16_t y0);
OLED_Pixel_t OLED_DrawPixel (uint16_t x, uint16_t y, OLED_Pixel_t Pixel_Set, uint8_t Type);
void OLED_DrawHorizontalLine(int16_t x0, int16_t x1, int16_t y, OLED_Pixel_t SetPixel, uint8_t Type);
void OLED_DrawVerticalLine  (int16_t x, int16_t y0, int16_t y1, OLED_Pixel_t SetPixel, uint8_t Type);
void OLED_FillArea          (uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, OLED_Pixel_t SetPixel, uint8_t Type);
void OLED_FillScreen        (OLED_Pixel_t SetPixel);
void OLED_RefreshScreen     (void);
void OLED_RefreshScreen_Force(void);

//void OLED_RefreshArea(uint8_t start_page, uint8_t end_page, uint8_t start_col, uint8_t end_col);

#endif
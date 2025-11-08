#ifndef __OLED_H
#define __OLED_H 

#include "main.h"
#include "i2c.h"

#define SSD1315 

// IO pin
#define SSD1315_I2C hi2c1

// Address
#define SSD1315_Dev_Addr 0x78
#define SSD1315_MemCmd_Addr  0x00
#define SSD1315_MemData_Addr 0x40

// Screen Size
#define ST7735_ColSTART 	0x00
#define ST7735_ColEND 		0x7F
#define ST7735_PageSTART 	0x00
#define ST7735_PageEND 		0x07

// Screen Direction
#define SSD1315_ROTATION 	0x00

// Screen Brightness
#define SSD315_Brightness 0x7F

// Color Inverse: 0xA6=NO, 0xA7=YES
#define ST7735_INVERSE 0xA7

void SSD1315_Init(void);
void OLED_DrawPoint(uint8_t x,uint8_t y);
void OLED_Refresh(void);
void OLED_Clear(void);

#endif

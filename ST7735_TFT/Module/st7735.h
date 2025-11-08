#ifndef ST7735_H
#define ST7735_H

#include "main.h"
#include "spi.h"
#include "stdio.h"
#include "fonts.h"

#define ST7735 

// IO pin
#define ST7735_RST_Pin 				TFT_RES_Pin
#define ST7735_RST_GPIO_Port 	TFT_RES_GPIO_Port
#define ST7735_DC_Pin 				TFT_DC_Pin
#define ST7735_DC_GPIO_Port		TFT_DC_GPIO_Port
#define ST7735_CS_Pin 				TFT_CS_Pin
#define ST7735_CS_GPIO_Port 	TFT_CS_GPIO_Port
#define ST7735_BLK_Pin 				TFT_BLK_Pin
#define ST7735_BLK_GPIO_Port 	TFT_BLK_GPIO_Port
#define ST7735_SPI hspi1

// Screen Size
#define ST7735_XSTART 2
#define ST7735_YSTART 3
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 128

// Screen Direction
#define ST7735_ROTATION 0

// Color Mode: RGB or BGR
#define ST7735_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MODE ST7735_MADCTL_BGR

// Color Inverse: 0=NO, 1=YES
#define ST7735_INVERSE 0

// Color definitions 
#define ST7735_BLACK   0x0000
#define ST7735_BLUE    0x001F
#define ST7735_RED     0xF800
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF
#define ST7735_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

#define ST7735_SLPOUT   0x11
#define ST7735_FRMCTR1  0xB1
#define ST7735_FRMCTR2  0xB2
#define ST7735_FRMCTR3  0xB3
#define ST7735_INVCTR   0xB4
#define ST7735_PWCTR1   0xC0
#define ST7735_PWCTR2   0xC1
#define ST7735_PWCTR3   0xC2
#define ST7735_PWCTR4   0xC3
#define ST7735_PWCTR5   0xC4
#define ST7735_VMCTR1   0xC5
#define ST7735_COLMOD   0x3A
#define ST7735_GMCTRP1  0xE0
#define ST7735_GMCTRN1  0xE1
#define ST7735_NORON    0x13
#define ST7735_DISPON   0x29
#define ST7735_CASET    0x2A
#define ST7735_RASET    0x2B
#define ST7735_RAMWR    0x2C
#define ST7735_INVOFF   0x20
#define ST7735_INVON    0x21

#define ST7735_MADCTL     0x36
#define ST7735_MADCTL_MX  0x40
#define ST7735_MADCTL_MY  0x80
#define ST7735_MADCTL_MV  0x20

// Font
extern const uint8_t Font_8x16_consolas[96][16];

void ST7735_Init(void);
void ST7735_FillScreen(uint16_t color);
void ST7735_DrawLine(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end,uint16_t color);
void ST7735_DrawQuarterArc(uint16_t x0, uint16_t y0, uint16_t r, uint8_t quadrant, uint16_t color);
void ST7735_DrawRectangle(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t color);
void ST7735_DrawQuarterSector(uint16_t x0, uint16_t y0, uint16_t r, uint8_t quadrant, uint16_t color);
void ST7735_DrawChar(uint16_t x0, uint16_t y0, char ch, uint16_t color, const struFont *font, uint8_t type);
void ST7735_DrawString(uint16_t x0, uint16_t y0, const char *str, uint16_t color, const struFont *font, uint8_t type);
void ST7735_DrawFrame(void);

#endif

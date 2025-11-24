#ifndef __SSD1315_H
#define __SSD1315_H 

#include "main.h"
#include "i2c.h"

#define SSD1315 

// GPIO and Peripheral Setting
#define SSD1315_I2C              hi2c1

#define SSD1315_Dev_Addr         0x78
#define SSD1315_MemCmd_Addr      0x00
#define SSD1315_MemData_Addr     0x40

// Screen Size
#define SSD1315_WIDTH            128
#define SSD1315_HEIGHT           64
#define SSD1315_PAGES            8
#define SSD1315_PAGE_HEIGHT      8    // 每页8行

// Screen Direction 
#define SSD1315_ROTATION         0x00

// 屏幕亮度
#define SSD1315_Brightness       0x7F

// 颜色反色: 0xA6=NO, 0xA7=YES
#define SSD1315_INVERSE          0xA6

// 分区刷新宏 
//#define SSD1315_PARTIAL_REFRESH 

typedef enum OLED_Event{
	OLED_ERROR = 0,	// 错误
	OLED_SUCCESS 	// 绘制成功
} OLED_Event_t;

typedef enum OLED_Pixel{
	OLED_OFF = 0x00,// 灭 
	OLED_ON = 0xFF	// 亮
} OLED_Pixel_t;

typedef enum OLED_Type{
	OLED_Nor = 0x00,// 正常显示
	OLED_Xor = 0xFF,// 异或显示
} OLED_Type_t;

// 函数声明
void OLED_Init(void);
OLED_Pixel_t OLED_ReadPixel(uint16_t x, uint16_t y);
void OLED_DrawPixel(int16_t x, int16_t y, OLED_Pixel_t Pixel_Set);
void OLED_DrawHorLine(int16_t x0, int16_t x1, int16_t y, OLED_Pixel_t Pixel_Set, OLED_Type_t type);
void OLED_DrawVerLine(int16_t x, int16_t y1, int16_t y2, OLED_Pixel_t Pixel_Set, OLED_Type_t type);
void OLED_FillArea(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1, OLED_Pixel_t Pixel_Set, OLED_Type_t type);
void OLED_FillScreen(OLED_Pixel_t Pixel_Set);
void OLED_RefreshScreen(void);
void OLED_RefreshScreen_Force(void);

#endif

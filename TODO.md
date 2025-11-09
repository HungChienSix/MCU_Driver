
## ST7735 文件结构
- `Module/st7735.c(.h)` 屏幕驱动IC芯片的驱动代码
- `Module/oled.c(.h)` 包含OLED屏幕的基本绘制函数
- `Module/fonts.c(.h)` 字体数据与相关结构体

## SSD1315 文件结构
- `Module/ssd1315.c(.h)` 屏幕驱动IC芯片的驱动代码
- `Module/oled.c(.h)` 包含OLED屏幕的基本绘制函数
- `Module/fonts.c(.h)` 字体数据与相关结构体

### `Module/ssd1315.h`

```c
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
#define SSD1315_ColSTART    0x00
#define SSD1315_ColEND      0x7F
#define SSD1315_PageSTART   0x00
#define SSD1315_PageEND     0x07

// Screen Direction
#define SSD1315_ROTATION    0x00

// Screen Brightness
#define SSD315_Brightness   0x7F

// Color Inverse: 0xA6=NO, 0xA7=YES
#define SSD1315_INVERSE     0xA6

void OLED_Init(void);
void OLED_DrawPixel(uint8_t x, uint8_t y);
void OLED_RefreshScreen(uint8_t color);

#endif
```

### `Module/ssd1315.c`

```c
#include "oled.h"
//include ...

uint8_t display_ram[8][128]; // 定义屏幕显示缓冲区(可以换成一维数组)

void SSD1315_WriteCmd(uint8_t cmd){
    // 发送命令代码,每次发送一个字节
}

void SSD1315_WriteData(uint8_t data, uint8_t size){
    // 发送数据代码,每次可发送多个字节
}

void SSD1315_SetRotation(uint8_t rotation){
    // 设置屏幕方向
}

void OLED_Init(void){
    // 初始化代码
    // ...
    // SSD1315_SetRotation(SSD1315_ROTATION);
    // ...
}

void OLED_DrawPixel(uint8_t x, uint8_t y){
    // 绘制单个像素点,这是所有绘图函数的基础
    // 填写屏幕显示缓冲区
}

void OLED_RefreshScreen(uint8_t color){
    // 刷新整个屏幕
}

```
### `Module/oled.h(.c)`

oled.c和oled.h用来实现绘制图形功能，比如绘制线条、矩形、圆弧、字符和字符串等。

```c
#ifndef __OLED_H
#define __OLED_H

#include "ssd1315.h"
//#include ...

void ST7735_FillScreen(uint16_t color); // 填充屏幕
void ST7735_DrawLine(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end,uint16_t color); // 绘制直线
void ST7735_DrawQuarterArc(uint16_t x0, uint16_t y0, uint16_t r, uint8_t quadrant, uint16_t color); // 绘制四分之一圆弧
void ST7735_DrawRectangle(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t color); // 绘制空心矩形
void ST7735_DrawQuarterSector(uint16_t x0, uint16_t y0, uint16_t r, uint8_t quadrant, uint16_t color); // 绘制四分之一扇形
void ST7735_DrawChar(uint16_t x0, uint16_t y0, char ch, uint16_t color, const struFont *font, uint8_t type);
void ST7735_DrawString(uint16_t x0, uint16_t y0, const char *str, uint16_t color, const struFont *font, uint8_t type);
```

### `Module/font.h(.c)`

字体的存储主要靠一个字体结构体,每类字体包含Normal和Bold两种类型。


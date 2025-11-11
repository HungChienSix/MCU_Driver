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

#define OLED_ON  1
#define OLED_OFF 0

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
void OLED_DrawPixel (uint16_t x0, uint16_t y0, uint8_t SetPixel);
void OLED_DrawHorizontalLine (int16_t x0, int16_t x1, int16_t y, uint8_t SetPixel);
void OLED_FillArea  (uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t SetPixel);
void OLED_FillScreen(uint8_t SetPixel);
void OLED_RefreshScreen(void);
void OLED_RefreshScreen_Force(void);

//void OLED_RefreshArea(uint8_t start_page, uint8_t end_page, uint8_t start_col, uint8_t end_col);

#endif
```

### `Module/ssd1315.c`

对 `ssd1315.h` 所定义的函数的实现

### `Module/oled.h(.c)`

oled.c和oled.h用来实现绘制图形功能，比如绘制线条、矩形、圆弧、字符和字符串等。

### `Module/font.h(.c)`

```
typedef struct {
  const uint8_t width;
  const uint8_t height;
  const uint8_t bytes_per_row;  // 每行字节数，可以预计算
  const uint8_t bit_order;      // 位顺序：0=高位在前，1=低位在前
	
	const uint8_t *Font_Regular;
	const uint8_t *Font_Italic;
	const uint8_t *Font_Bold;
	const uint8_t *Font_Under;
} struFont;

```

字体的存储主要靠一个字体结构体,每类字体包含Normal,Bold,Italy,斜体四种类型。


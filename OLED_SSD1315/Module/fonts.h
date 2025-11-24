#ifndef __FONTS_H__
#define __FONTS_H__

#include <stdint.h>

#define FONT_NORMAL     0x0 
#define FONT_ITALIC     0x1 
#define FONT_BOLD       0x2
#define FONT_UNDERLINE  0x3

typedef struct {
	const uint8_t *Font_Regular;
	const uint8_t *Font_Italic;
	const uint8_t *Font_Bold;
	const uint8_t *Font_Under;
} struFont_type_t;

typedef struct {
  const uint8_t width;
  const uint8_t height;
  const uint8_t bytes_per_row;  // 每行字节数，可以预计算
  struFont_type_t type;
} struFont;

#endif
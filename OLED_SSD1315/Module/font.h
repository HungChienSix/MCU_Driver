#ifndef __FONT_H__
#define __FONT_H__

#include <stdint.h>

#define FONT_NORMAL     0x0 
#define FONT_ITALIC     0x1 
#define FONT_BOLD       0x2
#define FONT_UNDERLINE  0x3

typedef struct {
  const uint8_t width;
  const uint8_t height;
  const uint8_t bytes_per_row;  // 每行字节数，可以预计算
  const uint8_t bit_order;      // 位顺序：0=高位在前，1=低位在前
	
	const uint8_t *data_n;
	const uint8_t *data_i;
	const uint8_t *data_b;
	const uint8_t *data_u;
} struFont;

#endif
#include "oled.h"
#include "ssd1315.h"
#include "font.h"
#include <stdlib.h> // For abs()
#include <string.h> // For memset()

/**
 * @brief 绘制直线 - Bresenham算法
 * @note  坐标 (x, y) = (水平, 垂直)
 */
void OLED_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t SetPixel)
{
    // 优化：使用 stdlib.h 中的 abs()
	int16_t dx = abs(x1 - x0);
	int16_t dy = abs(y1 - y0);
	int16_t sx = (x0 < x1) ? 1 : -1;
	int16_t sy = (y0 < y1) ? 1 : -1;
	int16_t err = dx - dy;
	int16_t e2;

	while(1) {
		// 优化：使用 DrawPixel
		OLED_DrawPixel(x0, y0, SetPixel);
        // 优化：移除 frame_bit 写入

		if (x0 == x1 && y0 == y1) break;

		e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}
	}
}

/**
 * @brief 绘制实心矩形
 * @note  坐标 (x, y) = (水平, 垂直)
 * @note  绘制从 (x0, y0) 到 (x1-1, y1-1) 的矩形
 */
void OLED_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t SetPixel)
{
    // 修正坐标系
	uint16_t x_start = (x0 > x1) ? x1 : x0;
	uint16_t x_end   = (x0 > x1) ? x0 : x1;
	uint16_t y_start = (y0 > y1) ? y1 : y0;
	uint16_t y_end   = (y0 > y1) ? y0 : y1;
	
	for(uint16_t y = y_start; y < y_end; y++)
	{
		for(uint16_t x = x_start; x < x_end; x++)
		{
			OLED_DrawPixel(x, y, SetPixel);
		}
	}
}

/**
 * @brief 绘制四分之一圆弧 - Bresenham算法 (优化)
 * @note  坐标 (x0, y0) = (水平中心, 垂直中心)
 * @param quadrant_mask: 位掩码 0x01=右上, 0x02=左上, 0x04=左下, 0x08=右下
 */
void OLED_DrawQuarterArc(uint16_t x0, uint16_t y0, uint16_t r, uint8_t quadrant_mask, uint8_t SetPixel)
{
	int16_t x = 0;
	int16_t y = r;
	int16_t d = 3 - 2 * r;
	
    // 优化：合并为一个循环
    while(x <= y) {
        // (x0, y0) 是中心
        // 坐标系已修正为 (水平, 垂直)
        
        // 象限 1 (右上): (+x, -y), (+y, -x)
        if (quadrant_mask & 0x01) { 
            OLED_DrawPixel(x0 + x, y0 - y, SetPixel);
            OLED_DrawPixel(x0 + y, y0 - x, SetPixel);
        }
        // 象限 2 (左上): (-x, -y), (-y, -x)
        if (quadrant_mask & 0x02) { 
            OLED_DrawPixel(x0 - x, y0 - y, SetPixel);
            OLED_DrawPixel(x0 - y, y0 - x, SetPixel);
        }
        // 象限 3 (左下): (-x, +y), (-y, +x)
        if (quadrant_mask & 0x04) { 
            OLED_DrawPixel(x0 - x, y0 + y, SetPixel);
            OLED_DrawPixel(x0 - y, y0 + x, SetPixel);
        }
        // 象限 4 (右下): (+x, +y), (+y, +x)
        if (quadrant_mask & 0x08) { 
            OLED_DrawPixel(x0 + x, y0 + y, SetPixel);
            OLED_DrawPixel(x0 + y, y0 + x, SetPixel);
        }

        if(d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

/**
 * @brief 绘制四分之一扇形 (填充) (性能优化)
 * @note  坐标 (x0, y0) = (水平中心, 垂直中心)
 * @param quadrant_mask: 位掩码 0x01=右上, 0x02=左上, 0x04=左下, 0x08=右下
 */
void OLED_DrawQuarterSector(uint16_t x0, uint16_t y0, uint16_t r, uint8_t quadrant_mask, uint8_t SetPixel)
{
	int16_t x = 0;
	int16_t y = r;
	int16_t d = 3 - 2 * r;
	
	// 优化：使用水平扫描线填充，而不是径向线
	while(x <= y) {
		// 象限 1 (右上)
		if(quadrant_mask & 0x01) {
            OLED_DrawHorizontalLine(x0, x0 + x, y0 - y, SetPixel);
            OLED_DrawHorizontalLine(x0, x0 + y, y0 - x, SetPixel);
		}
		// 象限 2 (左上)
		if(quadrant_mask & 0x02) {
            OLED_DrawHorizontalLine(x0 - x, x0, y0 - y, SetPixel);
            OLED_DrawHorizontalLine(x0 - y, x0, y0 - x, SetPixel);
		}
		// 象限 3 (左下)
		if(quadrant_mask & 0x04) {
            OLED_DrawHorizontalLine(x0 - x, x0, y0 + y, SetPixel);
            OLED_DrawHorizontalLine(x0 - y, x0, y0 + x, SetPixel);
		}
		// 象限 4 (右下)
		if(quadrant_mask & 0x08) {
            OLED_DrawHorizontalLine(x0, x0 + x, y0 + y, SetPixel);
            OLED_DrawHorizontalLine(x0, x0 + y, y0 + x, SetPixel);
		}
	
		// Bresenham算法更新
		if(d < 0) {
			d = d + 4 * x + 6;
		} else {
			d = d + 4 * (x - y) + 10;
			y--;
		}
		x++;
	}
}

/**
 * @brief 绘制字符
 * @note  坐标 (x, y) = (水平, 垂直)
 */
void OLED_DrawChar(uint16_t x, uint16_t y, char ch, uint8_t SetPixel, const struFont *font, uint8_t type){
	uint8_t char_index = ch - ' ';
	const uint8_t *char_data ;
	switch(type){
		case(0):char_data = font->data_n + char_index * font->height * font->bytes_per_row;
			break;
		case(1):char_data = font->data_i + char_index * font->height * font->bytes_per_row;
			break;
		case(2):char_data = font->data_b + char_index * font->height * font->bytes_per_row;
			break;
		case(3):char_data = font->data_u + char_index * font->height * font->bytes_per_row;
			break;
	}

	for(uint8_t row = 0; row < font->height; row++) // 垂直
	{
		for(uint8_t col = 0; col < font->width; col++) // 水平
		{
			uint8_t byte_index = col / 8;
			uint8_t bit_index = col % 8;
			uint8_t current_byte = char_data[row * font->bytes_per_row + byte_index];
			
			uint8_t pixel_set = 0;
			if(font->bit_order == 0) {
				// 高位在前（MSB first）
				pixel_set = current_byte & (0x80 >> bit_index);
			} else {
				// 低位在前（LSB first）
				pixel_set = current_byte & (0x01 << bit_index);
			}
	
			if(pixel_set)
			{
				OLED_DrawPixel(x + col, y + row, SetPixel);
			}
		}
	}
}

/**
 * @brief 绘制字符串
 * @note  坐标 (x, y) = (水平, 垂直)
 */
void OLED_DrawString(uint16_t x, uint16_t y, const char *str, uint8_t SetPixel, const struFont *font, uint8_t type)
{
    // 修正坐标系
	uint16_t current_x = x;
	uint16_t current_y = y;
	
	while(*str != '\0')
	{
		OLED_DrawChar(current_x, current_y, *str, SetPixel, font, type);
		
		// 水平前进
		current_x += font->width;
	
		// 检查换行
		if(current_x + font->width >= SSD1315_WIDTH)
		{
			current_x = x; // 回到起始 x
			current_y += font->height; // 换到下一行 (y 增加)
	
			if(current_y + font->height >= SSD1315_HEIGHT)
			{
				break; // 超出屏幕底部
			}
		}
		
		str++; 
	}
}

/**
  * @brief 绘制图像 
  * @note  
	* @note  坐标 (x, y) = (水平, 垂直)
	*/
void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *image){
	
}
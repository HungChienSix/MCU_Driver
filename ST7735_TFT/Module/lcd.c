#include "lcd.h"
#include "st7735.h"

/**
 * @brief 绘制直线 - Bresenham算法
 * @param x0,x1: 直线水平坐标起始点
 * @param y0,y1: 直线垂直坐标起始点
 * @param Pixel_Set: 绘制图像的颜色(RGB565)
 * @param type: 绘制模式,LCD_Xor 异或模式,LCD_Nor 普通模式
 */
LCD_Event_t LCD_DrawLine(int16_t x0, int16_t x1, int16_t y0, int16_t y1, LCD_Pixel_t Pixel_Set, LCD_Type_t type)
{
	int16_t dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
	int16_t dy = (x1 > x0) ? (y1 - y0) : (x0 - x1);
	int16_t sx = (x0 < x1) ? 1 : -1;
	int16_t sy = (y0 < y1) ? 1 : -1;
	int16_t err = dx - dy;
	int16_t e2;

	while(1) {
		if((type == LCD_Xor)&&(LCD_ReadPixel(x0,y0) == Pixel_Set))
			LCD_DrawPixel(x0, y0, ~Pixel_Set);
		else
			LCD_DrawPixel(x0, y0, Pixel_Set);
			
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
	
	return LCD_SUCCESS;
}

/**
 * @brief 绘制实心矩形
 * @param x0,x1: 矩形水平坐标起始点
 * @param y0,y1: 矩形垂直坐标起始点
 * @param Pixel_Set: 绘制图像的颜色(RGB565)
 * @param type: 绘制模式,LCD_Xor 异或模式,LCD_Nor 普通模式
 */
LCD_Event_t LCD_DrawRectangle(int16_t x0, int16_t x1, int16_t y0, int16_t y1, LCD_Pixel_t Pixel_Set, LCD_Type_t type)
{
	LCD_FillArea(x0, x1, y0, y1, Pixel_Set, type);
	return LCD_SUCCESS;
}

/**
 * @brief 绘制四分之一圆弧 - Bresenham算法 
 * @param x0,y0: 圆弧的圆心
 * @param r: 圆弧的半径
 * @param quadrant_mask: 绘制的象限 Quarter1,Quarter2,Quarter3,Quarter4
 * @param Pixel_Set: 绘制图像的颜色(二值)
 * @param type: 绘制模式,LCD_Xor 异或模式,LCD_Nor 普通模式
 */
LCD_Event_t LCD_DrawQuarterArc(int16_t x0, int16_t y0, uint16_t r, uint8_t quadrant_mask, LCD_Pixel_t Pixel_Set, LCD_Type_t type)
{
    if (r == 0) {
        LCD_DrawPixel(x0, y0, Pixel_Set);
        return LCD_SUCCESS;
    }
    
    // 使用静态数组缓存圆弧点（适用于多次绘制相同半径的圆弧）
    static uint16_t last_r = 0;
    static int16_t arc_points[64][2]; // 假设最大半径对应的点数
    static uint16_t point_count = 0;
    
    // 如果半径改变，重新计算圆弧点
    if (r != last_r) {
        point_count = 0;
        int16_t x = 0;
        int16_t y = r;
        int16_t d = 3 - 2 * r;
        
        while(x <= y) {
            // 存储第一组点 (x,y)
            arc_points[point_count][0] = x;
            arc_points[point_count][1] = y;
            point_count++;
            
            // 存储第二组点 (y,x) - 只有当 x < y 时
            if (x < y) {
                arc_points[point_count][0] = y;
                arc_points[point_count][1] = x;
                point_count++;
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
        last_r = r;
    }
    
    // 使用预计算的点绘制圆弧
    for (uint16_t i = 0; i < point_count; i++) {
        int16_t x = arc_points[i][0];
        int16_t y = arc_points[i][1];
        
		int16_t x_temp,y_temp;
        if (quadrant_mask & 0x01) {x_temp = x0 + x; y_temp = y0 - y;}
		if((type == LCD_Xor)&&(LCD_ReadPixel(x0,y0) == Pixel_Set))
			LCD_DrawPixel(x_temp, y_temp, ~Pixel_Set);
		else
			LCD_DrawPixel(x_temp, y_temp, Pixel_Set);
		
        if (quadrant_mask & 0x02) {x_temp = x0 - x; y_temp = y0 - y;}
		if((type == LCD_Xor)&&(LCD_ReadPixel(x0,y0) == Pixel_Set))
			LCD_DrawPixel(x_temp, y_temp, ~Pixel_Set);
		else
			LCD_DrawPixel(x_temp, y_temp, Pixel_Set);
		
        if (quadrant_mask & 0x04) {x_temp = x0 - x; y_temp = y0 + y;}
				if((type == LCD_Xor)&&(LCD_ReadPixel(x0,y0) == Pixel_Set))
			LCD_DrawPixel(x_temp, y_temp, ~Pixel_Set);
		else
			LCD_DrawPixel(x_temp, y_temp, Pixel_Set);
		
        if (quadrant_mask & 0x08) {x_temp = x0 + x; y_temp = y0 + y;}
		if((type == LCD_Xor)&&(LCD_ReadPixel(x0,y0) == Pixel_Set))
			LCD_DrawPixel(x_temp, y_temp, ~Pixel_Set);
		else
			LCD_DrawPixel(x_temp, y_temp, Pixel_Set);
    }
    
    return LCD_SUCCESS;
}

// 辅助函数：更新边界数组
static void updateBounds(int16_t x, int16_t y, int16_t *min_x, int16_t *max_x)
{
    if(y >= 0 && y < ST7735_HEIGHT) {
        if(x < min_x[y]) min_x[y] = x;
        if(x > max_x[y]) max_x[y] = x;
    }
}

/**
 * @brief 绘制四分之一扇形 
 * @param x0,y0: 圆弧的圆心
 * @param r: 圆弧的半径
 * @param quadrant_mask: 绘制的象限 Quarter1,Quarter2,Quarter3,Quarter4
 * @param Pixel_Set: 绘制图像的颜色(RGB565)
 * @param type: 绘制模式,LCD_Xor 异或模式,LCD_Nor 普通模式
 */
LCD_Event_t LCD_DrawQuarterSector(int16_t x0, int16_t y0, uint16_t r, uint8_t quadrant_mask, LCD_Pixel_t Pixel_Set, LCD_Type_t type)
{
    if (r == 0) {
        return LCD_ERROR;
    }
    
    // 预计算圆弧的所有点
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;
    
    // 存储每行的最小和最大x值
    int16_t min_x[ST7735_HEIGHT] = {0};
    int16_t max_x[ST7735_HEIGHT] = {0};
    
    // 初始化数组
    for(int16_t i = 0; i < ST7735_HEIGHT; i++) {
        min_x[i] = ST7735_WIDTH;
        max_x[i] = -1;
    }
    
    // 扫描圆弧边界
    while(x <= y) {
        // 更新四个象限的边界
        if(quadrant_mask & 0x01) { // 右上
            updateBounds(x0 + x, y0 - y, min_x, max_x);
            if(x < y) updateBounds(x0 + y, y0 - x, min_x, max_x);
        }
        if(quadrant_mask & 0x02) { // 左上
            updateBounds(x0 - x, y0 - y, min_x, max_x);
            if(x < y) updateBounds(x0 - y, y0 - x, min_x, max_x);
        }
        if(quadrant_mask & 0x04) { // 左下
            updateBounds(x0 - x, y0 + y, min_x, max_x);
            if(x < y) updateBounds(x0 - y, y0 + x, min_x, max_x);
        }
        if(quadrant_mask & 0x08) { // 右下
            updateBounds(x0 + x, y0 + y, min_x, max_x);
            if(x < y) updateBounds(x0 + y, y0 + x, min_x, max_x);
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
    
    // 填充扇形区域
    for(int16_t row = 0; row < ST7735_HEIGHT; row++) {
        if(min_x[row] <= max_x[row]) {
            LCD_DrawHorLine(min_x[row], max_x[row], row, Pixel_Set, type);
        }
    }
    
    return LCD_SUCCESS;
}

/**
 * @brief 绘制字符
 * @param x0,y0 绘制字符的左上角
 * @param ch 要显示的字符
 * @param font 字体结构体指针
 * @param font_t 字体类型
 * @param Pixel_Set: 绘制图像的颜色(RGB565)
 * @param type: 绘制模式,OLED_Xor 异或模式,OLED_Nor 普通模式
 */
LCD_Event_t LCD_DrawChar(int16_t x0, int16_t y0, char ch, const struFont *font, uint8_t font_t, LCD_Pixel_t Pixel_Set, LCD_Type_t type)
{	
    // 参数检查
    if (font == NULL || ch < ' ' || ch > '~') {
        return LCD_ERROR;
    }

    // 获取字符索引（从空格开始）
    uint8_t char_index = ch - ' ';
    const uint8_t *char_data = NULL;
    
    // 选择字体类型
    switch(font_t) {
        case FONT_Regular:
            char_data = font->type.Font_Regular;
            break;
        case FONT_Italic:
            char_data = font->type.Font_Italic;
            break;
        case FONT_Bold:
            char_data = font->type.Font_Bold;
            break;
        case FONT_Under:
            char_data = font->type.Font_Under;
            break;
        default:
            return LCD_ERROR;
    }
    
    if (char_data == NULL) {
        return LCD_ERROR;
    }
    
    // 定位字符数据
    char_data += char_index * font->height * font->bytes_per_row;

    // 逐像素绘制
    for(uint8_t row = 0; row < font->height; row++) {
        for(uint8_t col = 0; col < font->width; col++) {
            // 计算字节和位索引
            uint8_t byte_index = col / 8;
            uint8_t bit_index = col % 8;  
            uint8_t current_byte = char_data[row * font->bytes_per_row + byte_index];
			
			if(current_byte & (0x01 << bit_index)) 
			{
				if((type == LCD_Xor)&&(LCD_ReadPixel(x0,y0) == Pixel_Set))
					LCD_DrawPixel(x0 + col, y0 + row, ~Pixel_Set);
				else
					LCD_DrawPixel(x0 + col, y0 + row, Pixel_Set);
			} 
        }
    }
    
    return LCD_SUCCESS;
}

/**
 * @brief 绘制字符串
 * @note  坐标 (x, y) = (水平, 垂直)
 */
LCD_Event_t LCD_DrawString(int16_t x0, int16_t y0, const char *str, const struFont *font, uint8_t font_t, LCD_Pixel_t Pixel_Set, LCD_Type_t type)
{
    // 修正坐标系
	uint16_t current_x = x0;
	uint16_t current_y = y0;
	
	while(*str != '\0')
	{
		LCD_DrawChar(current_x, current_y, *str, font, font_t, Pixel_Set, type);
		
		// 水平前进
		current_x += font->width;
	
		// 检查换行
		if(current_x + font->width >= ST7735_WIDTH)
		{
			return LCD_SUCCESS;
//			current_x = x0; // 回到起始 x
//			current_y += font->height; // 换到下一行 (y 增加)
//	
//			if(current_y + font->height >= ST7735_HEIGHT)
//			{
//				break; // 超出屏幕底部
//			}
		}
		
		str++; 
	}
	return LCD_SUCCESS;
}

/**
 * @brief 绘制单色位图图像(使用LCD_DrawPixel)
 * @param x0: 图像左上角X坐标
 * @param y0: 图像左上角Y坐标
 * @param width: 图像宽度(像素)
 * @param height: 图像高度(像素)
 * @param image: 图像数据指针
 * @param type: 绘制模式
 */
LCD_Event_t LCD_DrawImage(int16_t x0, int16_t y0, uint16_t width, uint16_t height, const uint8_t *image, LCD_Pixel_t Pixel_Set, LCD_Type_t type) {
    // 参数边界检查
    if (x0 >= ST7735_WIDTH || y0 >= ST7735_HEIGHT || image == NULL) 
        return LCD_ERROR;
    
    // 计算每行字节数(宽度向上取整到8的倍数)
    uint16_t bytes_per_line = (width + 7) / 8;
    
    for (uint16_t row = 0; row < height; row++) {
        // 检查行是否在屏幕范围内
        int16_t current_y = y0 + row;
        if (current_y < 0 || current_y >= ST7735_HEIGHT) continue;
        
        for (uint16_t col = 0; col < width; col++) {
            // 检查列是否在屏幕范围内
            int16_t current_x = x0 + col;
            if (current_x < 0 || current_x >= ST7735_WIDTH) continue;
            
            // 计算字节和位位置
            uint16_t byte_index = row * bytes_per_line + (col / 8);
            uint8_t bit_index = 7 - (col % 8); // 通常位图数据是MSB在前
            
            uint8_t current_byte = image[byte_index];
			
			if(current_byte & (0x01 << bit_index)) {
				if((type == LCD_Xor)&&(LCD_ReadPixel(x0,y0) == Pixel_Set))
					LCD_DrawPixel(x0 + col, y0 + row, ~Pixel_Set);
				else
					LCD_DrawPixel(x0 + col, y0 + row, Pixel_Set);
			} 
			
        }
    }
    return LCD_SUCCESS; // 添加缺失的分号
}

#include "st7735.h"
#include <stdbool.h>
#include <stdlib.h> // For abs()
#include <string.h> // For memset()

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

// 帧缓冲区：128x128 像素, 16位色 (RGB565), 2 字节/像素
uint8_t frame[ST7735_WIDTH * ST7735_HEIGHT * 2] = {0x00};
// 脏矩阵,高位在前,如果位是1代表需要刷新
uint8_t frame_bit[ST7735_HEIGHT][ST7735_WIDTH / 8] = {0x00};

// --- 低级SPI函数 ---

void ST7735_Reset(void){
	HAL_GPIO_WritePin(ST7735_RST_GPIO_Port, ST7735_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(ST7735_RST_GPIO_Port, ST7735_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
}

void ST7735_WriteCommand(uint8_t cmd){
	HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&ST7735_SPI_INSTANCE, &cmd, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

void ST7735_WriteByte(uint8_t data){
  HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&ST7735_SPI_INSTANCE, &data, 1, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

void ST7735_WriteData(uint8_t *data, size_t data_size){
  HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&ST7735_SPI_INSTANCE, data, data_size, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

/**
 * @brief 设置屏幕旋转
 * @note  修复了 ST7735_MADCTL_MODE 拼写错误 (假设为 BGR)
 */
void ST7735_SetRotation(uint8_t rotation){
	uint8_t madctl = 0;

	switch (rotation)
	{
	case 0:
		madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_MODE;
		break;
	case 1:
		madctl = ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_MODE;
		break;
	case 2:
		madctl = ST7735_MADCTL_MODE;
		break;
	case 3:
		madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_MODE;
		break;
}

	ST7735_WriteCommand(ST7735_MADCTL);
	ST7735_WriteByte(madctl);
}

/**
  * @brief 初始化
  * @note  修正了 RASET 结束地址以匹配 128x128 缓冲区
  */
void ST7735_Init(void) {
	ST7735_Reset();
	ST7735_WriteCommand(ST7735_SLPOUT);
	HAL_Delay(120);
	ST7735_WriteCommand(ST7735_FRMCTR1);
	ST7735_WriteByte(0x01);
	ST7735_WriteByte(0x2C);
	ST7735_WriteByte(0x2D);
	ST7735_WriteCommand(ST7735_FRMCTR2);
	ST7735_WriteByte(0x01);
	ST7735_WriteByte(0x2C);
	ST7735_WriteByte(0x2D);
	ST7735_WriteCommand(ST7735_FRMCTR3);
	ST7735_WriteByte(0x01);
	ST7735_WriteByte(0x2C);
	ST7735_WriteByte(0x2D);
	ST7735_WriteByte(0x01);
	ST7735_WriteByte(0x2C);
	ST7735_WriteByte(0x2D);
	ST7735_WriteCommand(ST7735_INVCTR);
	ST7735_WriteByte(0x07);
	ST7735_WriteCommand(ST7735_PWCTR1);
	ST7735_WriteByte(0xA2);
	ST7735_WriteByte(0x02);
	ST7735_WriteByte(0x84);
	ST7735_WriteCommand(ST7735_PWCTR2);
	ST7735_WriteByte(0xC5);
	ST7735_WriteCommand(ST7735_PWCTR3);
	ST7735_WriteByte(0x0A);
	ST7735_WriteByte(0x00);
	ST7735_WriteCommand(ST7735_PWCTR4);
	ST7735_WriteByte(0x8A);
	ST7735_WriteByte(0x2A);
	ST7735_WriteCommand(ST7735_PWCTR5);
	ST7735_WriteByte(0x8A);
	ST7735_WriteByte(0xEE);
	ST7735_WriteCommand(ST7735_VMCTR1);
	ST7735_WriteByte(0x0E);
	ST7735_WriteCommand(ST7735_INVERSE ? ST7735_INVON : ST7735_INVOFF);
	ST7735_WriteCommand(ST7735_COLMOD);
	ST7735_WriteByte(0x05); // 16-bit color
	ST7735_WriteCommand(ST7735_CASET);
	ST7735_WriteByte(0x00);
	ST7735_WriteByte(0x00);
	ST7735_WriteByte(0x00);
	ST7735_WriteByte(ST7735_WIDTH - 1); // 0x7F for 128
	ST7735_WriteCommand(ST7735_RASET);
	ST7735_WriteByte(0x00);
	ST7735_WriteByte(0x00);
	ST7735_WriteByte(0x00);
	ST7735_WriteByte(ST7735_HEIGHT - 1); // 0x7F for 128
	ST7735_WriteCommand(ST7735_GMCTRP1);
	ST7735_WriteByte(0x02);
	ST7735_WriteByte(0x1C);
	ST7735_WriteByte(0x07);
	ST7735_WriteByte(0x12);
	ST7735_WriteByte(0x37);
	ST7735_WriteByte(0x32);
	ST7735_WriteByte(0x29);
	ST7735_WriteByte(0x2D);
	ST7735_WriteByte(0x29);
	ST7735_WriteByte(0x25);
	ST7735_WriteByte(0x2B);
	ST7735_WriteByte(0x39);
	ST7735_WriteByte(0x00);
	ST7735_WriteByte(0x01);
	ST7735_WriteByte(0x03);
	ST7735_WriteByte(0x10);
	ST7735_WriteCommand(ST7735_GMCTRN1);
	ST7735_WriteByte(0x03);
	ST7735_WriteByte(0x1D);
	ST7735_WriteByte(0x07);
	ST7735_WriteByte(0x06);
	ST7735_WriteByte(0x2E);
	ST7735_WriteByte(0x2C);
	ST7735_WriteByte(0x29);
	ST7735_WriteByte(0x2D);
	ST7735_WriteByte(0x2E);
	ST7735_WriteByte(0x2E);
	ST7735_WriteByte(0x37);
	ST7735_WriteByte(0x3F);
	ST7735_WriteByte(0x00);
	ST7735_WriteByte(0x00);
	ST7735_WriteByte(0x02);
	ST7735_WriteByte(0x10);
	ST7735_WriteCommand(ST7735_NORON);
	HAL_Delay(10);
	ST7735_WriteCommand(ST7735_DISPON);
	HAL_Delay(10);
	ST7735_SetRotation(ST7735_ROTATION);
	ST7735_FillScreen(ST7735_BLACK);
}

/**
  * @brief 创建绘图区域
  * @note  坐标 (x, y) = (水平, 垂直)
  */
void ST7735_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){
	uint8_t data[] = {0x00, 0x00, 0x00, 0x00};
	
	x0 += ST7735_XSTART;
	x1 += ST7735_XSTART;
	y0 += ST7735_YSTART;
	y1 += ST7735_YSTART;
	
	ST7735_WriteCommand(ST7735_CASET); // Column
	data[1] = x0;
	data[3] = x1;
	ST7735_WriteData(data, sizeof(data));

	ST7735_WriteCommand(ST7735_RASET); // Row
	data[1] = y0;
	data[3] = y1;
	ST7735_WriteData(data, sizeof(data));
}

// --- 绘图函数 ---

/**
 * @brief  [新] 内部辅助函数：在帧缓冲区绘制一个像素
 * @note   坐标 (x, y) = (水平, 垂直)
 */
static void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color){
	if (x >= ST7735_WIDTH || y >= ST7735_HEIGHT) {
		return; // 超出边界
    }

    // 帧缓冲区索引：frame[y][x]
    uint32_t index = (y * ST7735_WIDTH + x) * 2;
    
    frame[index]     = color >> 8;   // 高字节
    frame[index + 1] = color & 0xFF; // 低字节	
	frame_bit[y][x / 8] |= (0x80 >> (x % 8));
}

/**
 * @brief  [新] 内部辅助函数：在帧缓冲区绘制一条水平线
 * @note   坐标 (x, y) = (水平, 垂直)
 */
static void ST7735_DrawHorizontalLine(int16_t x0, int16_t x1, int16_t y, uint16_t color){
	if (x0 > x1) {
		int16_t temp = x0;
        x0 = x1;
        x1 = temp;
    }
    for (int16_t x = x0; x <= x1; x++) {
        ST7735_DrawPixel(x, y, color);
    }
}

/**
 * @brief 填充整个帧缓冲区
 * @note  使用 memset 优化常见颜色的填充,自己填充缓冲区,然后刷新,不需要再次调用DrawFrame
 */
void ST7735_FillScreen(uint16_t color) {
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    // 如果高低字节相同（例如 0x0000 黑色, 0xFFFF 白色），memset 速度极快
    if (hi == lo) {
		memset(frame, hi, sizeof(frame));
    } else {
		// 否则，循环填充
        for (uint32_t i = 0; i < sizeof(frame); i += 2) {
            frame[i] = hi;
            frame[i + 1] = lo;
        }
    }
	ST7735_SetAddressWindow(0, 0, ST7735_WIDTH - 1, ST7735_HEIGHT - 1);
	ST7735_WriteCommand(ST7735_RAMWR);
	ST7735_WriteData(frame, sizeof(frame));
}

/**
  * @brief 绘制图像 (直接写入屏幕)
  * @note  此函数绕过帧缓冲区，用于从Flash绘制大图像
 * @note  坐标 (x, y) = (水平, 垂直)
  */
void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *image){
	ST7735_SetAddressWindow(x, y, x + width - 1, y + height - 1);
	ST7735_WriteCommand(ST7735_RAMWR);
	ST7735_WriteData((uint8_t *)image, width * height * 2);
}

/**
 * @brief 绘制字符
 * @note  坐标 (x, y) = (水平, 垂直)
 */
void ST7735_DrawChar(uint16_t x, uint16_t y, char ch, uint16_t color, const struFont *font, uint8_t type){
	uint8_t char_index = ch - ' ';
	const uint8_t *char_data ;
	switch(type){
		case(0):char_data = font->data0 + char_index * font->height * font->bytes_per_row;
			break;
		case(1):char_data = font->data1 + char_index * font->height * font->bytes_per_row;
			break;
		case(2):char_data = font->data2 + char_index * font->height * font->bytes_per_row;
			break;
		case(3):char_data = font->data3 + char_index * font->height * font->bytes_per_row;
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
                // 修正坐标系：
                // x (水平基址) + col (水平偏移)
                // y (垂直基址) + row (垂直偏移)
				ST7735_DrawPixel(x + col, y + row, color);
			}
		}
	}
}

/**
 * @brief 绘制字符串
 * @note  坐标 (x, y) = (水平, 垂直)
 */
void ST7735_DrawString(uint16_t x, uint16_t y, const char *str, uint16_t color, const struFont *font, uint8_t type)
{
    // 修正坐标系
	uint16_t current_x = x;
	uint16_t current_y = y;
	
	while(*str != '\0')
	{
		ST7735_DrawChar(current_x, current_y, *str, color, font, type);
		
		// 水平前进
		current_x += font->width;
	
		// 检查换行
		if(current_x + font->width >= ST7735_WIDTH)
		{
			current_x = x; // 回到起始 x
			current_y += font->height; // 换到下一行 (y 增加)
	
			if(current_y + font->height >= ST7735_HEIGHT)
			{
				break; // 超出屏幕底部
			}
		}
		
		str++; 
	}
}

/**
 * @brief 绘制四分之一圆弧 - Bresenham算法 (优化)
 * @note  坐标 (x0, y0) = (水平中心, 垂直中心)
 * @param quadrant_mask: 位掩码 0x01=右上, 0x02=左上, 0x04=左下, 0x08=右下
 */
void ST7735_DrawQuarterArc(uint16_t x0, uint16_t y0, uint16_t r, uint8_t quadrant_mask, uint16_t color)
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
            ST7735_DrawPixel(x0 + x, y0 - y, color);
            ST7735_DrawPixel(x0 + y, y0 - x, color);
        }
        // 象限 2 (左上): (-x, -y), (-y, -x)
        if (quadrant_mask & 0x02) { 
            ST7735_DrawPixel(x0 - x, y0 - y, color);
            ST7735_DrawPixel(x0 - y, y0 - x, color);
        }
        // 象限 3 (左下): (-x, +y), (-y, +x)
        if (quadrant_mask & 0x04) { 
            ST7735_DrawPixel(x0 - x, y0 + y, color);
            ST7735_DrawPixel(x0 - y, y0 + x, color);
        }
        // 象限 4 (右下): (+x, +y), (+y, +x)
        if (quadrant_mask & 0x08) { 
            ST7735_DrawPixel(x0 + x, y0 + y, color);
            ST7735_DrawPixel(x0 + y, y0 + x, color);
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
void ST7735_DrawQuarterSector(uint16_t x0, uint16_t y0, uint16_t r, uint8_t quadrant_mask, uint16_t color)
{
	int16_t x = 0;
	int16_t y = r;
	int16_t d = 3 - 2 * r;
	
	// 优化：使用水平扫描线填充，而不是径向线
	while(x <= y) {
		// 象限 1 (右上)
		if(quadrant_mask & 0x01) {
            ST7735_DrawHorizontalLine(x0, x0 + x, y0 - y, color);
            ST7735_DrawHorizontalLine(x0, x0 + y, y0 - x, color);
		}
		// 象限 2 (左上)
		if(quadrant_mask & 0x02) {
            ST7735_DrawHorizontalLine(x0 - x, x0, y0 - y, color);
            ST7735_DrawHorizontalLine(x0 - y, x0, y0 - x, color);
		}
		// 象限 3 (左下)
		if(quadrant_mask & 0x04) {
            ST7735_DrawHorizontalLine(x0 - x, x0, y0 + y, color);
            ST7735_DrawHorizontalLine(x0 - y, x0, y0 + x, color);
		}
		// 象限 4 (右下)
		if(quadrant_mask & 0x08) {
            ST7735_DrawHorizontalLine(x0, x0 + x, y0 + y, color);
            ST7735_DrawHorizontalLine(x0, x0 + y, y0 + x, color);
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
 * @brief 绘制直线 - Bresenham算法
 * @note  坐标 (x, y) = (水平, 垂直)
 */
void ST7735_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
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
		ST7735_DrawPixel(x0, y0, color);
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
void ST7735_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
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
			ST7735_DrawPixel(x, y, color);
		}
	}
}

/**
 * @brief 刷新 - 将帧缓冲区推送到屏幕
 * @note  优化：修正了函数签名，并使用 WIDTH/HEIGHT 宏
 */
void ST7735_DrawFrame(void)
{
    uint16_t min_x = ST7735_WIDTH;
    uint16_t max_x = 0;
    uint16_t min_y = ST7735_HEIGHT;
    uint16_t max_y = 0;
    bool is_dirty = false; // 标记是否有任何像素被修改

    // 1. 遍历脏矩阵 (frame_bit)，查找脏区域的边界
    for (uint16_t y = 0; y < ST7735_HEIGHT; y++)
    {
        for (uint16_t x_byte = 0; x_byte < (ST7735_WIDTH / 8); x_byte++)
        {
            uint8_t dirty_byte = frame_bit[y][x_byte];
            
            if (dirty_byte != 0)
            {
                is_dirty = true; // 标记为脏

                // 更新 Y 边界
                if (y < min_y) min_y = y;
                if (y > max_y) max_y = y;

                // 查找此字节中具体的 X 边界 (从左到右检查8个位)
                for (uint8_t bit = 0; bit < 8; bit++)
                {
                    // 检查MSB (高位在前)
                    if (dirty_byte & (0x80 >> bit)) 
                    {
                        uint16_t x = (x_byte * 8) + bit;
                        if (x < min_x) min_x = x;
                        if (x > max_x) max_x = x;
                    }
                }
            }
        }
    }

    // 2. 如果没有脏像素，则直接返回
    if (!is_dirty)
    {
        return; // 屏幕是干净的，无需刷新
    }

    // 3. 设置地址窗口为我们找到的最小脏矩形
    ST7735_SetAddressWindow(min_x, min_y, max_x, max_y);

    // 4. 准备写入RAM
    ST7735_WriteCommand(ST7735_RAMWR);

    // 5. 逐行发送脏矩形区域的像素数据
    //    对于窗口内的每一行 (y)，其 (min_x 到 max_x) 
    //    的像素数据在 'frame' 缓冲区中是连续的。
    
    uint16_t width_bytes = (max_x - min_x + 1) * 2; // 每个像素2字节

    for (uint16_t y = min_y; y <= max_y; y++)
    {
        // 计算该行脏区域在 'frame' 中的起始内存地址
        uint32_t start_index = (y * ST7735_WIDTH + min_x) * 2;
        
        // 发送这一行连续的像素数据
        ST7735_WriteData(&frame[start_index], width_bytes);
    }
    
    // 6. 刷新完成后，清除整个脏矩阵
    memset(frame_bit, 0, sizeof(frame_bit));
}

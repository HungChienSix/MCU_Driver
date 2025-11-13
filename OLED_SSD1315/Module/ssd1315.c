#include "ssd1315.h"
#include <string.h>

// 屏幕显示缓冲区 [页][列]
static uint8_t display_ram[SSD1315_PAGES][SSD1315_WIDTH] = {0};

#ifdef SSD1315_PARTIAL_REFRESH
    static uint8_t page_checksum[SSD1315_PAGES] = {0x00}; // 页校验码数组
#endif

/**
 * @brief 发送命令到SSD1315
 * @param cmd 要发送的命令
 * @return 
 */
static void SSD1315_WriteCmd(uint8_t cmd){
	HAL_I2C_Mem_Write(&SSD1315_I2C, SSD1315_Dev_Addr, SSD1315_MemCmd_Addr, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 10);
}

/**
 * @brief 发送数据到SSD1315
 * @param data 数据指针
 * @param size 数据大小
 * @return 
 */
static void SSD1315_WriteData(uint8_t *data, uint16_t size){
	HAL_I2C_Mem_Write(&SSD1315_I2C, SSD1315_Dev_Addr, SSD1315_MemData_Addr, I2C_MEMADD_SIZE_8BIT, data, size, 500);
}

/**
	* @brief 设置OLED屏幕方向
	* @param rotation 0x00：正常方向 0x11：左右相反，上下颠倒
	*/
void SSD1315_SetRotation(uint8_t rotation){
	if(rotation == 0x00){
		SSD1315_WriteCmd(0xA0); // 正常列映射
		SSD1315_WriteCmd(0xC0); // 正常行映射
	}
	else if(rotation == 0x11){
		SSD1315_WriteCmd(0xA1); 
		SSD1315_WriteCmd(0xC8); 
	}
}
		
/**
	* @brief 初始化OLED屏幕
	* @param
	*/
void OLED_Init(void){
	HAL_Delay(100);
	SSD1315_WriteCmd(0xAE);//0xAE Set Display ON ,0xAF Set Display OFF 
	
	SSD1315_WriteCmd(0x00);//set low  column address 
	SSD1315_WriteCmd(0x10);//set high column address 
	
	SSD1315_WriteCmd(0x20); SSD1315_WriteCmd(0x10); // Horizontal Addressing Mode 
	//SSD1315_WriteCmd(0x21); SSD1315_WriteCmd(0x00); SSD1315_WriteCmd(SSD1315_WIDTH-1); // Col start and end .Only for Other Addressing Mode 
	//SSD1315_WriteCmd(0x22); SSD1315_WriteCmd(0x00); SSD1315_WriteCmd(SSD1315_PAGES-1); // Page start and end .Only for Other Addressing Mode 
	
	SSD1315_WriteCmd(0xA8); SSD1315_WriteCmd(0x3F); // Set Multiplex Ratio 0x3F+0x01 == 64d
	SSD1315_WriteCmd(0xD3); SSD1315_WriteCmd(0x00); // Set Display Offset
	SSD1315_WriteCmd(0x40); // Set display RAM display start line 
	SSD1315_SetRotation(SSD1315_ROTATION);
	SSD1315_WriteCmd(0xDA); SSD1315_WriteCmd(0x12); // Set com pins hardware configuration 
	
	SSD1315_WriteCmd(0xD5); SSD1315_WriteCmd(0x80); // Set Display Clock Divide Ratio/Oscillator Frequency
	SSD1315_WriteCmd(0xD9); SSD1315_WriteCmd(0xF1); // Set Pre-charge Period
	SSD1315_WriteCmd(0xDB); SSD1315_WriteCmd(0x30); // Set COM select voltage level
	
	SSD1315_WriteCmd(0x81); SSD1315_WriteCmd(SSD1315_Brightness); // Set Contrast Control
	SSD1315_WriteCmd(0x8D); SSD1315_WriteCmd(0x14); // Enable internal charge pump
	
	SSD1315_WriteCmd(0xA4); // if A5 Entire display ON 
	SSD1315_WriteCmd(SSD1315_INVERSE); // if A7 inverse display
	
	SSD1315_WriteCmd(0xAF);//if AE open the sleep mode
	
	OLED_FillScreen(OLED_OFF);
	OLED_RefreshScreen_Force();
	HAL_Delay(100);
}



/**
 * @brief 在缓冲区读取像素点状态
 * @param x: X坐标(0-127)
 * @param y: Y坐标(0-63)
 * @return 像素状态: OLED_ON=点亮, OLED_OFF=熄灭, OLED_ERROR=坐标无效
 */
OLED_Pixel_t OLED_ReadPixel(uint16_t x, uint16_t y)
{
    // 参数检查
    if (x >= SSD1315_WIDTH || y >= SSD1315_HEIGHT) 
        return OLED_ERROR;
    
    uint8_t page = y / SSD1315_PAGE_HEIGHT;
    uint8_t bit_mask = 1 << (y % SSD1315_PAGE_HEIGHT);
    
    return (display_ram[page][x] & bit_mask) ? OLED_ON : OLED_OFF;
}

/**
 * @brief 绘制像素点
 * @param x, y: 坐标
 * @param Pixel_Set: 像素设置
 * @param Type: 绘制类型
 * @return 最终像素状态
 */
OLED_Pixel_t OLED_DrawPixel(uint16_t x, uint16_t y, OLED_Pixel_t Pixel_Set, uint8_t Type)
{
    // 参数检查
    if (x >= SSD1315_WIDTH || y >= SSD1315_HEIGHT) 
        return OLED_ERROR;
    
	// 读像素值 
    uint8_t page = y / SSD1315_PAGE_HEIGHT;
    uint8_t bit_mask = 1 << (y % SSD1315_PAGE_HEIGHT);
    uint8_t *pixel_byte = &display_ram[page][x];
    OLED_Pixel_t current_pixel = (*pixel_byte & bit_mask) ? OLED_ON : OLED_OFF;
	
	// 排除不绘制的情况
    if (!Pixel_Set && !(Type & OLED_Over)) {
        return OLED_IDLE;
    }
    
	// 绘制
    switch (Type & 0x0F) { 
        case OLED_Normal:
            if (Pixel_Set) {
                *pixel_byte |= bit_mask;
                return OLED_ON;
            } else {
                *pixel_byte &= ~bit_mask;
                return OLED_OFF;
            }
            break;
            
        case OLED_Inverse:
            if (Pixel_Set) {
                *pixel_byte &= ~bit_mask;
                return OLED_OFF;
            } else {
                *pixel_byte |= bit_mask;
                return OLED_ON;
            }
            break;
            
        case OLED_XOR:
			if(Pixel_Set){
				if (current_pixel) {
					*pixel_byte &= ~bit_mask;
					return OLED_OFF;
				} else {
					*pixel_byte |= bit_mask;
					return OLED_ON;
				}
			}
            break;
            
        default:
            return OLED_IDLE;
    }
}

/**
 * @brief 绘制水平线 (优化版本)
 */
OLED_Pixel_t OLED_DrawHorizontalLine(int16_t x0, int16_t x1, int16_t y, OLED_Pixel_t SetPixel, uint8_t Type)
{
    // 参数检查
    if((x0 >= SSD1315_WIDTH)||(x1 < 0)||(x0 > x1))
		return OLED_ERROR;
    if((y >= SSD1315_HEIGHT)||(y < 0))
		return OLED_ERROR;
    
    // 对x进行裁剪
    if (x0 < 0) x0 = 0;
    if (x1 >= SSD1315_WIDTH) x1 = SSD1315_WIDTH - 1;
    
	// 绘制
    for(int16_t x = x0; x <= x1; x++) {
        OLED_DrawPixel(x, y, SetPixel, Type);
    }
	return OLED_SUCCESS;
}

/**
 * @brief 绘制垂直线 (优化版本)
 */
OLED_Pixel_t OLED_DrawVerticalLine(int16_t x, int16_t y0, int16_t y1, OLED_Pixel_t SetPixel, uint8_t Type)
{
	// 参数检查
    if((y0 >= SSD1315_WIDTH)||(y1 < 0)||(y0 > y1))
		return OLED_ERROR;
    if((x >= SSD1315_HEIGHT)||(x < 0))
		return OLED_ERROR;
    
    // 对y进行裁剪
    if (y0 < 0) y0 = 0;
    if (y1 >= SSD1315_HEIGHT) y1 = SSD1315_HEIGHT - 1;
    
    for(int16_t y = y0; y <= y1; y++) {
        OLED_DrawPixel(x, y, SetPixel, Type);
    }
	return OLED_SUCCESS;
}

/**
 * @brief 填充矩形区域 (优化版本)
 */
OLED_Pixel_t OLED_FillArea(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, OLED_Pixel_t SetPixel, uint8_t Type)
{
    // 参数检查
    if((y0 >= SSD1315_WIDTH)||(y1 < 0)||(y0 > y1))
		return OLED_ERROR;
    if((x0 >= SSD1315_WIDTH)||(x1 < 0)||(x0 > x1))
		return OLED_ERROR;
    
    // 裁剪
    if (x0 < 0) x0 = 0;
    if (x1 >= SSD1315_WIDTH) x1 = SSD1315_WIDTH - 1;
    if (y0 < 0) y0 = 0;
    if (y1 >= SSD1315_HEIGHT) y1 = SSD1315_HEIGHT - 1;
    
    for (uint16_t y = y0; y <= y1; y++) {
        for(uint16_t x = x0; x <= x1; x++) {
            OLED_DrawPixel(x, y, SetPixel, Type);
        }
    }
	return OLED_SUCCESS;
}

/**
 * @brief 填充整个屏幕
 */
void OLED_FillScreen(OLED_Pixel_t SetPixel)
{
    memset(display_ram, SetPixel ? 0xFF : 0x00, sizeof(display_ram));
}

#ifdef SSD1315_PARTIAL_REFRESH
/**
 * @brief 计算页数据的校验码（简单异或校验，更快）
 */
uint8_t OLED_CalculateChecksum(uint8_t page)
{
    uint8_t checksum = 0;
    for(int i = 0; i < SSD1315_WIDTH; i++) {
        checksum ^= display_ram[page][i];
    }
    return checksum;
}
#endif

/**
 * @brief 刷新校验码变化的屏幕
 */
void OLED_RefreshScreen(void)
{
#ifdef SSD1315_PARTIAL_REFRESH  // 修复宏名称
    for (uint8_t page = 0; page < SSD1315_PAGES; page++) {
        uint8_t current_checksum = OLED_CalculateChecksum(page);
        
        if (current_checksum != page_checksum[page]) {
            SSD1315_WriteCmd(0xB0 | page); // 设置页地址
            SSD1315_WriteCmd(0x00);        // 设置列地址低4位
            SSD1315_WriteCmd(0x10);        // 设置列地址高4位
            SSD1315_WriteData(display_ram[page], SSD1315_WIDTH);
            page_checksum[page] = current_checksum;
        }
    }
#else
    OLED_RefreshScreen_Force();
#endif
}

/**
 * @brief 强制刷新整个屏幕
 */
void OLED_RefreshScreen_Force(void)
{
    for(uint8_t page = 0; page < SSD1315_PAGES; page++) {
        SSD1315_WriteCmd(0xB0 | page); // 设置页地址
//        SSD1315_WriteCmd(0x00);        // 设置列地址低4位  
//        SSD1315_WriteCmd(0x10);        // 设置列地址高4位
        SSD1315_WriteData(display_ram[page], SSD1315_WIDTH);
    }
}

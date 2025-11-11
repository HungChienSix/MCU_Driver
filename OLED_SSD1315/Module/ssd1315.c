#include "ssd1315.h"
#include "string.h"

uint8_t display_ram[8][128] = {0}; //屏幕显示缓冲区

static inline void  SSD1315_WriteCmd(uint8_t cmd){
	HAL_I2C_Mem_Write(&SSD1315_I2C, SSD1315_Dev_Addr, SSD1315_MemCmd_Addr, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 10);
}

static inline void  SSD1315_WriteData(uint8_t *data, uint16_t size){
	HAL_I2C_Mem_Write(&SSD1315_I2C, SSD1315_Dev_Addr, SSD1315_MemData_Addr, I2C_MEMADD_SIZE_8BIT, data, size, 500);
}

/**
	* @brief 设置OLED屏幕方向
	* @param
	*/
void SSD1315_SetRotation(uint8_t rotation){
	if(rotation == 0x00){
		SSD1315_WriteCmd(0xA0); // Set SEG/Column Mapping /A1 is inverted left and right
		SSD1315_WriteCmd(0xC0); // Set COM Mapping /C8 is inverted left and right
	}
	else 	if(rotation == 0x11){
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
	
	SSD1315_WriteCmd(0x81); SSD1315_WriteCmd(SSD315_Brightness); // Set Contrast Control
	SSD1315_WriteCmd(0x8D); SSD1315_WriteCmd(0x14); // Enable internal charge pump
	
	SSD1315_WriteCmd(0xA4); // if A5 Entire display ON 
	SSD1315_WriteCmd(SSD1315_INVERSE); // if A7 inverse display
	
	SSD1315_WriteCmd(0xAF);//if AE open the sleep mode
	
	OLED_FillScreen(OLED_OFF);
	OLED_RefreshScreen_Force();
	
}

/**
 * @brief 在缓冲区读取像素点状态
 * @param x: X坐标(0-127)
 * @param y: Y坐标(0-63)
 * @return 像素状态: 1=点亮, 0=熄灭, 0xFF=坐标无效
 */
uint8_t OLED_ReadPixel(uint16_t x, uint16_t y) {
    // 参数边界检查
    if (x >= SSD1315_WIDTH || y >= SSD1315_HEIGHT) {
        return 0xFF; // 返回错误值
    }
    
    uint8_t page = y >> 3;  // y / 8
    uint8_t bit_mask = 1 << (y & 0x07); // y % 8
    
    // 读取像素状态
    if (display_ram[page][x] & bit_mask) {
        return 1; // 像素点亮
    } else {
        return 0; // 像素熄灭
    }
}

/**
	* @brief 在缓冲区绘制像素点
  * @param x: X坐标, y: Y坐标, SetPixel: 填充值
  */
void OLED_DrawPixel(uint16_t x, uint16_t y, uint8_t SetPixel) {
	if (x >= SSD1315_WIDTH || y >= SSD1315_HEIGHT) return;
	
	uint8_t page = y >> 3;  // y / 8
	uint8_t bit_mask = 1 << (y & 0x07); // y % 8
	
	if (SetPixel) {
			display_ram[page][x] |= bit_mask;
	} else {
			display_ram[page][x] &= ~bit_mask;
	}
}

/**
	* @brief 设置缓冲区某一水平线区域
  * @param SetPixel: 填充值
  */
void OLED_DrawHorizontalLine (int16_t x0, int16_t x1, int16_t y, uint8_t SetPixel) {	
	for(uint16_t n = x0; n <= x1; n++) {
		uint8_t page = y >> 3;  // y / 8
		uint8_t bit_mask = 1 << (y & 0x07); // y % 8
		
		if (SetPixel) {
			display_ram[page][n] |= bit_mask;
		} else {
			display_ram[page][n] &= ~bit_mask;
		}
	}
}

/**
	* @brief 设置缓冲区某一区域的值
  * @param SetPixel: 填充值
  */
void OLED_FillArea (uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t SetPixel) {	
	for (uint16_t m = y0; m <= y1; m++) {
		for(uint16_t n = x0; n <= x1; n++) {
			uint8_t page = m >> 3;  // y / 8
			uint8_t bit_mask = 1 << (m & 0x07); // y % 8
			
			if (SetPixel) {
				display_ram[page][n] |= bit_mask;
			} else {
				display_ram[page][n] &= ~bit_mask;
			}
		}
	}
}

/**
	* @brief 设置缓冲区全部的值
  * @param SetPixel: 填充值
  */
void OLED_FillScreen(uint8_t SetPixel){
	memset(display_ram, SetPixel ? 0xFF : 0x00, sizeof(display_ram));
}

#ifdef PartRefresh
	static uint8_t page_checksum[SSD1315_PAGES] = {0x00}; // 定义校验码数组
	
/**
  * @brief 计算页数据的校验码（简单CRC8）
  * @param page: 页号(0-7)
  * @return 校验码
  */
uint8_t OLED_CalculateChecksum(uint8_t page) {
	uint8_t crc = 0xFF;  // 初始值
	uint8_t polynomial = 0x07;  // CRC-8多项式
	
	for(int i = 0; i < 128; i++) {
		crc ^= display_ram[page][i];
		
		for(int j = 0; j < 8; j++) {
			if(crc & 0x80) {
				crc = (crc << 1) ^ polynomial;
			} else {
				crc <<= 1;
			}
		}
	}
	
	return crc;
}
#endif

/**
  * @brief 刷新校验码变化的屏幕(更新校验码)
  */
void OLED_RefreshScreen(void) {
#ifdef PartRefresh
    for (uint8_t page = 0; page < SSD1315_PAGES; page++) {
        uint8_t current_checksum = OLED_CalculateChecksum(page);
        
        if (current_checksum != page_checksum[page]) {
            SSD1315_WriteCmd(0xB0 | page); // 设置页地址
            SSD1315_WriteData(display_ram[page], SSD1315_WIDTH);
            page_checksum[page] = current_checksum;
        }
    }
#else
    OLED_RefreshScreen_Force();
#endif
}

/**
	* @brief 刷新整个屏幕(不更新校验码)
  * @param
  */
void OLED_RefreshScreen_Force(void) {
    for(uint8_t m = 0; m < SSD1315_PAGES; m++) {
        SSD1315_WriteCmd(0xB0 + m);
        SSD1315_WriteData(&display_ram[m][0], 128);
    }
}

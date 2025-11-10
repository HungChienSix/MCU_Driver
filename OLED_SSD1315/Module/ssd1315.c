#include "ssd1315.h"
#include "string.h"

uint8_t display_ram[8][128] = {0}; //屏幕显示缓冲区
	
void SSD1315_WriteCmd(uint8_t cmd){
	HAL_I2C_Mem_Write(&SSD1315_I2C, SSD1315_Dev_Addr, SSD1315_MemCmd_Addr, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 10);
}
	
void SSD1315_WriteData(uint8_t *data, uint16_t size){
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
	
	OLED_FillScreen(0);
	HAL_I2C_Mem_Write(&SSD1315_I2C, SSD1315_Dev_Addr, SSD1315_MemData_Addr, I2C_MEMADD_SIZE_8BIT, (uint8_t*)display_ram, 1024, 500);
}

/**
	* @brief 绘制像素点
	* @param
	*/
void OLED_DrawPixel(uint8_t x, uint8_t y, uint8_t SetPixel){
	if((x > SSD1315_WIDTH)|(y > SSD1315_HEIGHT))
		return;
	if(SetPixel){
		display_ram[y/8][x] |= (0x01 << (y%8));
	}
	else{
		display_ram[y/8][x] |= ~(0x01 << (y%8));
	}
}

/**
	* @brief 覆盖屏幕
	* @param
	*/
void OLED_FillScreen(uint8_t SetPixel){
	if(SetPixel){
		memset(display_ram, 0xFF, 1024);
	}
	else{
		memset(display_ram, 0x00, 1024);
	}
}

// 定义校验码数组
static uint8_t page_checksum[SSD1315_PAGES] = {0x00};

/**
  * @brief 计算页数据的校验码（简单的异或校验）
  * @param page: 页号(0-7)
  * @return 校验码
  */
uint8_t OLED_CalculateChecksum(uint8_t page) {
    uint8_t checksum = 0;
    for(int i = 0; i < 128; i++) {
        checksum ^= display_ram[page][i];
    }
    return checksum;
}

uint8_t OLED_CalculateChecksum(uint8_t page) {
    uint8_t crc = 0x00;
    
    for(int i = 0; i <= SSD1315_WIDTH; i++) {
        crc = crc8_table[crc ^ display_ram[page][i]];
    }
    
    return crc;
}

/**
  * @brief 刷新屏幕（带校验码检查）
  * @param
  */
void OLED_RefreshScreen(void) {
    for(uint8_t m = 0; m < SSD1315_PAGES; m++) {
        uint8_t current_checksum = OLED_CalculateChecksum(m);
        
        // 只有校验码改变时才刷新该页
        if(current_checksum != page_checksum[m]) {
            SSD1315_WriteCmd(0xB0 + m);
            SSD1315_WriteData(&display_ram[m][0], 128);
            page_checksum[m] = current_checksum; // 更新校验码
        }
    }
}

/**
  * @brief 强制刷新整个屏幕（忽略校验码）
  * @param
  */
void OLED_RefreshScreen_Force(void) {
    for(uint8_t m = 0; m < SSD1315_PAGES; m++) {
        SSD1315_WriteCmd(0xB0 + m);
        SSD1315_WriteData(&display_ram[m][0], 128);
        page_checksum[m] = OLED_CalculateChecksum(m); // 更新校验码
    }
}

/**
  * @brief 刷新指定区域（带校验码检查）
  * @param start_page: 起始页(0-7)
  * @param end_page: 结束页(0-7)
  * @param start_col: 起始列(0-127)
  * @param end_col: 结束列(0-127)
  */
void OLED_RefreshArea(uint8_t start_page, uint8_t end_page, uint8_t start_col, uint8_t end_col) {
    // 参数检查
    if(start_page > end_page || start_col > end_col) return;
    if(end_page >= SSD1315_PAGES || end_col >= 128) return;
    
    uint8_t width = end_col - start_col + 1;
    
    for(uint8_t page = start_page; page <= end_page; page++) {
        uint8_t current_checksum = OLED_CalculateChecksum(page);
        
        // 只有校验码改变时才刷新该页
        if(current_checksum != page_checksum[page]) {
            // 设置页地址
            SSD1315_WriteCmd(0xB0 + page);
            // 设置列地址低位
            SSD1315_WriteCmd(0x00 + (start_col & 0x0F));
            // 设置列地址高位
            SSD1315_WriteCmd(0x10 + ((start_col >> 4) & 0x0F));
            
            // 发送该页指定列范围的数据
            SSD1315_WriteData(&display_ram[page][start_col], width);
            
            page_checksum[page] = current_checksum; // 更新校验码
        }
    }
}

/**
  * @brief 标记指定页为需要刷新（手动设置校验码无效）
  * @param page: 页号(0-7)
  */
void OLED_MarkPageDirty(uint8_t page) {
    if(page < SSD1315_PAGES) {
        page_checksum[page] = 0xFF; // 设置为无效值，下次必定刷新
    }
}

/**
  * @brief 标记所有页为需要刷新
  * @param
  */
void OLED_MarkAllDirty(void) {
    for(uint8_t i = 0; i < SSD1315_PAGES; i++) {
        page_checksum[i] = 0xFF;
    }
}

///**
//  * @brief 初始化校验码数组
//  * @param
//  */
//void OLED_Checksum_Init(void) {
//    for(uint8_t i = 0; i < SSD1315_PAGES; i++) {
//        page_checksum[i] = OLED_CalculateChecksum(i);
//    }
//}
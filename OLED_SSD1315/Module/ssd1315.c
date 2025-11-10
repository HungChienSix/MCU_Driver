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

/**
	* @brief 刷新屏幕
	* @param
	*/
void OLED_RefreshScreen(void){
	for(unsigned short int m = 0; m < SSD1315_PAGES; m++){
		SSD1315_WriteCmd(0xB0+m);
		SSD1315_WriteCmd(0x00);
		SSD1315_WriteCmd(0x10);
//		for(unsigned short int n = 0; n < SSD1315_WIDTH; n++){
//			//SSD1315_WriteData((uint8_t*)display_ram, 1024);
//			SSD1315_WriteData(&display_ram[m][n], 1);
//		}
		SSD1315_WriteData(&display_ram[m][0], 128);
	}
}


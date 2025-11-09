#include "oled.h"
#include "string.h"

uint8_t g_ucaOledRam[8][128];/*显存*/
	
void SSD1315_WriteCmd(uint8_t cmd){
	HAL_I2C_Mem_Write(&SSD1315_I2C, SSD1315_Dev_Addr, SSD1315_MemCmd_Addr, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
}
	
void SSD1315_WriteData(uint8_t data){
	HAL_I2C_Mem_Write(&SSD1315_I2C, SSD1315_Dev_Addr, SSD1315_MemData_Addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}

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
void SSD1315_Init(void){
	
	SSD1315_WriteCmd(0xAE);//0xAE Set Display ON ,0xAF Set Display OFF
	
	//SSD1315_WriteCmd(0x00);//set low  column address
	//SSD1315_WriteCmd(0x10);//set high column address
	
	SSD1315_WriteCmd(0x20); SSD1315_WriteCmd(0x00); // Horizontal Addressing Mode
	SSD1315_WriteCmd(0x21); SSD1315_WriteCmd(SSD1315_ColSTART ); SSD1315_WriteCmd(SSD1315_ColEND ); // Col start and end
	SSD1315_WriteCmd(0x22); SSD1315_WriteCmd(SSD1315_PageSTART); SSD1315_WriteCmd(SSD1315_PageEND); // Page start and end
	
	SSD1315_WriteCmd(0xA8); SSD1315_WriteCmd(0x3F); // Set Multiplex Ratio 0x3F+0x01 == 64d
	SSD1315_WriteCmd(0xD3); SSD1315_WriteCmd(0x00); // Set Display Offset
	//SSD1315_WriteCmd(0x40); // Set display RAM display start line 
	SSD1315_SetRotation(SSD1315_ROTATION);
	//SSD1315_WriteCmd(0xDA); SSD1315_WriteCmd(0x12); // Set com pins hardware configuration
	
	SSD1315_WriteCmd(0xD5); SSD1315_WriteCmd(0x80); // Set Display Clock Divide Ratio/Oscillator Frequency
	SSD1315_WriteCmd(0xD9); SSD1315_WriteCmd(0xF1); // Set Pre-charge Period
	SSD1315_WriteCmd(0xDB); SSD1315_WriteCmd(0x30); // Set COM select voltage level
	
	SSD1315_WriteCmd(0x81); SSD1315_WriteCmd(SSD315_Brightness); // Set Contrast Control
	SSD1315_WriteCmd(0x8D); SSD1315_WriteCmd(0x14); // Enable internal charge pump
	
	//SSD1315_WriteCmd(0xA4); // if A5 Entire display ON
	SSD1315_WriteCmd(ST7735_INVERSE); // if A7 inverse display
	
	SSD1315_WriteCmd(0xAF);//if AE open the sleep mode
	
	OLED_Clear();
	OLED_Refresh();
	
}

void OLED_Clear(void){
    memset(g_ucaOledRam, 0x00, sizeof(g_ucaOledRam));
}

void OLED_DrawPoint(uint8_t x,uint8_t y){
    uint8_t ucTmp;
    uint8_t ucY,ucZ;
    if(x>127 || y>63)
    {
        return;
    }
    else
    {
        ucY = y / 8;
        ucZ = y % 8;
        ucTmp = g_ucaOledRam[ucY][x];
        ucTmp |= (0x01 << ucZ);
        g_ucaOledRam[ucY][x] = ucTmp;
    }
}

void OLED_Refresh(void){
    HAL_I2C_Mem_Write( &SSD1315_I2C,SSD1315_Dev_Addr,0x40,I2C_MEMADD_SIZE_8BIT,  &g_ucaOledRam[0][0], 1024,500);  //
}


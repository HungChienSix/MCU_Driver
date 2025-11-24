#include "st7735.h"
#include <stdbool.h>
#include "usart.h"

// 帧缓冲区：128x128 像素, 16位色 (RGB565), 2 字节/像素
uint16_t display_ram[ST7735_HEIGHT][ST7735_WIDTH] = {0x0000};

#ifdef ST7735_PARTIAL_REFRESH
    static uint16_t page_checksum[ST7735_HEIGHT] = {0x00}; // 页校验码数组
#endif

// 脏矩阵,高位在前,如果位是1代表需要刷新
uint8_t frame_bit[ST7735_HEIGHT][ST7735_WIDTH / 8] = {0x00};

void ST7735_Reset(void){
	HAL_GPIO_WritePin(ST7735_RST_GPIO_Port, ST7735_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(ST7735_RST_GPIO_Port, ST7735_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
}

void ST7735_WriteCommand(uint8_t cmd){
	HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&ST7735_SPI, &cmd, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

void ST7735_WriteByte(uint8_t data){
	HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&ST7735_SPI, &data, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

void ST7735_WriteData(uint8_t *data, size_t data_size){
	  HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
	  HAL_SPI_Transmit(&ST7735_SPI, data, data_size, HAL_MAX_DELAY);
	  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

/**
 * @brief 设置屏幕旋转
 * @note  
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
  * @note  
  */
void LCD_Init(void) {
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
	LCD_FillScreen(ST7735_BLACK);
	LCD_RefreshScreen_Force();
	HAL_Delay(100);
}

/**
  * @brief 创建绘图区域
  * @note  坐标 (x, y) = (水平, 垂直)
  */
void ST7735_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){
	uint8_t data[4] = {0x00};
	
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

/**
 * @brief 在缓冲区读取像素点状态
 * @param x: X坐标(0-127)
 * @param y: Y坐标(0-127)
 * @return 像素状态 LCD_Pixel_t 像素RGB565值
 */
LCD_Pixel_t LCD_ReadPixel(uint16_t x, uint16_t y){
	return display_ram[y][x];
}

/**
 * @brief 绘制像素点
 * @param x, y: 坐标
 * @param Pixel_Set: 像素状态
 */
void LCD_DrawPixel(int16_t x, int16_t y, LCD_Pixel_t Pixel_Set){
	if((x < 0)||(y < 0)||(x > ST7735_WIDTH-1)||(y > ST7735_HEIGHT-1))
		return ;
	display_ram[y][x] = Pixel_Set;
}

/**
 * @brief 绘制水平线 
 */
void LCD_DrawHorLine(int16_t x0, int16_t x1, int16_t y, LCD_Pixel_t Pixel_Set, LCD_Type_t type){
    uint16_t x_start = (x0 < x1)? x0 : x1;
    uint16_t x_end   = (x0 < x1)? x1 : x0;
	
	for(int16_t x = x_start; x <= x_end; x++) {
		if((type == LCD_Xor)&&(LCD_ReadPixel(x,y)))
			LCD_DrawPixel(x, y, ~Pixel_Set);
		else
			LCD_DrawPixel(x, y, Pixel_Set);
	}
}

/**
 * @brief 绘制垂直线 
 */
void LCD_DrawVerLine(int16_t x, int16_t y0, int16_t y1, LCD_Pixel_t Pixel_Set, LCD_Type_t type){
	uint16_t y_start = (y0 < y1)? y0 : y1;
    uint16_t y_end   = (y0 < y1)? y1 : y0;
	
	for(int16_t y = y_start; y <= y_end; y++) {
		if((type == LCD_Xor)&&(LCD_ReadPixel(x,y)))
			LCD_DrawPixel(x, y, ~Pixel_Set);
		else
			LCD_DrawPixel(x, y, Pixel_Set);
	}
}

/**
 * @brief 填充矩形区域 
 */
void LCD_FillArea(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1, LCD_Pixel_t Pixel_Set, LCD_Type_t type){
    uint16_t x_start = (x0 < x1)? x0 : x1;
    uint16_t x_end   = (x0 < x1)? x1 : x0;
	
	uint16_t y_start = (y0 < y1)? y0 : y1;
    uint16_t y_end   = (y0 < y1)? y1 : y0;
    
	for(int16_t y = y_start; y <= y_end; y++) {
		for(int16_t x = x_start; x <= x_end; x++) {
			if((type == LCD_Xor)&&(LCD_ReadPixel(x,y)))
				LCD_DrawPixel(x, y, ~Pixel_Set);
			else
				LCD_DrawPixel(x, y, Pixel_Set);
		}
	}
}

/**
 * @brief 填充整个屏幕
 */
void LCD_FillScreen(LCD_Pixel_t Pixel_Set) {
  for (uint32_t h = 0; h < ST7735_HEIGHT; h++) {
	for (uint32_t w = 0; w < ST7735_WIDTH; w++) {
	  display_ram[h][w] = Pixel_Set;
	}
  }
}

#ifdef ST7735_PARTIAL_REFRESH
/**
 * @brief 计算页数据的校验码
 * @note 使用改进的校验算法减少冲突概率
 */
uint16_t LCD_CalculateChecksum(uint16_t height) {
    uint32_t checksum = 0;  // 使用32位中间计算防止溢出
    
    for(int i = 0; i < ST7735_WIDTH; i++) {
        // 改进的校验算法：结合旋转和异或
        checksum = ((checksum << 7) | (checksum >> 9)) ^ display_ram[height][i];
        checksum += display_ram[height][i] * 31;  // 质数乘法减少冲突
    }
    
    return (uint16_t)(checksum & 0xFFFF);  // 返回16位结果
}
#endif

/**
 * @brief 刷新校验码变化的屏幕
 */
void LCD_RefreshScreen(void) {
#ifdef ST7735_PARTIAL_REFRESH
    static uint8_t buff[ST7735_WIDTH * 2];
    
    for (uint16_t h = 0; h < ST7735_HEIGHT; h++) {  // 改为uint16_t防止溢出
        uint16_t current_checksum = LCD_CalculateChecksum(h);  // 改为uint16_t
        
        if (current_checksum != page_checksum[h]) {

			ST7735_SetAddressWindow(0, h, ST7735_WIDTH - 1, h);
			ST7735_WriteCommand(ST7735_RAMWR);

            
            // 准备行数据
            for (uint16_t w = 0; w < ST7735_WIDTH; w++) {
                buff[w * 2] = display_ram[h][w] >> 8;
                buff[w * 2 + 1] = display_ram[h][w] & 0xFF;
            }
            
            // 发送数据
            ST7735_WriteData(buff, ST7735_WIDTH * 2);
            
            // 更新校验和
            page_checksum[h] = current_checksum;
            
		}

    }
#else
    LCD_RefreshScreen_Force();
#endif
}

/**
 * @brief 强制刷新整个屏幕
 */
void LCD_RefreshScreen_Force(void) {
    ST7735_SetAddressWindow(0, 0, ST7735_WIDTH - 1, ST7735_HEIGHT - 1);
    ST7735_WriteCommand(ST7735_RAMWR);
    
    static uint8_t buff[ST7735_WIDTH * 2];  // 单行缓冲区
    
    for (uint16_t h = 0; h < ST7735_HEIGHT; h++) {
        // 准备当前行数据
        for (uint16_t w = 0; w < ST7735_WIDTH; w++) {
            buff[w * 2] = display_ram[h][w] >> 8;
            buff[w * 2 + 1] = display_ram[h][w] & 0xFF;
        }
        
        // 发送当前行数据
        ST7735_WriteData(buff, ST7735_WIDTH * 2);
    }
}
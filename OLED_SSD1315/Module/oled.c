//#include "oled.h"
//#include "string.h"

//void OLED_Clear(void){
//    memset(g_ucaOledRam, 0x00, sizeof(g_ucaOledRam));
//}

//void OLED_DrawPoint(uint8_t x,uint8_t y){
//    uint8_t ucTmp;
//    uint8_t ucY,ucZ;
//    if(x>127 || y>63)
//    {
//        return;
//    }
//    else
//    {
//        ucY = y / 8;
//        ucZ = y % 8;
//        ucTmp = g_ucaOledRam[ucY][x];
//        ucTmp |= (0x01 << ucZ);
//        g_ucaOledRam[ucY][x] = ucTmp;
//    }
//}

//void OLED_Refresh(void){
//    HAL_I2C_Mem_Write( &SSD1315_I2C,SSD1315_Dev_Addr,0x40,I2C_MEMADD_SIZE_8BIT,  &g_ucaOledRam[0][0], 1024,500);  //
//}


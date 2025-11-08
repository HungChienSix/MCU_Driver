#ifndef __OLED_H
#define __OLED_H 

#include "main.h"

#define SSD1315 
//#define SSD1306

#define OLED_COM_PORT	hi2c1
#define OLED_ADDR		0X78

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


void OLED_Init(void);
void OLED_ColorTurn(uint8_t i);
void OLED_DisplayTurn(uint8_t i);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);

void OLED_SetStart(void);

void OLED_ClearPoint(uint8_t x,uint8_t y);
void OLED_DrawPoint(uint8_t x,uint8_t y);
void OLED_Fill(uint8_t ucData);
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1);
void OLED_ShowString(uint8_t x,uint8_t y,char *chr,uint8_t size1);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1);
void OLED_ShowPicture(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t *ucP);
void OLED_ShowBigNum(uint8_t x,uint8_t y,uint8_t num);//32*16大数字
void OLED_DrawRec(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye);/*绘制矩形*/
void OLED_DrawProgress(uint8_t _ucX0,uint8_t _ucY0,uint8_t _ucX1,uint8_t _ucY1,uint8_t _ucPercet);/*进度条绘制*/
void OLED_ShowChinesStr(uint8_t _ucX0,uint8_t _ucY0,char *pstr);/*显示中文字符串*/

void OLED_Refresh(void);
#endif
//


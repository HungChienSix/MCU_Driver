#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include "i2c.h"

uint8_t g_ucaOledRam[8][128];/*显存*/

void OLED_WriteByte(uint8_t dat,uint8_t cmd);

/*
**********************************************************
* 说明：设置正反显
* 输入：
* 输出：
**********************************************************
*/
void OLED_ColorTurn(uint8_t i)
{
    if(i==0)
    {
        OLED_WriteByte(0xA6,OLED_CMD);//正常显示
    }
    if(i==1)
    {
        OLED_WriteByte(0xA7,OLED_CMD);//反色显示
    }
}

/*
**********************************************************
* 说明：旋转屏幕180
* 输入：
* 输出：
**********************************************************
*/
void OLED_DisplayTurn(uint8_t i)
{
    if(i==0)
    {
        OLED_WriteByte(0xC8,OLED_CMD);//正常显示
        OLED_WriteByte(0xA1,OLED_CMD);
    }
    if(i==1)
    {
        OLED_WriteByte(0xC0,OLED_CMD);//反转显示
        OLED_WriteByte(0xA0,OLED_CMD);
    }
}

/*
**********************************************************
* 说明：OLED写入一个字节
* 输入：
* 输出：
**********************************************************
*/
void OLED_WriteByte(uint8_t dat,uint8_t cmd)
{
    if(cmd == OLED_CMD)
    {
        HAL_I2C_Mem_Write( &OLED_COM_PORT,OLED_ADDR,0x00,I2C_MEMADD_SIZE_8BIT,  &dat, 1,100);  //
    }
    else
    {
        HAL_I2C_Mem_Write( &OLED_COM_PORT,OLED_ADDR,0x40,I2C_MEMADD_SIZE_8BIT,  &dat, 1,100);  //
    }
}


/*
**********************************************************
* 说明：OLED开启显示
* 输入：
* 输出：
**********************************************************
*/
void OLED_DisPlay_On(void)
{
    OLED_WriteByte(0x8D,OLED_CMD);//电荷泵使能
    OLED_WriteByte(0x14,OLED_CMD);//开启电荷泵
    OLED_WriteByte(0xAF,OLED_CMD);//点亮屏幕
}

/*
**********************************************************
* 说明：OLED关闭显示
* 输入：
* 输出：
**********************************************************
*/
void OLED_DisPlay_Off(void)
{
    OLED_WriteByte(0x8D,OLED_CMD);//电荷泵使能
    OLED_WriteByte(0x10,OLED_CMD);//关闭电荷泵
    OLED_WriteByte(0xAE,OLED_CMD);//关闭屏幕
}

/*
**********************************************************
* 说明：刷新一帧数据 将显存数据写入OLED
* 输入：
* 输出：
**********************************************************
*/
void OLED_Refresh(void)
{
    HAL_I2C_Mem_Write( &OLED_COM_PORT,OLED_ADDR,0x40,I2C_MEMADD_SIZE_8BIT,  &g_ucaOledRam[0][0], 1024,200);  //
}

/*
**********************************************************
* 说明：填充屏幕
* 输入：
* 输出：
**********************************************************
*/
void OLED_Fill(uint8_t ucData)
{
    uint8_t *puc;
    uint16_t i;
    puc = &g_ucaOledRam[0][0];

    for(i=0; i<1024; i++)
    {
        *puc++ = ucData;
    }
}

/*
**********************************************************
* 说明：OLED初始化 该部分是数据手册的内容
* 输入：
* 输出：
**********************************************************
*/
void OLED_Init(void)
{
	HAL_Delay(100);
	
#ifdef  SSD1315
	OLED_WriteByte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WriteByte(0x00,OLED_CMD);//---set low column address
	OLED_WriteByte(0x10,OLED_CMD);//---set high column address
	OLED_WriteByte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WriteByte(0x81,OLED_CMD);//--set contrast control register
	OLED_WriteByte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
	OLED_WriteByte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WriteByte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WriteByte(0xA6,OLED_CMD);//--set normal display
	OLED_WriteByte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WriteByte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WriteByte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WriteByte(0x00,OLED_CMD);//-not offset
	OLED_WriteByte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WriteByte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WriteByte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WriteByte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WriteByte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WriteByte(0x12,OLED_CMD);
	OLED_WriteByte(0xDB,OLED_CMD);//--set vcomh
	OLED_WriteByte(0x30,OLED_CMD);//Set VCOM Deselect Level 
	OLED_WriteByte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02) 0X00 is Horizontal Addressin
	OLED_WriteByte(0x00,OLED_CMD);// 
	OLED_WriteByte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WriteByte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WriteByte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WriteByte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)
	OLED_WriteByte(0xAF,OLED_CMD);
#endif
}

/*
**********************************************************
* 说明：设置CGRAM起点为0，0
* 输入：
* 输出：
**********************************************************
*/
void OLED_SetStart(void)
{
    //寻址方式
    OLED_WriteByte(0X20,OLED_CMD);		// 设置GDDRAM模式
    OLED_WriteByte(0X00,OLED_CMD);		// 水平寻址模式

    OLED_WriteByte(0X21,OLED_CMD);	// 设置列起始和结束地址
    OLED_WriteByte(0X00,OLED_CMD);	// 列起始地址 0
    OLED_WriteByte(0X7F,OLED_CMD);	// 列终止地址 127

    OLED_WriteByte(0X22,OLED_CMD);	// 设置页起始和结束地址
    OLED_WriteByte(0X00,OLED_CMD);	// 页起始地址 0
    OLED_WriteByte(0X07,OLED_CMD);	// 页终止地址 7
}
/*
**********************************************************
* 说明：清除一个区域
* 输入：
* 输出：
**********************************************************
*/
void OLED_ClearArea(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye)
{
    uint8_t i,n, w,h;
    h = ye - ys;
    w = xe - xs;
    for(i = 0; i<h; i++)
    {
        for(n=0; n<w; n++)
        {
            OLED_ClearPoint(xs+n,ys+i);
        }
    }
}
/*
**********************************************************
* 说明：填充一个区域
* 输入：
* 输出：
**********************************************************
*/
void OLED_FillArea(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye)
{
    uint8_t i,n, w,h;
    h = ye - ys;
    w = xe - xs;
    for(i = 0; i<h; i++)
    {
        for(n=0; n<w; n++)
        {
            OLED_DrawPoint(xs+n,ys+i);
        }
    }
}
/*
**********************************************************
* 说明：写点
* 输入：
* 输出：
**********************************************************
*/
void OLED_DrawPoint(uint8_t x,uint8_t y)
{
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
/*
**********************************************************
* 说明：清除点
* 输入：
* 输出：
**********************************************************
*/
void OLED_ClearPoint(uint8_t x,uint8_t y)
{
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
        ucTmp &= ~(0x01 << ucZ);
        g_ucaOledRam[ucY][x] = ucTmp;
    }
}

//画线
//x:0~128
//y:0~64
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
{
    uint8_t i,k,k1,k2;
//	if((x1<0)||(x2>128)||(y1<0)||(y2>64)||(x1>x2)||(y1>y2))return;
    if(x1==x2)    //画竖线
    {
        for(i=0; i<(y2-y1); i++)
        {
            OLED_DrawPoint(x1,y1+i);
        }
    }
    else if(y1==y2)   //画横线
    {
        for(i=0; i<(x2-x1); i++)
        {
            OLED_DrawPoint(x1+i,y1);
        }
    }
    else      //画斜线
    {
        k1=y2-y1;
        k2=x2-x1;
        k=k1*10/k2;
        for(i=0; i<(x2-x1); i++)
        {
            OLED_DrawPoint(x1+i,y1+i*k/10);
        }
    }
}
//x,y:圆心坐标
//r:圆的半径
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r)
{
    int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)
    {
        OLED_DrawPoint(x + a, y - b);
        OLED_DrawPoint(x - a, y - b);
        OLED_DrawPoint(x - a, y + b);
        OLED_DrawPoint(x + a, y + b);

        OLED_DrawPoint(x + b, y + a);
        OLED_DrawPoint(x + b, y - a);
        OLED_DrawPoint(x - b, y - a);
        OLED_DrawPoint(x - b, y + a);

        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}



//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size:选择字体 12/16/24
//取模方式 逐列式
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1)
{
    uint8_t i,m,temp,size2,chr1;
    uint8_t y0=y;
    size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
    chr1=chr-' ';  //计算偏移后的值
    for(i=0; i<size2; i++)
    {
        temp=asc2_1608[chr1][i];//调用1608字体
//		if(size1==12)
//        {temp=asc2_1206[chr1][i];} //调用1206字体
//		else if(size1==16)
//        {temp=asc2_1608[chr1][i];} //调用1608字体
//		else if(size1==24)
//        {temp=asc2_2412[chr1][i];} //调用2412字体
//		else return;
        for(m=0; m<8; m++)         //写入数据
        {
            if(temp&0x80)OLED_DrawPoint(x,y);
            else OLED_ClearPoint(x,y);
            temp<<=1;
            y++;
            if((y-y0)==size1)
            {
                y=y0;
                x++;
                break;
            }
        }
    }
}


//显示字符串
//x,y:起点坐标
//size1:字体大小
//*chr:字符串起始地址
void OLED_ShowString(uint8_t x,uint8_t y,char *chr,uint8_t size1)
{
    while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
    {
        OLED_ShowChar(x,y,*chr,size1);
        x+=size1/2;
        if(x>128-size1)  //换行
        {
            x=0;
            y+=2;
        }
        chr++;
    }
}

//m^n
static uint32_t OLED_Pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
    while(n--)
    {
        result*=m;
    }
    return result;
}

////显示2个数字
////x,y :起点坐标
////len :数字的位数
////size:字体大小
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1)
{
    uint8_t t,temp;
    for(t=0; t<len; t++)
    {
        temp=(num/OLED_Pow(10,len-t-1))%10;
        if(temp==0)
        {
            OLED_ShowChar(x+(size1/2)*t,y,'0',size1);
        }
        else
        {
            OLED_ShowChar(x+(size1/2)*t,y,temp+'0',size1);
        }
    }
}

//
void OLED_ShowBigNum(uint8_t x,uint8_t y,uint8_t num)//32*16大数字
{
    uint8_t i,j,k,tmp;
    uint8_t x0=x;
    for(i=0; i<32; i++)
    {
        for(j=0; j<2; j++)
        {
            tmp=num_3216[num][i*2+j];
            for(k=0; k<8; k++)
            {
                if(tmp&0x80)OLED_DrawPoint(x,y);
                else OLED_ClearPoint(x,y);
                tmp<<=1;
                x++;
            }
        }
        y++;
        x=x0;
    }
}


//x0,y0：起点坐标
//x1,y1：终点坐标
//BMP[]：要写入的图片数组
void OLED_ShowPicture(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t *ucP)
{
//    uint32_t j=0;
//    uint8_t x=0,y=0;
//    if(y%8==0)y=0;
//    else y+=1;
//    for(y=y0; y<y1; y++)
//    {
//        OLED_WR_BP(x0,y);
//        for(x=x0; x<x1; x++)
//        {
//            OLED_WriteByte(ucP[j],OLED_DATA);
//            j++;
//        }
//    }
}


/*
* 功能：绘制进度条
* 说明：_ucX0，_ucY0=原点坐标；_ucWidth
*/
void OLED_DrawProgress(uint8_t _ucX0,uint8_t _ucY0,uint8_t _ucX1,uint8_t _ucY1,uint8_t _ucPercet)
{
    uint8_t xe,ye;
    OLED_ClearArea(_ucX0,_ucY0,_ucX1,_ucY1);
    xe = (_ucX1-_ucX0) * _ucPercet / 100 + _ucX0;
    ye = _ucY1;
    OLED_FillArea(_ucX0,_ucY0,xe,ye);
}
/*
* 功能：绘制矩形框
* 说明：_ucX0，_ucY0=原点坐标；_ucWidth
*/
void OLED_DrawRec(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye)
{
    OLED_DrawLine(xs,ys,xe,ys);
    OLED_DrawLine(xs,ye,xe,ye);
    OLED_DrawLine(xs,ys,xs,ye);
    OLED_DrawLine(xe,ys,xe,ye);
}
/*
* 功能：显示中文字符串
* 说明：_ucX0，_ucY0=原点坐标；
*/
void OLED_ShowChinesStr(uint8_t _ucX0,uint8_t _ucY0,char *pstr)
{

    __IO uint8_t i,k,j,cnt = 0,index = 0,num,tmp;
    uint8_t x =_ucX0,y = _ucY0;
    __IO char buff[2];

    num = sizeof(tHZ_FONT16) / sizeof(chType_T);

    while(*pstr != '\0')
    {
        buff[0] = *pstr++;
        buff[1] = *pstr++;
        /*索引汉字所在位置*/
        while(index<num)
        {

            if(buff[0] == tHZ_FONT16[index].str[0] && buff[1] == tHZ_FONT16[index].str[1])
            {
                break;
            }
            index++;
        }
        if(index == num)
        {
            //字库中无此汉字
        }
        else
        {

            for(i=0; i<16; i++)
            {
                for(j=0; j<2; j++)
                {
                    tmp=tHZ_FONT16[index].dot[cnt++];
                    for(k=0; k<8; k++)
                    {
                        if(tmp&0x80)OLED_DrawPoint(x,y);
                        else OLED_ClearPoint(x,y);
                        tmp<<=1;
                        x++;
                    }
                }
                y++;
                x=_ucX0;
            }
            index = 0;
            _ucX0 += 16;
            y = _ucY0;
            x = _ucX0;
            cnt = 0;
        }
    }
}

/*
* 功能：显示中文字符串
* 说明：_ucX0，_ucY0=原点坐标；
*/
//void OLED_ShowChinesStr(uint8_t _ucX0,uint8_t _ucY0,char *pstr)
//{
//
//    __IO uint8_t i,k,j,index = 0,tmp;
//	uint8_t x =_ucX0,y = _ucY0;
//	__IO uint8_t ucGB2312L,ucGB2312H;
//	__IO uint32_t ulHZaddr;
//	uint8_t ucaHZDot[32];/*16*16的汉字点阵信息*/
//
//
//	while(*pstr != '\0')
//	{
//		ucGB2312H = *pstr++;
//		ucGB2312L = *pstr++;
//		/*索引汉字所在位置*/
//		ulHZaddr = 32*((ucGB2312H-0xa0-1)*94+(ucGB2312L-0xa0-1));
//		SF_ReadBuffer(ucaHZDot,ulHZaddr,32);

//			for(i=0; i<16; i++)
//			{
//				for(j=0; j<2; j++)
//				{
//					tmp=ucaHZDot[index];
//					for(k=0; k<8; k++)
//					{
//						if(tmp&0x80)OLED_DrawPoint(x,y);
//						else OLED_ClearPoint(x,y);
//						tmp<<=1;
//						x++;
//					}
//					index++;
//				}
//				y++;
//				x=_ucX0;
//			}
//			index = 0;
//			_ucX0 += 16;
//			y = _ucY0;
//			x = _ucX0;
//
//	}
//
//}


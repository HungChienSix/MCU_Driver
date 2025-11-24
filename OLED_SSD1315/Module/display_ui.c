//#include "display_ui.h"
//#include <stdio.h> // 假设需要 NULL
//#include <string.h> // 假设需要 strlen 或类似的字符串处理函数

//// 假设您有一个字体定义
//extern const struFont Font_8x16_consolas;

//UI_Button_t button0 = {
//    .type = UI_BUTTON,
//    .x0 = 64,       // 中心 X 坐标
//    .y0 = 32,       // 中心 Y 坐标
//    .width = 80,    // 宽度
//    .height = 24,   // 高度
//    .text = "Click Me", // 按钮文本
//    .font = &Font_8x16_consolas, // 字体指针
//    .font_t = FONT_Bold,    // 字体颜色/类型参数 (根据您的实际定义)
//};

///**
// * @brief 绘制按钮
// * @param comp: 组件指针
// */
//void UI_DrawButton(UI_Button_t *comp) {
//	if (comp == NULL || comp->font == NULL) {
//        return; // 空指针检查
//    }

//    // --- 1. 计算按钮边界和圆角半径 ---
//    
//    // 按钮左上角 (x_start, y_start) 和右下角 (x_end, y_end)
//    int16_t x_start = comp->x0 - comp->width / 2;
//    int16_t y_start = comp->y0 - comp->height / 2;
//    int16_t x_end   = x_start + comp->width - 1;
//    int16_t y_end   = y_start + comp->height - 1;

//    // 假设圆角半径为按钮高度的 1/4，但不超过按钮宽度的一半
//    uint16_t radius = comp->height / 4;
//    if (radius > comp->width / 2) {
//        radius = comp->width / 2;
//    }
//    // 最小圆角半径为 1
//    if (radius < 1) {
//        radius = 1; 
//    }

//    // 定义像素设置方式（例如：白色/亮起）
//    OLED_Pixel_t pixel_set = OLED_ON;
//    OLED_Type_t draw_type = OLED_Nor;
//    
//    // --- 2. 绘制圆角矩形边框 (使用直线和四分之一圆弧) ---
//    
//    // 2.1. 绘制水平线 (连接圆弧)
//    // 上边: (x_start + radius, y_start) 到 (x_end - radius, y_start)
//    OLED_DrawLine(x_start + radius, x_end - radius, y_start, y_start, pixel_set, draw_type);
//    // 下边: (x_start + radius, y_end) 到 (x_end - radius, y_end)
//    OLED_DrawLine(x_start + radius, x_end - radius, y_end, y_end, pixel_set, draw_type);

//    // 2.2. 绘制垂直线 (连接圆弧)
//    // 左边: (x_start, y_start + radius) 到 (x_start, y_end - radius)
//    OLED_DrawLine(x_start, x_start, y_start + radius, y_end - radius, pixel_set, draw_type);
//    // 右边: (x_end, y_start + radius) 到 (x_end, y_end - radius)
//    OLED_DrawLine(x_end, x_end, y_start + radius, y_end - radius, pixel_set, draw_type);

//    // 2.3. 绘制四角圆弧
//    // 左上角: 圆心 (x_start + radius, y_start + radius), 象限 3
//    OLED_DrawQuarterArc(x_start + radius -1 , y_start + radius -1 , radius, Quarter3, pixel_set, draw_type);
//    // 右上角: 圆心 (x_end - radius, y_start + radius), 象限 4
//    OLED_DrawQuarterArc(x_end - radius +1 , y_start + radius -1 , radius, Quarter4, pixel_set, draw_type);
//    // 左下角: 圆心 (x_start + radius, y_end - radius), 象限 2
//    OLED_DrawQuarterArc(x_start + radius -1 , y_end - radius +1, radius, Quarter2, pixel_set, draw_type);
//    // 右下角: 圆心 (x_end - radius, y_end - radius), 象限 1
//    OLED_DrawQuarterArc(x_end - radius +1 , y_end - radius +1, radius, Quarter1, pixel_set, draw_type);
//    
//    // 

//    // --- 3. 绘制居中显示的文本 ---
//    if (comp->text != NULL) {
//        // 假设 font->width 和 font->height 分别是字符的宽度和高度
//        uint16_t text_len = 0;
//        // 这是一个假设，您可能需要一个实际的函数来计算字符串宽度
//        // 或者依赖 OLED_DrawString 内部的计算
//        // 简单起见，假设每个字符宽度固定为 comp->font->width
//        // 您可能需要根据实际字体计算文本总宽度
//        // 
//        // ⚠️ 注意: 某些字体可能不是等宽的，如果您的字体是非等宽的，需要更精确的文本宽度计算
//        if (comp->font->width > 0) {
//            // 假设这是一个等宽字体
//            text_len = strlen(comp->text);
//        } else {
//             // 如果字体宽度未知或非等宽，可能需要调用一个计算字符串像素宽度的函数
//             // 或者在 UI_DrawString 中处理居中
//             // 这里我们**假设**可以通过 comp->font->width * strlen(comp->text) 来估算
//             // 并且 OLED_DrawString 可以接受居中坐标，但接口不是这样定义的，
//             // 所以我们必须自己计算起始绘制坐标。
//             // 假设字体是等宽的
//             text_len = strlen(comp->text);
//        }

//        // 计算文本总宽度 (像素) 和高度 (像素)
//        uint16_t text_width_px = text_len * comp->font->width;
//        uint16_t text_height_px = comp->font->height;

//        // 计算文本起始绘制坐标 (使其居中)
//        int16_t text_x = comp->x0 - (int16_t)(text_width_px / 2);
//        int16_t text_y = comp->y0 - (int16_t)(text_height_px / 2);
//        
//        // 绘制文本
//        OLED_DrawString(text_x, text_y, comp->text, comp->font, comp->font_t, pixel_set, draw_type);
//    }
//}
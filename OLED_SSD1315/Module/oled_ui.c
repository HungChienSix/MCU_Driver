#include "oled_ui.h"

///**
// * @brief 绘制按钮
// * @param comp: UI组件指针
// * @param font: 字体指针
// */
//void UI_DrawButton(UI_Component *comp, const struFont *font) {
//	
//	
//    // 绘制按钮背景
//    if (comp->state) {
//        // 按下状态 - 实心矩形
//        OLED_DrawRectangle(comp->x, comp->y, 
//                          comp->x + comp->width, 
//                          comp->y + comp->height, 
//                          UI_COLOR_ACCENT);
//        // 绘制文字（反色）
//        if (comp->text) {
//            OLED_DrawString(comp->x + 2, comp->y + (comp->height - font->height) / 2, 
//                           comp->text, UI_COLOR_BG, font, 0);
//        }
//    } else {
//        // 正常状态 - 空心矩形
//        OLED_DrawRectangle(comp->x, comp->y, 
//                          comp->x + comp->width, 
//                          comp->y + comp->height, 
//                          UI_COLOR_FG);
//        // 绘制文字
//        if (comp->text) {
//            OLED_DrawString(comp->x + 2, comp->y + (comp->height - font->height) / 2, 
//                           comp->text, UI_COLOR_FG, font, 0);
//        }
//    }
//}
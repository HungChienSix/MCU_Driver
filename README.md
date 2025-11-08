## 项目概述

本仓库包含基于 STM32F4 系列 MCU 的驱动示例与模块，方便快速把外设（ST7735 TFT ，SSD1315 OLED等）集成到 Keil/MDK 工程中。该项目以模块化方式组织，包含示例工程、外设驱动和字体/界面代码。

该项目没有采用开源的可移植UI框架，使用纯C代码。

主要特点：
- 使用 STM32 HAL 驱动，兼容 STM32F4 系列 MCU 
- Keil MDK-ARM 工程与源代码同时提供，方便直接编译与调试

---

## 依赖与开发环境

- Keil MDK-ARM（便于直接打开 `.uvprojx` 并调试）
- STM32CubeMX（可直接查看引脚和外设的配置）

---

## 致谢

项目中的 `ST7735` 的 `ST7735_Init()` 参考了 `CSDN` 博主 `大宝SOD蜜涂在脸上很舒服你知道吗` ，文章链接 [ST7735驱动](https://blog.csdn.net/Xiaowu12345678910/article/details/129038133?fromshare=blogdetail&sharetype=blogdetail&sharerId=129038133&sharerefer=PC&sharesource=Moil233&sharefrom=from_link "STM32驱动ST7735彩色屏幕（任意分辨率），驱动不了你顺着网线来打我") ，仅更改了部分错误 ，绘图函数由作者另外实现。

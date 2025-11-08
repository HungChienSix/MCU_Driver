## 项目概述

本仓库包含基于 STM32F4 系列 MCU 的驱动示例与模块，方便快速把外设（ST7735 TFT 等）集成到 Keil/MDK 工程中。该项目以模块化方式组织，包含示例工程、外设驱动和字体/界面代码。项目使用的是

主要特点：
- 使用 STM32 HAL 驱动，兼容 STM32F4 系列 MCU 
- Keil MDK-ARM 工程与源代码同时提供，方便直接编译与调试

## 目录结构（概览）

仓库根目录示例结构：

- `OLED_SSD1315/` - OLED 示例工程与驱动
	- `Core/` - HAL 初始化、主程序、外设配置
	- `Module/` - `oled.c`/`oled.h`、字体 (`oledfont.h`) 等
	- `Drivers/` - HAL 与 CMSIS 驱动
	- `MDK-ARM/` - Keil 工程文件与编译产物（可选）

- `ST7735_TFT/` - ST7735 TFT 示例工程与驱动
	- 与 OLED 目录结构类似，包含 `st7735.c`、`fonts.c`、示例 UI 文件

- `Module/` -（如存在）共享模块或工具代码
- `README.md` - 本文件
- `Document/` - 设计说明或外设资料（可选）

（注：具体目录请参见仓库实际内容）

## 兼容硬件与引脚

本项目针对 STM32F4 系列 MCU（例如 STM32F401 / F407 等）。实际使用时请参考每个示例下的 `main.h`、`gpio.h` 或 `i2c.h`/`spi.h` 中的引脚定义。

常见连接说明：
- OLED (SSD1315) 通常使用 I2C 或 SPI，总线与电源需正确接入。
- ST7735 TFT 使用 SPI，总线引脚（SCK/MOSI/CS/DC/RESET）请对应板子焊接说明。

在 README 或 `Module/*` 文件中通常会有更明确的 pinout 注释，请以工程中的注释为准。

## 依赖与开发工具

- Keil MDK-ARM（推荐用于快速编译与调试）
- STM32CubeMX（可选，用于生成 HAL 初始化代码）
- GNU 工具链（若使用 Makefile 或其他构建工具）

库依赖：
- STM32F4 HAL 驱动（已包含在 `Drivers/STM32F4xx_HAL_Driver/`）
## MCU_Driver — 项目说明

这是一个面向 STM32F4 系列 MCU 的驱动示例仓库，包含 OLED（SSD1315）与 ST7735 TFT 等显示驱动模块、示例工程以及相关字体和 UI 示例。目标是为开发者提供开箱即用的驱动与示例，方便在 Keil/MDK 或命令行工具链中快速集成与调试。

**关键点**：模块化、基于 STM32 HAL、提供 Keil 工程文件以便快速编译与调试。

---

## 目录（概要）

根目录结构（示例）：

- `OLED_SSD1315/`
	- `Core/` — HAL 初始化代码、`main.c`、外设 init
	- `Module/` — `oled.c`、`oled.h`、`oledfont.h` 等
	- `Drivers/` — HAL、CMSIS 驱动
	- `MDK-ARM/` — Keil 工程文件（.uvprojx / .uvoptx）与构建产物

- `ST7735_TFT/` — 与 OLED 目录类似，包含 `st7735.c`、`fonts.c`、UI 示例
- `Module/` — 共享模块或工具（可能为字体、图像资源或通用库）
- `Document/` — 资料、数据手册或原理图（可选）
- `README.md` — 本文件

注：以仓库实际文件夹为准，部分模块可能位于各示例工程内部。

---

## 兼容 MCU 与硬件说明

目标 MCU：STM32F4 系列（例如 STM32F401、STM32F405/407 等）。

硬件连接（示例，实际以代码中的 `gpio.h` / `main.h` 为准）：

- SSD1315 (I2C) 常见引脚：VCC、GND、SCL、SDA、RESET（视模块而定）
- SSD1315 (SPI) 常见引脚：SCK、MOSI、CS、DC、RESET
- ST7735 (SPI) 常见引脚：SCK、MOSI、CS、DC、RESET、BL（背光）

请在使用前打开对应示例目录下的 `*.h` 文件确认具体引脚定义与外设实例（I2C1、SPI1 等）。如需，我可以将某个工程的引脚图以 markdown 表格形式加入 README。

---

## 依赖与开发环境

- 推荐：Keil MDK-ARM（便于直接打开 `.uvprojx` 并调试）
- 可选：STM32CubeMX（生成 HAL 初始化代码）
- 可选（命令行）：arm-none-eabi-gcc / arm-none-eabi-newlib + OpenOCD / ST-Link CLI / STM32CubeProgrammer

仓库已包含：
- `Drivers/STM32F4xx_HAL_Driver/`（HAL 源码）
- `Drivers/CMSIS/`（CMSIS 头文件）

---

## 快速构建与烧录

Keil (GUI)：

1. 打开对应工程目录下的 `MDK-ARM/*.uvprojx`。
2. 选择目标设备（若工程未指定）与调试器（如 ST-Link）。
3. Build -> Rebuild target。
4. 下载到板子：Flash -> Download，或使用 Debug 模式运行。

命令行（示例，基于 arm-none-eabi-gcc）：

1. 准备 `Makefile` 或 `CMake`（仓库未自带通用 Makefile 时需自行添加）。
2. 使用交叉编译器构建并生成 `.elf` / `.bin`。
3. 使用 `STM32CubeProgrammer` 或 `st-flash` / `stlink` 等工具烧录。

示例（仅供参考）：

```powershell
# 使用 STM32CubeProgrammer 烧录（二进制文件）
STM32_Programmer_CLI -c port=SWD -d build/your_project.bin 0x08000000
```

或使用 st-flash（需要安装 stlink 工具）：

```powershell
st-flash write build/your_project.bin 0x8000000
```

---

## 模块概览与 API（快速参考）

- `Module/oled.c` / `oled.h` — OLED 驱动（SSD1315）：
	- 常见函数：`OLED_Init()`、`OLED_Clear()`、`OLED_DrawPixel(x,y)`、`OLED_DrawString(x,y,str)`
- `Module/st7735.c` / `st7735.h` — ST7735 TFT 驱动：
	- 常见函数：`ST7735_Init()`、`ST7735_FillRect()`、`ST7735_DrawChar()`、`ST7735_DrawString()`
- `Module/fonts.c` / `fonts.h` — 字体数据与工具函数

（每个模块顶部注释中通常包含更详细的函数说明与参数示例，请以源码注释为准。）

---

## 完整快速示例（main.c 精简示例）

下列示例展示典型的初始化流程（请根据实际头文件与函数名调整）：

```c
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "i2c.h" // 或 spi.h
#include "oled.h"

int main(void) {
		HAL_Init();
		SystemClock_Config();
		MX_GPIO_Init();
		MX_I2C1_Init(); // 或 MX_SPI1_Init();

		OLED_Init();
		OLED_Clear();
		OLED_DrawString(0, 0, "Hello STM32!");

		while (1) {
				// 应用逻辑
				HAL_Delay(1000);
		}
}
```

将实际函数名替换为你工程中的实现（比如 `OLED_Init()` 在 `Module/oled.c` 中）。如果你希望，我可以把你当前工程的完整 `main.c` 示例提取并整合进 README。

---

## 调试建议与常见问题

- 无法识别外设：检查 VCC/GND、SCL/SDA（或 SCK/MOSI）与芯片选择（CS）等连线。
- I2C 通信失败：确认 I2C 地址、上拉电阻与速率是否正确。
- SPI 显示异常：尝试改变 SPI 模式（CPOL/CPHA）或降低速率。
- 编译报错：确认 `stm32f4xx_hal_conf.h`、包含路径与 HAL 版本匹配。

---

## 贡献与 PR 指南

感谢贡献！推荐流程：

1. Fork 本仓库并新建分支（feature/xxx 或 fix/yyy）。
2. 在本地修改并进行功能测试。
3. 提交简洁的 commit，并发起 PR，说明变更目的、影响范围与测试步骤。

请在 PR 中注明：使用的开发板型号、STM32 HAL 版本与主要测试步骤。

---

## 许可证

仓库当前未强制指定许可证（请根据需要添加，例如 MIT、Apache-2.0 等）。若你希望我为项目添加 `LICENSE`（例如 MIT），我可以自动生成并提交到仓库。

---

## 联系与支持

如需支持或报告问题，请在仓库 Issue 中描述问题、复现步骤及所用硬件/固件版本。

---

如果你希望 README 包含：
- 针对某个工程的完整引脚表、原理图或接线图片；
- 直接可复制粘贴的 `main.c` 完整示例（我会提取并调整为可编译状态）；
- 在仓库根部添加 `Makefile` / `CMake` 配置用于命令行构建；

告诉我你优先要哪一项，我会继续把它加入 README 或在仓库中创建相应文件。


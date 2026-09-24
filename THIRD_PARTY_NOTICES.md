# 第三方代码与许可声明

本仓库中下列部分由 JaysonHZhu 编写，采用 [MIT License](LICENSE) 授权：

- `User/`（应用层：`main.c`、中断服务骨架）
- `Drivers/BSP/`（LED / OLED / BUZZER / TIMER / KEY 驱动）
- `Drivers/SYSTEM/`（自有的系统时钟、延时、串口实现）
- `Projects/MDK-ARM/`（Keil 工程文件）

下列目录包含**第三方提供的代码**，版权归原作者所有。本项目未修改其代码内容，各文件头部的版权与许可声明一律保留。

## STMicroelectronics —— BSD-3-Clause

| 路径 | 说明 |
| --- | --- |
| `Drivers/STM32F1xx_HAL_Driver/Inc/**`、`Drivers/STM32F1xx_HAL_Driver/Src/**` | STM32F1xx HAL 驱动 |
| `Drivers/CMSIS/Device/ST/STM32F1xx/**` | ST 提供的 CMSIS 设备头文件与启动文件 |
| `User/stm32f1xx_it.c`、`User/stm32f1xx_it.h` | ST 提供的中断服务模板 |
| `User/stm32f1xx_hal_conf.h` | ST 提供的 HAL 配置模板 |

许可全文见 [LICENSES/BSD-3-Clause.txt](LICENSES/BSD-3-Clause.txt)。各文件头部保留了原始声明，例如：

```text
Copyright (c) 2016 STMicroelectronics.
This software component is licensed by ST under BSD 3-Clause license.
```

## Arm Limited —— Apache-2.0

| 路径 | 说明 |
| --- | --- |
| `Drivers/CMSIS/Include/**` | CMSIS Core 头文件（`core_cm3.h`、`cmsis_compiler.h`、`mpu_armv7.h` 等） |

许可全文见 [LICENSES/Apache-2.0.txt](LICENSES/Apache-2.0.txt)。各文件头部保留了 `SPDX-License-Identifier: Apache-2.0` 与 Arm 的版权声明。

## 说明

- 本项目不对上述第三方组件作任何担保；使用与再分发前请自行确认其最新许可条款。
- 若你基于本仓库再分发，请同样保留上述版权与许可声明。
- 硬件设计文件（`Hardware/PCB/` 下的原理图、Gerber、钻孔、飞针测试文件）由项目作者使用嘉立创 EDA 绘制并导出，随本仓库一并按 MIT 许可提供，不构成任何制造质量担保。

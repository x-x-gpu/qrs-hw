# QRS 智能网关期末大作业

本仓库整合了 ESP32-C6 多传感器采集节点、RS485 通信、STM32 战舰 V3 主站显示，以及 PC 端 MILP 求解和本地网页可视化控制台。整体目标是完成一个“采集—汇总—求解—控制返回”的智能网关闭环演示。

## 1. 项目分工

| 部分 | 主要内容 | 目录 |
|---|---|---|
| ESP32-C6 采集节点 | HC-SR501、BH1750、AM2302/DHT22 采集，RS485 上行 | `esp32-wroom-c6-workspace/` |
| STM32 RS485 主站 | 战舰 V3 轮询节点、LCD 显示、RS485 调试 | `stm32-warship-v3-rs485-test/` |
| PC MILP 与可视化 | 本地求解器、网页控制台、协议解析、串口返回控制帧 | `code/`、`frontend/`、`data/` |
| STM32 MILP 网关副本 | 接收 `@C` 控制帧，LCD 显示 `MILP CTRL`，输出 ACK | `stm32-warship-v3-milp-gateway/` |
| 报告与资料 | 调研、硬件、软件、移植、算法和调试记录 | `research-docs/`、`vendor-docs/` |

## 2. 系统闭环

```text
ESP32-C6 传感器节点
  -> RS485 @D 数据帧
  -> STM32 战舰 V3 汇总与 LCD 显示
  -> PC 本地 MILP 求解与网页可视化
  -> USART @C 控制帧
  -> STM32 LCD/GPIO 反馈
```

主要协议帧：

```text
@Q,ID=01*00
@D,ID=01,PIR=1,LUX=118.4,T=29.1,H=63.5,BH=ESP_OK,DHT=ESP_OK
@C,LED=1,BUZZER=0,FAN=1,AC=0*00
```

## 3. 快速检查入口

### 3.1 PC 端网页控制台

```powershell
cd .\code\pc_milp_solver
python .\gateway_server.py
```

浏览器访问：

```text
http://127.0.0.1:8765
```

如需自动发送控制帧到 STM32：

```powershell
python .\gateway_server.py --serial-port COM3
```

注意：自动转发时 COM3 不能同时被 XCOM、PuTTY 或 Arduino Serial Monitor 占用。

### 3.2 STM32 MILP 网关固件

```powershell
cd .\stm32-warship-v3-milp-gateway
make
make flash
```

烧录成功后，LCD 应出现 `MILP CTRL` 区域。串口收到 `@C` 帧后，LCD 会更新 LED、BZ、FAN、AC、UPD、U1 RX 和 OV。

### 3.3 ESP32/RS485 原始硬件链路

ESP32-C6 节点和 STM32 RS485 主站仍保留在原目录中：

```text
esp32-wroom-c6-workspace/
stm32-warship-v3-rs485-test/
```

这部分用于证明传感器采集、RS485 轮询和 STM32 LCD 汇总显示正常。

## 4. 研究文档结构

`research-docs/` 按日期和主题组织，便于老师按贡献模块检查：

| 文档目录 | 内容 |
|---|---|
| `2026-05-25-HC-SR501-GY-302-AM2302多节点接入ESP32-C6并发送至战舰V3调研报告/` | 传感器与多节点方案调研 |
| `2026-05-29-ESP32-C6多传感器RS485采集节点硬件方案报告/` | ESP32 节点硬件方案 |
| `2026-05-31-ESP32-C6多节点RS485采集系统软件调试报告/` | ESP32/RS485 软件调试 |
| `2026-06-16-ESP32-C6与战舰V3烧录移植说明报告/` | 烧录、移植和工程文件说明 |
| `2026-06-17-STM32本地MILP可视化网关前期准备报告/` | MILP 网关前期准备与环境整理 |
| `2026-06-18-智能网关MILP算法核心设计报告/` | MILP 变量、约束、目标函数和输出策略 |
| `2026-06-19-STM32与QRS硬件接口对齐报告/` | `@D`、`@Q`、`@C` 协议和串口链路 |
| `2026-06-20-本地网页演示与串口闭环调试报告/` | 网页演示、串口转发、LCD 反馈和问题排查 |

## 5. 演示材料

截图和视频位于：

```text
assets/screenshots/
```

建议检查顺序：

1. 编译环境截图：`make和arm-none-eabi-gcc安装成功.png`
2. 前端页面截图：`前端展示示例.png`
3. 串口通信截图：`串口调试助手确认串口连接通讯正常.png`
4. STM32 LCD 截图：`接入MILP的显示示例.jpg`、`发送控制帧后LCD显示.jpg`
5. 完整演示视频：`接入MILP与前端基本演示.mp4`

## 6. 常见问题

- `OpenOCD open failed`：优先检查 ST-Link 驱动、USB 线、板子供电和连接方式。
- XCOM 发送 `@C` 无响应：确认使用的是 USART 串口，不是 SWD/JTAG 烧录口。
- Python 打开 COM3 报拒绝访问：关闭占用该串口的 XCOM 或其他串口工具。
- LCD 显示 `NO REPLY`：通常是 RS485 节点回包超时，不等同于 PC 到 STM32 的控制帧失败。
- LCD 显示 `LED:ON` 但物理灯未亮：检查战舰 V3 板卡版本、GPIO 映射和有效电平。

## 7. 推荐答辩路线

先展示 ESP32/RS485/STM32 的硬件采集链路，再打开 PC 网页控制台修改输入并求解，最后通过串口把 `@C` 帧返回 STM32，观察 LCD 的 `MILP CTRL` 区域和串口 ACK。这样可以把两位成员的贡献自然串成一个完整闭环，而不是把算法和硬件割裂展示。

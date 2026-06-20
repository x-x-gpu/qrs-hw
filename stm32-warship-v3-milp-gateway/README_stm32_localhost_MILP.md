# stm32_localhost_MILP

本目录归档智能网关课程设计中“软件算法、PC 端求解、前端展示和功能演示”的部分。硬件采集、ESP32-C6 节点、STM32 战舰 V3 RS485 主站代码保留在相邻的 `qrs-hw` 仓库中，本目录只读取其协议和配置，不修改原仓库。

## 分工定位

- 合作同学：ESP32-C6 传感器采集、STM32 战舰 V3 主站、RS485 通信、LCD 显示、硬件接线和调试。
- 本目录：PC 端 MILP 调度模型、串口协议整理、前端可视化页面、演示流程和报告中算法部分材料。

## 当前归档结构

```text
stm32_localhost_MILP/
  README.md
  code/
    MILP_01.cpp                 # UTF-8 修复后的嵌入式枚举策略原型
    MILP_dp.cpp                 # UTF-8 修复后的嵌入式 DP 策略原型
    pc_milp_solver/
      gateway_milp.py           # PC 端 MILP 风格求解脚本，无第三方依赖
      gateway_server.py         # localhost 交互式智能网关控制台后端
    protocol/
      qrs_frames.py             # qrs-hw @D 数据帧解析和 @C 控制帧生成
  data/
    sample_sensor_snapshot.json # 演示输入样例
    latest_solution.json        # 运行脚本后生成，供前端导入
  docs/
    algorithm_design.md         # MILP 建模说明
    demo_workflow.md            # 完整调试和演示流程
    interface_with_qrs_hw.md    # 与 qrs-hw 的协议和配置对齐
    stm32_milp_gateway_plan.md  # STM32 显示和控制帧接入计划
  frontend/
    index.html
    styles.css
    app.js
  stm32-warship-v3-milp-gateway/
    main.c                      # qrs-hw STM32 工程的独立副本
    startup.c
    stm32f103ze.ld
    Makefile
    README.md                   # 构建和烧录说明
  assets/
    screenshots/                # 后续放演示截图
```

## 快速运行交互控制台

推荐运行本地控制台服务：

```powershell
cd .\code\pc_milp_solver
python .\gateway_server.py
```

浏览器打开：

```text
http://127.0.0.1:8765
```

控制台支持：

- 编辑电价、功率上限、目标温度。
- 编辑 ID01/ID02 的 PIR、光照、温湿度。
- 粘贴 `qrs-hw` 中 ESP32 节点返回的 `@D` 帧。
- 点击求解，实时生成 MILP 最优策略。
- 生成返回 STM32/ESP32 的控制帧，例如 `@C,LED=1,BUZZER=0,FAN=1,AC=1*00`。

闭环逻辑：

```text
ESP32 传感器输入 -> STM32/RS485 网关汇总 -> PC MILP 求解 -> 控制帧返回 STM32/ESP32 -> LED/蜂鸣器反馈
```

## 命令行求解

在本目录下运行 PC 求解器：

```powershell
python .\code\pc_milp_solver\gateway_milp.py --input ..\..\data\sample_sensor_snapshot.json
```

更稳的方式是直接在脚本目录运行默认相对路径：

```powershell
cd .\code\pc_milp_solver
python .\gateway_milp.py
```

运行后会生成：

```text
data/latest_solution.json
```

命令行模式主要用于报告截图和检查 JSON 输出；正式演示建议使用 `gateway_server.py`。

## STM32 固件副本

STM32 侧改造放在：

```text
stm32-warship-v3-milp-gateway/
```

这个目录是从 `qrs-hw/stm32-warship-v3-rs485-test` 复制出来的独立副本。后续图形化界面、MILP 控制状态显示、`@C` 控制帧解析都在这里改，不动原仓库。

推荐构建和烧录：

```powershell
cd .\stm32-warship-v3-milp-gateway
make
make flash
```

这条路线使用 `arm-none-eabi-gcc` 和 OpenOCD/ST-Link。当前不建议先用 CubeMX 重建，因为原工程是寄存器级裸机工程，LCD、FSMC、USART3、RS485 方向控制都已经手写跑通；CubeMX/Keil 可以作为备选，但移植工作量更大。

## 与 qrs-hw 对齐的关键配置

从 `qrs-hw` 当前代码读取到的接口如下：

- ESP32-C6 节点工程：ESP-IDF v5.5.1，目标芯片 `esp32c6`。
- 传感器接线：BH1750 SDA=GPIO6、SCL=GPIO7；AM2302 DATA=GPIO11；HC-SR501 OUT=GPIO10。
- RS485：ESP32 UART1，TX=GPIO20、RX=GPIO21、DE,/RE=GPIO18，波特率 38400。
- 节点上行帧：

```text
@D,ID=01,PIR=1,LUX=118.4,T=29.1,H=63.5,BH=ESP_OK,DHT=ESP_OK
```

- STM32 主站轮询帧：

```text
@Q,ID=01*00
@Q,ID=02*00
```

- STM32 LCD 约 5 秒刷新一次汇总。

本目录提出 PC 到网关的控制帧，作为最终智能控制闭环的返回通道：

```text
@C,LED=1,BUZZER=0,FAN=1,AC=0*00
```

其中 LED 和 BUZZER 对应当前可用执行端；FAN/AC 是楼宇设备代理变量，演示时可以作为前端状态或未来扩展负载。若硬件侧暂时不修改，控制帧先在 PC 控制台中展示；若后续允许对接，可在 STM32 或 ESP32 端解析该帧并驱动 GPIO。

## 调试记录

2026-06-12：

- 读取课程题目要求，确认题目 2 需要智能网关、多类型变量、电气/温度等测量、MILP 优化。
- 读取 `qrs-hw`，确认已有 ESP32-C6 多传感器采集、RS485 上行、STM32 主站轮询和 LCD 显示。
- 明确本目录只承载软件算法、PC 求解、前端展示，不修改 `qrs-hw`。
- 修复 `code/MILP_01.cpp` 和 `code/MILP_dp.cpp` 的乱码注释，统一为 UTF-8/ASCII 注释。
- 新增 PC 端求解脚本 `code/pc_milp_solver/gateway_milp.py`，使用二进制决策变量精确枚举小规模 MILP。
- 新增 localhost 后端 `code/pc_milp_solver/gateway_server.py`，提供 `/api/state`、`/api/solve`、`/api/frame`、`/api/reset`。
- 新增协议工具 `code/protocol/qrs_frames.py`，用于解析 `@D` 传感器帧并生成 `@C` 控制帧。
- 将前端升级为交互式智能网关控制台，可调参数、模拟传感器输入、粘贴硬件帧、实时求解并生成反馈控制帧。
- 新增 `docs/` 文档，便于人工回溯和报告撰写。

## 后续建议

- 如果硬件侧允许，可以在 STM32 端增加 `@C` 控制帧接收，并把 LED/蜂鸣器作为实际执行端。
- 如果需要严格展示“PC 求解 MILP”，可以把 `gateway_milp.py` 的枚举求解替换为 PuLP/OR-Tools，同时保留当前输入输出格式。
- 报告中建议说明：当前硬件没有额外电气计量模块，因此电气变量采用功率额定值和功率上限作为调度约束；温湿度、光照、人体感应用真实传感器数据。

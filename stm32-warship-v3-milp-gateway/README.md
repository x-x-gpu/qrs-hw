# STM32 Warship V3 MILP Gateway

This folder is an independent copy of the working STM32 RS485 gateway from
`qrs-hw/stm32-warship-v3-rs485-test`. It has been modified to add a PC command
channel (USART1) for receiving MILP control frames and displaying them on the LCD.

## Current Baseline

- MCU: STM32F103ZE on ALIENTEK Warship V3.
- Display: 4.3 inch TFT LCD driven by the existing bare-metal FSMC/GPIO code.
- Bus: RS485 master on USART3.
- Pins inherited from the working baseline:
  - `PB10`: USART3_TX
  - `PB11`: USART3_RX
  - `PF12`: RS485 RSE / direction control
- Baud rate (RS485): 38400 8N1.
- Node polling: `@Q,ID=01*00` and `@Q,ID=02*00` via RS485.
- Node response: `@D,ID=01,PIR=1,LUX=118.4,T=29.1,H=63.5,...`

## MILP Control Channel (USART1)

### Overview

A secondary UART (USART1) is configured to receive `@C` control frames from the
PC over the board's USB-to-serial (CH340G) port. This creates a side channel
for the MILP solver to send control decisions to the STM32 without interfering
with the RS485 sensor bus.

### Data Flow

```
PC MILP Solver -> USB Serial -> CH340G -> USART1 (PA9/PA10) -> STM32
                                               |
                                               v
                                        @C Parser
                                               |
                                    +----------+----------+
                                    |                     |
                                    v                     v
                                 LCD Display          GPIO Output
                            (MILP CTRL status)     (LED/Buzzer)
```

### USART1 Configuration

| Parameter | Value |
|-----------|-------|
| TX Pin    | PA9 (alternate function push-pull, 50 MHz) |
| RX Pin    | PA10 (floating input) |
| Baud rate | 115200 8N1 |
| Ring buf  | 128 bytes |
| IRQ       | Position 37 (NVIC) |

The board connects USART1 to the onboard CH340G USB-to-serial converter.
Plug a USB cable from the PC to the board's USB serial port. On the PC side,
open the corresponding COM port at 115200 baud and send `@C` frames.

### @C Frame Format

```
@C,LED=1,BUZZER=0,FAN=1,AC=1*00
```

Fields:
- `LED=0/1`: Onboard LED control (LCD display + GPIO PB5 output)
- `BUZZER=0/1`: Buzzer control (LCD display + GPIO PF8 output)
- `FAN=0/1`: Fan proxy variable (LCD display only)
- `AC=0/1`: Air conditioner proxy variable (LCD display only)

The `*00` is a checksum placeholder matching the existing RS485 frame format.

### LCD Status Area

After the ID01/ID02 sensor data, the LCD shows:

```
MILP CTRL  UPD:3
LED:ON  BZ:OFF
FAN:ON  AC:ON
```

- `UPD` counts how many @C frames have been received.
- Device states are shown as ON/OFF or --- (if no frame received yet).

## Modified Files

| File | Changes |
|------|---------|
| `main.c` | +176 lines: USART1 registers, ring buffer, IRQ handler, @C parser, LCD rendering, init |
| `startup.c` | +2 lines: USART1_IRQHandler weak alias + vector table entry |
| `Makefile` | Output firmware renamed to `warship_v3_milp_gateway` |

## Build

Use the existing bare-metal Makefile flow:

```powershell
cd D:\Kaiyuan\UGRD\大二\作业\大二下\嵌入式\期末大作业\stm32_localhost_MILP\stm32-warship-v3-milp-gateway
make
```

Expected output:

```text
build/warship_v3_milp_gateway.elf
build/warship_v3_milp_gateway.bin
```

The Makefile requires:
- `arm-none-eabi-gcc`
- `arm-none-eabi-objcopy`
- `arm-none-eabi-size`

## Flash

```powershell
make flash
```

This uses OpenOCD:

```text
openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program build/warship_v3_milp_gateway.elf verify reset exit"
```

Hardware requirement: ST-Link connected to SWDIO, SWCLK, GND, and target 3V3
reference. Warship V3 board powered normally.

## Testing the MILP Control Channel

1. Build and flash the firmware (`make && make flash`).
2. Connect the board's USB serial port to the PC.
3. Identify the COM port (e.g., COM3) in Device Manager.
4. Open a serial terminal at 115200 baud (PuTTY, Arduino Serial Monitor, etc.).
5. Send a test command:

```
@C,LED=1,BUZZER=0,FAN=1,AC=1*00
```

6. The LCD should show `MILP CTRL  UPD:1` with LED:ON, BZ:OFF, FAN:ON, AC:ON.
7. The serial terminal should receive `ACK @C UPD=1` after the valid frame.

The LCD also shows a USART1 debug line:

```text
U1 RX:33 OV:0
```

`RX` is the number of bytes received by USART1. `OV` is the USART1 ring-buffer
overflow count and should normally stay at 0.

## Troubleshooting

- **No MILP CTRL on LCD**: Verify USART1 is initialized (`usart1_init()` in `main()`).
- **No `STM32 MILP UART1 READY` in the serial terminal**: Check that the terminal is opened on the CH340/USB-serial COM port, not the ST-Link SWD/JTAG debug port.
- **`U1 RX` does not increase after sending**: Check USB-serial wiring, COM port selection, and baud rate 115200 8N1.
- **`U1 RX` increases but `UPD` does not**: Check that the frame starts with `@C` and the terminal sends a newline (`CRLF` is recommended).
- **Python auto-forwarding reports `PermissionError(13)`**: Close XCOM/PuTTY/other serial monitors first. Windows serial ports are exclusive and only one program can open `COM3` at a time.
- **@C frame not updating**: Check USART1 baud rate matches the terminal setting.
- **Build errors**: Run `make clean && make` to rebuild from scratch.
- **Flash errors**: Verify ST-Link connection and board power.

## Debug Log

During bring-up, the first visible symptom was that the LCD showed
`MILP CTRL UPD:0 LED:-- BZ:--` even after sending a control frame from XCOM.
The RS485 sensor part was already healthy, so the fault was isolated to the
PC-to-STM32 USART1 return path.

The key issue was the interrupt vector table entry. `USART1_IRQn` is 37, but
the Cortex-M vector table indexes peripheral interrupts after the 16 core
exception entries, so the actual table index must be 53. The startup vector was
changed to:

```c
[53] = USART1_IRQHandler,
```

To make later debugging visible, USART1 now prints:

```text
STM32 MILP UART1 READY
ACK @C UPD=1
```

and the LCD shows `U1 RX/OV` counters. A successful manual XCOM test should
therefore show all three signals: serial READY text, serial ACK text, and LCD
`MILP CTRL` update.

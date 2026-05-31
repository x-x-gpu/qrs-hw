# ESP32-C6-WROOM-1 ESP-IDF Project

This workspace is configured for ESP-IDF v5.5.1 and target `esp32c6`.

## Linux Setup

ESP-IDF is installed with Espressif Installation Manager under:

```text
/home/x/.espressif/v5.5.1/esp-idf
```

Activate it in a shell with:

```bash
source "$HOME/.espressif/tools/activate_idf_v5.5.1.sh"
```

This project also provides a wrapper that activates ESP-IDF automatically:

```bash
./tools/idf.sh set-target esp32c6
./tools/idf.sh build
./tools/idf.sh -p /dev/ttyACM0 flash
./tools/idf.sh -p /dev/ttyACM0 monitor
./tools/idf.sh -p /dev/ttyACM0 flash monitor
```

If ESP-IDF must be reinstalled on Ubuntu without `python3-venv`, use:

```bash
ESP_IDF_PROXY=http://127.0.0.1:7897 ./tools/install-esp-idf-linux.sh
```

Omit `ESP_IDF_PROXY` if the network works directly. The local Python wrapper in
`tools/python3-venv-wrapper/` only exists to let EIM create a virtual environment
on systems where `python3-venv` cannot be installed with sudo.

## VS Code

Open the repository root and run tasks from `Terminal > Run Task`:

- `ESP-IDF: build`
- `ESP-IDF: flash`
- `ESP-IDF: monitor`
- `ESP-IDF: flash and monitor`
- `ESP-IDF: flash sensor check`
- `ESP-IDF: menuconfig`
- `ESP-IDF: fullclean`

The flash and monitor tasks ask for the serial port. On Linux this is usually
`/dev/ttyACM0` for the native USB Serial/JTAG port or `/dev/ttyUSB0` for a
USB-UART bridge.

## Sensor Flash Helper

For the current sensor wiring check:

```bash
./tools/flash-monitor-sensors.sh -p /dev/ttyACM0
```

Without `-p`, the script tries to auto-detect a likely ESP32-C6 serial port.

The expected wiring is:

- BH1750/GY-302: SDA=GPIO6, SCL=GPIO7, VCC=3V3, GND=GND
- AM2302/DHT22: DATA=GPIO11, VCC=3V3, GND=GND
- HC-SR501: OUT=GPIO10, VCC=5V/VBUS, GND=GND

## Linux Permissions Note

During installation, EIM could not copy OpenOCD udev rules to `/etc/udev/rules.d`
because that requires root. UART flashing and monitoring should still work if the
user is in the appropriate serial-device groups. If JTAG/OpenOCD access is needed,
copy the rule manually:

```bash
sudo cp "$HOME/.espressif/tools/openocd-esp32/v0.12.0-esp32-20250707/openocd-esp32/share/openocd/contrib/60-openocd.rules" /etc/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger
```

## Windows Legacy

The original PowerShell wrappers remain in `tools/*.ps1` for reference. The
Linux workflow uses the Bash scripts above and builds directly in this folder.

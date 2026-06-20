#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
IDF_WRAPPER="$SCRIPT_DIR/idf.sh"

PORT=""
MONITOR=1

usage() {
    cat <<'EOF'
Usage: tools/flash-monitor-sensors.sh [-p PORT] [--no-monitor]

Builds the ESP32-C6 sensor check app, flashes it, and starts the serial monitor.
EOF
}

while [ "$#" -gt 0 ]; do
    case "$1" in
        -p|--port)
            PORT="${2:-}"
            if [ -z "$PORT" ]; then
                printf 'Missing value after %s\n' "$1" >&2
                exit 2
            fi
            shift 2
            ;;
        --no-monitor)
            MONITOR=0
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            printf 'Unknown argument: %s\n' "$1" >&2
            usage >&2
            exit 2
            ;;
    esac
done

detect_serial_port() {
    local candidates=()
    local port

    while IFS= read -r port; do
        candidates+=("$port")
    done < <(
        {
            find /dev/serial/by-id -maxdepth 1 -type l 2>/dev/null | sort
            printf '%s\n' /dev/ttyACM* /dev/ttyUSB* 2>/dev/null
        } | while IFS= read -r candidate; do
            [ -e "$candidate" ] && printf '%s\n' "$candidate"
        done | awk '!seen[$0]++'
    )

    if [ "${#candidates[@]}" -eq 0 ]; then
        printf 'No serial port found. Reconnect the ESP32-C6 board and check USB permissions.\n' >&2
        return 1
    fi

    for port in "${candidates[@]}"; do
        case "$port" in
            *Espressif*|*USB_JTAG*|*USB-Serial*|*USB_Serial*|*CP210*|*CH340*|*CH343*|*WCH*)
                printf '%s\n' "$port"
                return 0
                ;;
        esac
    done

    if [ "${#candidates[@]}" -eq 1 ]; then
        printf '%s\n' "${candidates[0]}"
        return 0
    fi

    printf 'Multiple serial ports found. Re-run with -p PORT:\n' >&2
    printf '  %s\n' "${candidates[@]}" >&2
    return 1
}

if [ -z "$PORT" ]; then
    PORT="$(detect_serial_port)"
fi

printf 'Project: %s\n' "$PROJECT_ROOT"
printf 'Serial port: %s\n' "$PORT"
printf 'Expected sensor wiring:\n'
printf '  BH1750/GY-302: SDA=GPIO6, SCL=GPIO7, VCC=3V3, GND=GND\n'
printf '  AM2302/DHT22:  DATA=GPIO11, VCC=3V3, GND=GND\n'
printf '  HC-SR501:      OUT=GPIO10, VCC=5V/VBUS, GND=GND\n'
printf '  RS485 module:  TX=GPIO20, RX=GPIO21, DE,/RE=GPIO18, VCC=3V3, GND=GND\n'

"$IDF_WRAPPER" build

if [ "$MONITOR" -eq 1 ]; then
    printf 'Starting flash + monitor. Press Ctrl+] to exit the ESP-IDF monitor.\n'
    "$IDF_WRAPPER" -p "$PORT" flash monitor
else
    "$IDF_WRAPPER" -p "$PORT" flash
fi

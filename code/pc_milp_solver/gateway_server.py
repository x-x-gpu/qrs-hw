#!/usr/bin/env python3
"""Localhost smart-gateway controller.

This server closes the demo loop:

    ESP32/STM32 sensor frame -> PC snapshot -> MILP solve -> control frame

It uses only Python standard-library modules so the classroom demo can run on a
clean Windows machine. Optional serial port forwarding requires pyserial.
"""

from __future__ import annotations

import argparse
import json
import threading
from copy import deepcopy
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import urlparse

from gateway_milp import solve_to_result

ROOT = Path(__file__).resolve().parents[2]
FRONTEND = ROOT / "frontend"
DATA = ROOT / "data"
SAMPLE = DATA / "sample_sensor_snapshot.json"
LATEST = DATA / "latest_solution.json"

SERIAL_PORT = None
SERIAL_BAUD = 115200
SERIAL_ACK_TIMEOUT = 3
SERIAL_CONN = None
SERIAL_WRITE_LOCK = threading.Lock()
STATE_LOCK = threading.RLock()


def load_initial_snapshot() -> dict:
    return json.loads(SAMPLE.read_text(encoding="utf-8"))


STATE = {
    "snapshot": load_initial_snapshot(),
    "last_result": None,
    "solve_history": [],
}

MAX_HISTORY = 50


def parse_key_value_frame(frame: str) -> dict:
    text = frame.strip()
    if "*" in text:
        text = text.split("*", 1)[0]
    parts = [part for part in text.split(",") if part]
    if not parts:
        raise ValueError("empty frame")

    result = {"frame_type": parts[0]}
    for part in parts[1:]:
        if "=" not in part:
            result.setdefault("flags", []).append(part)
            continue
        key, value = part.split("=", 1)
        result[key] = value
    return result


def data_frame_to_node(frame: str) -> dict:
    fields = parse_key_value_frame(frame)
    if fields.get("frame_type") != "@D":
        raise ValueError(f"expected @D frame, got {fields.get('frame_type')}")
    return {
        "id": fields.get("ID", "00"),
        "pir": int(fields.get("PIR", "0")),
        "lux": float(fields.get("LUX", "-1")),
        "temperature_c": float(fields.get("T", "-999")),
        "humidity_percent": float(fields.get("H", "-999")),
        "bh_status": fields.get("BH", "UNKNOWN"),
        "dht_status": fields.get("DHT", "UNKNOWN"),
    }


def upsert_node(snapshot: dict, node: dict) -> None:
    nodes = snapshot.setdefault("nodes", [])
    for index, current in enumerate(nodes):
        if str(current.get("id")) == str(node.get("id")):
            nodes[index] = node
            return
    nodes.append(node)


def solve_current_state(send_serial: bool = True) -> dict:
    snapshot = deepcopy(STATE["snapshot"])
    result = solve_to_result(snapshot)
    STATE["snapshot"] = result["snapshot"]
    STATE["last_result"] = result

    STATE["solve_history"].append({
        "timestamp": result.get("snapshot", {}).get("_derived_average", {}),
        "solution": result.get("solution", {}),
        "solve_time_ms": result.get("solve_time_ms", 0),
        "stm32_policy_frame": result.get("stm32_policy_frame", ""),
    })
    if len(STATE["solve_history"]) > MAX_HISTORY:
        STATE["solve_history"] = STATE["solve_history"][-MAX_HISTORY:]

    LATEST.write_text(json.dumps(result, ensure_ascii=False, indent=2), encoding="utf-8")

    frame = result.get("stm32_policy_frame", "")
    if send_serial and frame:
        try_send_serial(frame)

    return result


def handle_serial_line(line: str) -> None:
    text = line.strip()
    if not text:
        return

    print(f"  [serial] STM32: {text}")
    if text.startswith("@D"):
        try:
            node = data_frame_to_node(text)
            with STATE_LOCK:
                upsert_node(STATE["snapshot"], node)
                solve_current_state(send_serial=True)
        except Exception as exc:
            print(f"  [serial] Failed to parse STM32 @D frame: {exc}")


def serial_reader_loop() -> None:
    while SERIAL_CONN is not None:
        try:
            raw = SERIAL_CONN.readline()
            if not raw:
                continue
            handle_serial_line(raw.decode("ascii", errors="replace"))
        except Exception as exc:
            print(f"  [serial] Reader stopped: {exc}")
            break


def start_serial_bridge(port: str) -> None:
    global SERIAL_CONN

    try:
        import serial
        SERIAL_CONN = serial.Serial(port, SERIAL_BAUD, timeout=0.2)
        SERIAL_CONN.reset_input_buffer()
        print(f"Serial bridge opened: {port} at {SERIAL_BAUD} baud")
        thread = threading.Thread(target=serial_reader_loop, daemon=True)
        thread.start()
    except ImportError:
        print("  [serial] pyserial not installed; install with: pip install pyserial")
    except PermissionError:
        print(f"  [serial] Failed to open {port}: port is busy. Close XCOM/serial monitor first.")
    except Exception as exc:
        print(f"  [serial] Failed to open {port}: {exc}")


def try_send_serial(frame: str) -> None:
    if SERIAL_PORT is None:
        return
    try:
        if SERIAL_CONN is not None and SERIAL_CONN.is_open:
            with SERIAL_WRITE_LOCK:
                SERIAL_CONN.write((frame + "\r\n").encode("ascii"))
                SERIAL_CONN.flush()
            print(f"  [serial] Sent to {SERIAL_PORT}: {frame}")
            return

        import serial
        with serial.Serial(SERIAL_PORT, SERIAL_BAUD, timeout=SERIAL_ACK_TIMEOUT) as ser:
            ser.write((frame + "\r\n").encode("ascii"))
            ser.flush()
            ack = ser.read_until(b"\n", 96).decode("ascii", errors="replace").strip()
        print(f"  [serial] Sent to {SERIAL_PORT}: {frame}")
        if ack:
            print(f"  [serial] STM32 replied: {ack}")
        else:
            print("  [serial] No STM32 reply before timeout; if LCD updated, forwarding still worked.")
    except ImportError:
        print("  [serial] pyserial not installed; install with: pip install pyserial")
    except PermissionError:
        print(f"  [serial] Failed to open {SERIAL_PORT}: port is busy. Close XCOM/serial monitor first.")
    except Exception as exc:
        print(f"  [serial] Failed to send to {SERIAL_PORT}: {exc}")


def solve_and_store() -> dict:
    with STATE_LOCK:
        return solve_current_state(send_serial=True)


def response(handler: SimpleHTTPRequestHandler, status: int, payload: dict) -> None:
    body = json.dumps(payload, ensure_ascii=False, indent=2).encode("utf-8")
    handler.send_response(status)
    handler.send_header("Content-Type", "application/json; charset=utf-8")
    handler.send_header("Content-Length", str(len(body)))
    handler.end_headers()
    handler.wfile.write(body)


class GatewayHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=str(FRONTEND), **kwargs)

    def do_GET(self) -> None:
        path = urlparse(self.path).path
        if path == "/api/state":
            result = STATE["last_result"] or solve_and_store()
            response(self, 200, result)
            return
        if path == "/api/history":
            response(self, 200, {"history": STATE["solve_history"]})
            return
        if path == "/":
            self.path = "/index.html"
        super().do_GET()

    def do_POST(self) -> None:
        path = urlparse(self.path).path
        try:
            length = int(self.headers.get("Content-Length", "0"))
            body = self.rfile.read(length).decode("utf-8") if length else "{}"
            payload = json.loads(body)

            if path == "/api/solve":
                snapshot = payload.get("snapshot")
                if not isinstance(snapshot, dict):
                    raise ValueError("missing snapshot object")
                STATE["snapshot"] = snapshot
                response(self, 200, solve_and_store())
                return

            if path == "/api/frame":
                frame = str(payload.get("frame", ""))
                node = data_frame_to_node(frame)
                upsert_node(STATE["snapshot"], node)
                response(self, 200, solve_and_store())
                return

            if path == "/api/reset":
                STATE["snapshot"] = load_initial_snapshot()
                STATE["last_result"] = None
                STATE["solve_history"] = []
                response(self, 200, solve_and_store())
                return

            response(self, 404, {"error": f"unknown endpoint: {path}"})
        except Exception as exc:
            response(self, 400, {"error": str(exc)})


def main() -> int:
    global SERIAL_PORT

    parser = argparse.ArgumentParser(description="Smart gateway MILP control server")
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=8765)
    parser.add_argument(
        "--serial-port",
        default=None,
        help="COM port for auto-forwarding @C frames to STM32 (e.g. COM3)",
    )
    args = parser.parse_args()

    if args.serial_port:
        SERIAL_PORT = args.serial_port
        print(f"Serial port configured: {SERIAL_PORT} at {SERIAL_BAUD} baud")
        start_serial_bridge(SERIAL_PORT)

    solve_and_store()
    server = ThreadingHTTPServer((args.host, args.port), GatewayHandler)
    print(f"Smart gateway console: http://{args.host}:{args.port}")
    print("Press Ctrl+C to stop.")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nStopped.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

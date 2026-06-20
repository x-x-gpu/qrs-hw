#!/usr/bin/env python3
"""Helpers for the qrs-hw RS485 text protocol.

Existing qrs-hw sensor frame:
    @D,ID=01,PIR=1,LUX=120.5,T=28.3,H=61.2,BH=ESP_OK,DHT=ESP_OK

Proposed PC-to-gateway control frame:
    @C,LED=1,BUZZER=0,FAN=1,AC=0*00
"""

from __future__ import annotations

import argparse
import json
from pathlib import Path


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


def make_control_frame(commands: dict) -> str:
    return (
        "@C,"
        f"LED={int(commands.get('LED', 0))},"
        f"BUZZER={int(commands.get('BUZZER', 0))},"
        f"FAN={int(commands.get('FAN_PROXY', commands.get('FAN', 0)))},"
        f"AC={int(commands.get('AC_PROXY', commands.get('AC', 0)))}*00"
    )


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--frame", required=True, help="Raw @D sensor frame.")
    parser.add_argument("--output", help="Optional JSON output path.")
    args = parser.parse_args()

    node = data_frame_to_node(args.frame)
    print(json.dumps(node, ensure_ascii=False, indent=2))
    if args.output:
        Path(args.output).write_text(json.dumps(node, ensure_ascii=False, indent=2), encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

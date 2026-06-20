#!/usr/bin/env python3
"""PC-side MILP-style optimizer for the smart-gateway demonstration.

The model uses binary decision variables for controllable loads:

    x_light, x_fan, x_alert, x_ac

The objective maximizes comfort and safety reward while subtracting energy cost.
For the course demo we solve the binary model by exhaustive enumeration, which
is exact for this small instance and keeps the script dependency-free. The
formulation remains MILP-compatible and can be moved to PuLP, OR-Tools, or
scipy.optimize.milp later without changing the interface.
"""

from __future__ import annotations

import argparse
import itertools
import json
import time
from dataclasses import dataclass, asdict
from pathlib import Path
from typing import Dict, Iterable, List


@dataclass(frozen=True)
class Device:
    name: str
    command: str
    power_w: int
    base_reward: float


@dataclass
class Solution:
    objective: float
    total_power_w: int
    estimated_cost_yuan_per_hour: float
    commands: Dict[str, int]
    reasons: List[str]


DEVICES = [
    Device("light", "LED", 8, 2.0),
    Device("fan", "FAN_PROXY", 120, 3.0),
    Device("alert", "BUZZER", 2, 0.5),
    Device("ac", "AC_PROXY", 1500, 7.0),
]


def average(nodes: Iterable[dict], field: str, fallback: float) -> float:
    values = [float(node[field]) for node in nodes if field in node]
    return sum(values) / len(values) if values else fallback


def occupied(nodes: Iterable[dict]) -> bool:
    return any(int(node.get("pir", 0)) != 0 for node in nodes)


def build_dynamic_rewards(snapshot: dict) -> Dict[str, float]:
    nodes = snapshot.get("nodes", [])
    avg_lux = average(nodes, "lux", 300.0)
    avg_temp = average(nodes, "temperature_c", 26.0)
    avg_humidity = average(nodes, "humidity_percent", 55.0)
    is_occupied = occupied(nodes)
    target_temp = float(snapshot.get("target_temperature_c", 26.0))

    rewards = {device.name: device.base_reward for device in DEVICES}
    reasons: List[str] = []

    if avg_lux < 180 and is_occupied:
        rewards["light"] += 6.0
        reasons.append("Low light and occupancy raise LED/lighting priority.")
    if avg_temp > target_temp + 1.0 and is_occupied:
        rewards["fan"] += 5.0
        rewards["ac"] += 8.0
        reasons.append("Temperature is above target, so cooling devices gain reward.")
    if avg_humidity > 70:
        rewards["fan"] += 2.0
        reasons.append("High humidity raises ventilation priority.")
    if is_occupied and (avg_temp > 32 or avg_humidity > 80):
        rewards["alert"] += 8.0
        reasons.append("Extreme comfort condition enables buzzer warning reward.")
    if not is_occupied:
        rewards["light"] -= 5.0
        rewards["fan"] -= 2.0
        rewards["ac"] -= 4.0
        reasons.append("No PIR occupancy, so comfort loads are discouraged.")

    snapshot["_derived_reasons"] = reasons
    snapshot["_derived_average"] = {
        "lux": round(avg_lux, 1),
        "temperature_c": round(avg_temp, 1),
        "humidity_percent": round(avg_humidity, 1),
        "occupied": is_occupied,
    }
    return rewards


def solve(snapshot: dict) -> Solution:
    rewards = build_dynamic_rewards(snapshot)
    price = float(snapshot.get("price_yuan_per_kwh", 1.0))
    power_limit_w = int(snapshot.get("power_limit_w", 1800))

    best: Solution | None = None

    for bits in itertools.product([0, 1], repeat=len(DEVICES)):
        total_power = sum(device.power_w * bit for device, bit in zip(DEVICES, bits))
        if total_power > power_limit_w:
            continue

        cost = (total_power / 1000.0) * price
        reward = sum(rewards[device.name] * bit for device, bit in zip(DEVICES, bits))
        objective = reward - cost
        commands = {device.command: int(bit) for device, bit in zip(DEVICES, bits)}

        candidate = Solution(
            objective=round(objective, 3),
            total_power_w=total_power,
            estimated_cost_yuan_per_hour=round(cost, 3),
            commands=commands,
            reasons=[],
        )

        if best is None or candidate.objective > best.objective:
            best = candidate

    if best is None:
        best = Solution(
            objective=0.0,
            total_power_w=0,
            estimated_cost_yuan_per_hour=0.0,
            commands={device.command: 0 for device in DEVICES},
            reasons=["Power limit is too small; all controllable loads are disabled."],
        )

    best.reasons = snapshot.get("_derived_reasons", [])
    return best


def make_stm32_policy_frame(solution: Solution) -> str:
    return (
        "@C,"
        f"LED={solution.commands.get('LED', 0)},"
        f"BUZZER={solution.commands.get('BUZZER', 0)},"
        f"FAN={solution.commands.get('FAN_PROXY', 0)},"
        f"AC={solution.commands.get('AC_PROXY', 0)}*00"
    )


def solve_to_result(snapshot: dict) -> dict:
    t0 = time.perf_counter()
    solution = solve(snapshot)
    elapsed_ms = int((time.perf_counter() - t0) * 1000)

    return {
        "snapshot": snapshot,
        "model": {
            "decision_variables": [f"x_{device.name} in {{0,1}}" for device in DEVICES],
            "objective": "maximize comfort_reward - hourly_energy_cost",
            "constraints": [
                "sum(power_i * x_i) <= power_limit_w",
                "x_i are binary control decisions",
            ],
        },
        "solution": asdict(solution),
        "stm32_policy_frame": make_stm32_policy_frame(solution),
        "solve_time_ms": elapsed_ms,
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--input",
        default="../../data/sample_sensor_snapshot.json",
        help="Sensor snapshot JSON file.",
    )
    parser.add_argument(
        "--output",
        default="../../data/latest_solution.json",
        help="Path for the JSON result used by the frontend.",
    )
    args = parser.parse_args()

    base = Path(__file__).resolve().parent
    input_path = (base / args.input).resolve()
    output_path = (base / args.output).resolve()

    snapshot = json.loads(input_path.read_text(encoding="utf-8"))
    result = solve_to_result(snapshot)

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(json.dumps(result, ensure_ascii=False, indent=2), encoding="utf-8")
    print(json.dumps(result, ensure_ascii=False, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

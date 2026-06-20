#include <Arduino.h>
#include <vector>

// Dynamic-programming variant for small embedded targets.
// It is not a full MILP solver; it is a 0-1 knapsack approximation that can be
// explained as the lightweight fallback when the PC optimizer is unavailable.
struct Device {
    String name;
    int power_w;
    int comfort_score;
    bool enabled;
};

Device appliances[] = {
    {"AirConditioner", 1500, 10, false},
    {"WaterHeater", 2000, 8, false},
    {"WashingMachine", 800, 5, false},
    {"Lights", 200, 9, false},
};

const int NUM_DEVICES = sizeof(appliances) / sizeof(appliances[0]);

void solve_energy_optimization_dp(int max_power_limit_w, float price_yuan_per_kwh)
{
    std::vector<int> adjusted_utility(NUM_DEVICES, 0);

    for (int i = 0; i < NUM_DEVICES; i++) {
        int cost_penalty = (int)(appliances[i].power_w * price_yuan_per_kwh);
        int value = appliances[i].comfort_score * 1000 - cost_penalty;
        adjusted_utility[i] = value > 0 ? value : 0;
    }

    std::vector<int> dp(max_power_limit_w + 1, 0);
    std::vector<std::vector<bool>> choice(
        NUM_DEVICES,
        std::vector<bool>(max_power_limit_w + 1, false));

    for (int i = 0; i < NUM_DEVICES; i++) {
        int weight = appliances[i].power_w;
        int value = adjusted_utility[i];

        for (int capacity = max_power_limit_w; capacity >= weight; capacity--) {
            int candidate = dp[capacity - weight] + value;
            if (candidate > dp[capacity]) {
                dp[capacity] = candidate;
                choice[i][capacity] = true;
            }
        }
    }

    int capacity = max_power_limit_w;
    for (int i = NUM_DEVICES - 1; i >= 0; i--) {
        if (capacity >= 0 && choice[i][capacity]) {
            appliances[i].enabled = true;
            capacity -= appliances[i].power_w;
        } else {
            appliances[i].enabled = false;
        }
    }
}

#include <Arduino.h>

// Lightweight embedded-side reference policy.
// It enumerates all on/off combinations for a small set of controllable loads.
// The PC-side project in code/pc_milp_solver contains the full MILP demo.
struct Device {
    String name;
    int power_w;       // Rated power in watts.
    int comfort_score; // User comfort or priority score.
    bool enabled;      // false: off, true: on.
};

Device appliances[] = {
    {"AirConditioner", 1500, 10, false},
    {"WaterHeater", 2000, 8, false},
    {"WashingMachine", 800, 5, false},
    {"Lights", 200, 9, false},
};

const int NUM_DEVICES = sizeof(appliances) / sizeof(appliances[0]);

static void solve_energy_optimization(int max_power_limit_w, float price_yuan_per_kwh)
{
    float best_score = -1000000.0f;
    int best_mask = 0;
    int total_combinations = 1 << NUM_DEVICES;

    for (int mask = 0; mask < total_combinations; mask++) {
        int power_w = 0;
        int comfort = 0;

        for (int i = 0; i < NUM_DEVICES; i++) {
            if (((mask >> i) & 1) != 0) {
                power_w += appliances[i].power_w;
                comfort += appliances[i].comfort_score;
            }
        }

        // One-hour operating cost penalty. This keeps the simple prototype close
        // to the PC MILP objective: maximize comfort while avoiding high cost.
        float estimated_cost = (power_w / 1000.0f) * price_yuan_per_kwh;
        float score = comfort - estimated_cost;

        if (power_w <= max_power_limit_w && score > best_score) {
            best_score = score;
            best_mask = mask;
        }
    }

    for (int i = 0; i < NUM_DEVICES; i++) {
        appliances[i].enabled = ((best_mask >> i) & 1) != 0;
    }
}

static int parse_int_field(const String &input, const String &key, int fallback)
{
    int start = input.indexOf(key);
    if (start < 0) {
        return fallback;
    }
    start += key.length();
    int end = input.indexOf(',', start);
    if (end < 0) {
        end = input.length();
    }
    return input.substring(start, end).toInt();
}

static float parse_float_field(const String &input, const String &key, float fallback)
{
    int start = input.indexOf(key);
    if (start < 0) {
        return fallback;
    }
    start += key.length();
    int end = input.indexOf(',', start);
    if (end < 0) {
        end = input.length();
    }
    return input.substring(start, end).toFloat();
}

static void print_policy_frame()
{
    Serial.print("S:");
    for (int i = 0; i < NUM_DEVICES; i++) {
        Serial.print(appliances[i].enabled ? 1 : 0);
        if (i + 1 < NUM_DEVICES) {
            Serial.print(',');
        }
    }
    Serial.print('\n');
}

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    if (!Serial.available()) {
        return;
    }

    // Example input from a host:
    // P=1.20,LIMIT=3000
    String input = Serial.readStringUntil('\n');
    float price = parse_float_field(input, "P=", 1.20f);
    int limit = parse_int_field(input, "LIMIT=", 3000);

    solve_energy_optimization(limit, price);
    print_policy_frame();
}

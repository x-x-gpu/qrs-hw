#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "esp_chip_info.h"
#include "esp_check.h"
#include "esp_err.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_rom_sys.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/uart.h"

static const char *TAG = "sensor_check";

#define I2C_PORT I2C_NUM_0
#define BH1750_SDA_GPIO GPIO_NUM_6
#define BH1750_SCL_GPIO GPIO_NUM_7
#define PIR_OUT_GPIO GPIO_NUM_10
#define AM2302_DATA_GPIO GPIO_NUM_11
#define RS485_DE_RE_GPIO GPIO_NUM_18
#define RS485_TX_GPIO GPIO_NUM_20
#define RS485_RX_GPIO GPIO_NUM_21
#define RS485_UART_PORT UART_NUM_1
#define RS485_UART_BAUD_RATE 38400
#define RS485_PHY_DIAG_DIRECTION 0
#define RS485_UART_INVERT_TX_RX 0
#define RS485_RX_LINE_MAX 192
#define RS485_TX_ENABLE_DELAY_US 500
#define RS485_TX_DISABLE_DELAY_US 600
#define RS485_RESPONSE_DELAY_MS 120

#define BH1750_ADDR_LOW 0x23
#define BH1750_ADDR_HIGH 0x5C
#define BH1750_CMD_POWER_ON 0x01
#define BH1750_CMD_RESET 0x07
#define BH1750_CMD_CONT_HIGH_RES 0x10

typedef struct {
    float temperature_c;
    float humidity_percent;
} am2302_reading_t;

static uint8_t s_bh1750_addr;
static char s_node_id[3] = "00";

static void init_node_id(void)
{
    uint8_t mac[6] = {0};
    ESP_ERROR_CHECK(esp_read_mac(mac, ESP_MAC_WIFI_STA));

    if (mac[5] == 0x24) {
        snprintf(s_node_id, sizeof(s_node_id), "01");
    } else if (mac[5] == 0xEC) {
        snprintf(s_node_id, sizeof(s_node_id), "02");
    } else {
        snprintf(s_node_id, sizeof(s_node_id), "%02X", mac[5]);
    }

    ESP_LOGI(TAG,
             "RS485 node ID=%s from Wi-Fi MAC %02X:%02X:%02X:%02X:%02X:%02X",
             s_node_id,
             mac[0],
             mac[1],
             mac[2],
             mac[3],
             mac[4],
             mac[5]);
}

static void log_chip_features(const esp_chip_info_t *chip_info)
{
    ESP_LOGI(TAG, "Target: %s", CONFIG_IDF_TARGET);
    ESP_LOGI(TAG, "CPU cores: %d", chip_info->cores);
    ESP_LOGI(TAG, "Revision: %d", chip_info->revision);
    ESP_LOGI(TAG, "Features:%s%s%s%s",
             (chip_info->features & CHIP_FEATURE_WIFI_BGN) ? " Wi-Fi" : "",
             (chip_info->features & CHIP_FEATURE_BLE) ? " BLE" : "",
             (chip_info->features & CHIP_FEATURE_IEEE802154) ? " 802.15.4" : "",
             (chip_info->features & CHIP_FEATURE_EMB_FLASH) ? " embedded-flash" : "");
}

static esp_err_t init_pir(void)
{
    gpio_config_t pir_config = {
        .pin_bit_mask = 1ULL << PIR_OUT_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    return gpio_config(&pir_config);
}

static esp_err_t init_am2302_gpio(void)
{
    gpio_config_t dht_config = {
        .pin_bit_mask = 1ULL << AM2302_DATA_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    return gpio_config(&dht_config);
}

static bool am2302_wait_for_level(int level, int timeout_us)
{
    int64_t start_us = esp_timer_get_time();

    while (gpio_get_level(AM2302_DATA_GPIO) != level) {
        if ((esp_timer_get_time() - start_us) > timeout_us) {
            return false;
        }
    }

    return true;
}

static esp_err_t read_am2302(am2302_reading_t *reading)
{
    uint8_t data[5] = {0};

    gpio_set_direction(AM2302_DATA_GPIO, GPIO_MODE_OUTPUT_OD);
    gpio_set_pull_mode(AM2302_DATA_GPIO, GPIO_PULLUP_ONLY);
    gpio_set_level(AM2302_DATA_GPIO, 0);
    esp_rom_delay_us(1200);
    gpio_set_level(AM2302_DATA_GPIO, 1);
    esp_rom_delay_us(35);
    gpio_set_direction(AM2302_DATA_GPIO, GPIO_MODE_INPUT);

    if (!am2302_wait_for_level(0, 120) ||
        !am2302_wait_for_level(1, 120) ||
        !am2302_wait_for_level(0, 120)) {
        return ESP_ERR_TIMEOUT;
    }

    for (int bit = 0; bit < 40; bit++) {
        if (!am2302_wait_for_level(1, 80)) {
            return ESP_ERR_TIMEOUT;
        }

        int64_t high_start_us = esp_timer_get_time();
        if (!am2302_wait_for_level(0, 120)) {
            return ESP_ERR_TIMEOUT;
        }
        int64_t high_width_us = esp_timer_get_time() - high_start_us;

        data[bit / 8] <<= 1;
        if (high_width_us > 50) {
            data[bit / 8] |= 1;
        }
    }

    uint8_t checksum = (uint8_t)(data[0] + data[1] + data[2] + data[3]);
    if (checksum != data[4]) {
        return ESP_ERR_INVALID_CRC;
    }

    uint16_t raw_humidity = ((uint16_t)data[0] << 8) | data[1];
    uint16_t raw_temperature = (((uint16_t)data[2] & 0x7F) << 8) | data[3];

    reading->humidity_percent = raw_humidity / 10.0f;
    reading->temperature_c = raw_temperature / 10.0f;
    if (data[2] & 0x80) {
        reading->temperature_c = -reading->temperature_c;
    }

    return ESP_OK;
}

static esp_err_t bh1750_write_cmd(uint8_t addr, uint8_t cmd)
{
    return i2c_master_write_to_device(I2C_PORT, addr, &cmd, 1, pdMS_TO_TICKS(100));
}

static esp_err_t init_bh1750(void)
{
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = BH1750_SDA_GPIO,
        .scl_io_num = BH1750_SCL_GPIO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
        .clk_flags = 0,
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &i2c_config));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0));

    const uint8_t candidates[] = {BH1750_ADDR_LOW, BH1750_ADDR_HIGH};
    for (size_t i = 0; i < sizeof(candidates); i++) {
        uint8_t addr = candidates[i];
        if (bh1750_write_cmd(addr, BH1750_CMD_POWER_ON) == ESP_OK) {
            (void)bh1750_write_cmd(addr, BH1750_CMD_RESET);
            ESP_ERROR_CHECK(bh1750_write_cmd(addr, BH1750_CMD_CONT_HIGH_RES));
            s_bh1750_addr = addr;
            ESP_LOGI(TAG, "BH1750 found at I2C address 0x%02X", s_bh1750_addr);
            return ESP_OK;
        }
    }

    return ESP_ERR_NOT_FOUND;
}

static esp_err_t init_rs485_idle_check(void)
{
    gpio_config_t direction_config = {
        .pin_bit_mask = 1ULL << RS485_DE_RE_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    ESP_RETURN_ON_ERROR(gpio_config(&direction_config), TAG, "RS485 DE/RE GPIO config failed");

    // Receive mode for common MAX3485/SP3485 boards with DE and /RE tied together.
    gpio_set_level(RS485_DE_RE_GPIO, 0);

    uart_config_t uart_config = {
        .baud_rate = RS485_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 0,
        .source_clk = UART_SCLK_DEFAULT,
    };

    ESP_RETURN_ON_ERROR(uart_param_config(RS485_UART_PORT, &uart_config), TAG, "RS485 UART config failed");
    ESP_RETURN_ON_ERROR(uart_set_pin(RS485_UART_PORT,
                                     RS485_TX_GPIO,
                                     RS485_RX_GPIO,
                                     UART_PIN_NO_CHANGE,
                                     UART_PIN_NO_CHANGE),
                        TAG,
                        "RS485 UART pin config failed");
    ESP_RETURN_ON_ERROR(uart_driver_install(RS485_UART_PORT, 256, 0, 0, NULL, 0),
                        TAG,
                        "RS485 UART driver install failed");

#if RS485_UART_INVERT_TX_RX
    ESP_RETURN_ON_ERROR(uart_set_line_inverse(RS485_UART_PORT, UART_SIGNAL_TXD_INV | UART_SIGNAL_RXD_INV),
                        TAG,
                        "RS485 UART inversion config failed");
    ESP_LOGW(TAG, "RS485 UART TX/RX inversion is enabled for A/B polarity diagnosis.");
#endif

    int rx_idle_level = gpio_get_level(RS485_RX_GPIO);
    ESP_LOGI(TAG,
             "RS485 UART ready: TX=GPIO%d RX=GPIO%d DE,/RE=GPIO%d baud=%d RX_idle=%d",
             RS485_TX_GPIO,
             RS485_RX_GPIO,
             RS485_DE_RE_GPIO,
             RS485_UART_BAUD_RATE,
             rx_idle_level);

    if (rx_idle_level == 0) {
        ESP_LOGW(TAG,
                 "RS485 RX idle is low. If RO is connected, check VCC/GND, A/B biasing, and whether the bus is held dominant.");
    }

    return ESP_OK;
}

static void rs485_send_line(const char *line)
{
    gpio_set_level(RS485_DE_RE_GPIO, 1);
    esp_rom_delay_us(RS485_TX_ENABLE_DELAY_US);
    uart_write_bytes(RS485_UART_PORT, line, strlen(line));
    ESP_ERROR_CHECK(uart_wait_tx_done(RS485_UART_PORT, pdMS_TO_TICKS(100)));
    esp_rom_delay_us(RS485_TX_DISABLE_DELAY_US);
    gpio_set_level(RS485_DE_RE_GPIO, 0);
}

static bool rs485_query_is_for_this_node(const uint8_t *data, int length)
{
    for (int i = 0; i + 1 < length; i++) {
        if (data[i] != '@' || data[i + 1] != 'Q') {
            continue;
        }

        for (int j = i + 2; j + 3 < length; j++) {
            if (data[j] != 'I' || data[j + 1] != 'D' || data[j + 2] != '=') {
                continue;
            }

            const uint8_t *id = &data[j + 3];
            if (id[0] == '*' || (j + 5 < length && id[0] == 'A' && id[1] == 'L' && id[2] == 'L')) {
                return true;
            }

            if (j + 4 < length && id[0] == (uint8_t)s_node_id[0] && id[1] == (uint8_t)s_node_id[1]) {
                return true;
            }

            return false;
        }
    }

    return false;
}

static void handle_rs485_line(const uint8_t *line,
                              int line_len,
                              float lux,
                              esp_err_t bh1750_status,
                              const am2302_reading_t *am2302,
                              esp_err_t am2302_status,
                              int pir_level)
{
    if (line_len < 2) {
        return;
    }

    if (line[0] == '@' && line[1] == 'Q') {
        ESP_LOGI(TAG, "RS485 RX line: %.*s", line_len, (const char *)line);
    } else if (line[0] == '@' && line[1] == 'A') {
        ESP_LOGI(TAG, "RS485 ACK line: %.*s", line_len, (const char *)line);
        return;
    } else if (line[0] == '@' && line[1] == 'D') {
        ESP_LOGI(TAG, "RS485 DATA sniff: %.*s", line_len, (const char *)line);
        return;
    } else {
        ESP_LOGD(TAG, "RS485 ignored line: %.*s", line_len, (const char *)line);
        return;
    }

    if (rs485_query_is_for_this_node(line, line_len)) {
        char tx_line[160];
        snprintf(tx_line,
                 sizeof(tx_line),
                 "@D,ID=%s,PIR=%d,LUX=%.1f,T=%.1f,H=%.1f,BH=%s,DHT=%s\r\n",
                 s_node_id,
                 pir_level,
                 lux,
                 am2302->temperature_c,
                 am2302->humidity_percent,
                 esp_err_to_name(bh1750_status),
                 esp_err_to_name(am2302_status));
        ESP_LOGI(TAG, "RS485 TX: %s", tx_line);
        vTaskDelay(pdMS_TO_TICKS(RS485_RESPONSE_DELAY_MS));
        rs485_send_line(tx_line);
    }
}

static void poll_rs485_protocol(float lux,
                                esp_err_t bh1750_status,
                                const am2302_reading_t *am2302,
                                esp_err_t am2302_status,
                                int pir_level)
{
    static uint8_t line[RS485_RX_LINE_MAX];
    static int line_len = 0;
    uint8_t rx_buffer[64];
    int read_len = uart_read_bytes(RS485_UART_PORT, rx_buffer, sizeof(rx_buffer), pdMS_TO_TICKS(2));
    if (read_len <= 0) {
        return;
    }

    for (int i = 0; i < read_len; i++) {
        uint8_t ch = rx_buffer[i];

        if (line_len == 0) {
            if (ch != '@') {
                continue;
            }
        } else if (ch == '@') {
            line_len = 0;
        }

        if (ch == '\0' || ch == '\r') {
            continue;
        }

        if (ch == '\n') {
            handle_rs485_line(line,
                              line_len,
                              lux,
                              bh1750_status,
                              am2302,
                              am2302_status,
                              pir_level);
            line_len = 0;
            continue;
        }

        if (line_len < (int)sizeof(line)) {
            line[line_len++] = ch;
        } else {
            ESP_LOGW(TAG, "RS485 line overflow; discarding partial frame");
            line_len = 0;
        }
    }
}

static void send_rs485_diagnostic_query(int64_t now_us)
{
    static int64_t last_diag_us = 0;
    const char *query = NULL;
    int64_t first_query_us = 0;
    int64_t period_us = 0;

#if RS485_PHY_DIAG_DIRECTION == 1
    if (strcmp(s_node_id, "02") == 0) {
        query = "@Q,ID=01*00\r\n";
        first_query_us = 1000000;
        period_us = 3000000;
    } else {
        return;
    }
#elif RS485_PHY_DIAG_DIRECTION == 2
    if (strcmp(s_node_id, "01") == 0) {
        query = "@Q,ID=02*00\r\n";
        first_query_us = 1000000;
        period_us = 3000000;
    } else {
        return;
    }
#else
    return;
#endif

    if (now_us < first_query_us) {
        return;
    }

    if (last_diag_us != 0 && (now_us - last_diag_us) < period_us) {
        return;
    }
    last_diag_us = now_us;

    ESP_LOGI(TAG, "RS485 DIAG TX: %s", query);
    rs485_send_line(query);
}

static esp_err_t read_bh1750_lux(float *lux)
{
    if (s_bh1750_addr == 0) {
        return ESP_ERR_INVALID_STATE;
    }

    uint8_t raw_data[2] = {0};
    esp_err_t err = i2c_master_read_from_device(I2C_PORT, s_bh1750_addr, raw_data, sizeof(raw_data), pdMS_TO_TICKS(100));
    if (err != ESP_OK) {
        return err;
    }

    uint16_t raw_value = ((uint16_t)raw_data[0] << 8) | raw_data[1];
    *lux = raw_value / 1.2f;
    return ESP_OK;
}

void app_main(void)
{
    esp_chip_info_t chip_info;
    uint32_t flash_size = 0;

    esp_chip_info(&chip_info);
    ESP_ERROR_CHECK(esp_flash_get_size(NULL, &flash_size));

    ESP_LOGI(TAG, "ESP32-C6 single-board sensor check is running.");
    init_node_id();
    log_chip_features(&chip_info);
    ESP_LOGI(TAG, "Detected flash size: %" PRIu32 " MB", flash_size / (1024U * 1024U));
    ESP_LOGI(TAG, "Minimum free heap: %" PRIu32 " bytes", esp_get_minimum_free_heap_size());
    ESP_LOGI(TAG,
             "Expected wiring: BH1750 SDA=GPIO6 SCL=GPIO7, AM2302 DATA=GPIO11, PIR OUT=GPIO10, RS485 TX=GPIO20 RX=GPIO21 DE,/RE=GPIO18");

    ESP_ERROR_CHECK(init_pir());
    ESP_ERROR_CHECK(init_am2302_gpio());
    esp_err_t bh1750_status = init_bh1750();
    if (bh1750_status != ESP_OK) {
        ESP_LOGW(TAG, "BH1750 init failed: %s. Check GY-302 VCC/GND/SDA/SCL and address pin.",
                 esp_err_to_name(bh1750_status));
    }

    esp_err_t rs485_status = init_rs485_idle_check();
    if (rs485_status != ESP_OK) {
        ESP_LOGW(TAG, "RS485 init failed: %s. Check MAX3485/SP3485 VCC/GND/DI/RO/DE-/RE wiring.",
                 esp_err_to_name(rs485_status));
    }

    vTaskDelay(pdMS_TO_TICKS(200));

    uint32_t sample = 0;
    int pir_level = 0;
    float lux = -1.0f;
    esp_err_t bh1750_read_status = ESP_ERR_INVALID_STATE;
    am2302_reading_t am2302 = {
        .temperature_c = -999.0f,
        .humidity_percent = -999.0f,
    };
    esp_err_t am2302_status = ESP_ERR_INVALID_STATE;
    int64_t last_bh1750_sample_us = 0;
    int64_t last_am2302_sample_us = 0;

    while (true) {
        int64_t now_us = esp_timer_get_time();
        pir_level = gpio_get_level(PIR_OUT_GPIO);

        if (last_bh1750_sample_us == 0 || (now_us - last_bh1750_sample_us) >= 200000) {
            bh1750_read_status = read_bh1750_lux(&lux);
            last_bh1750_sample_us = now_us;
        }

        if (last_am2302_sample_us == 0 || (now_us - last_am2302_sample_us) >= 2000000) {
            am2302_status = read_am2302(&am2302);
            last_am2302_sample_us = now_us;

            ESP_LOGI(TAG,
                     "sample=%" PRIu32 " PIR=%d BH1750=%s lux=%.1f AM2302=%s temp=%.1fC hum=%.1f%%",
                     sample++,
                     pir_level,
                     esp_err_to_name(bh1750_read_status),
                     lux,
                     esp_err_to_name(am2302_status),
                     am2302.temperature_c,
                     am2302.humidity_percent);
        }

        if (rs485_status == ESP_OK) {
            poll_rs485_protocol(lux, bh1750_read_status, &am2302, am2302_status, pir_level);
            send_rs485_diagnostic_query(now_us);
        }

        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

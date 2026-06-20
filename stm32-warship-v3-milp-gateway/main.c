#include <stdbool.h>
#include <stdint.h>

#define REG32(addr) (*(volatile uint32_t *)(addr))

#define RCC_BASE 0x40021000UL
#define FSMC_BASE 0xA0000000UL
#define GPIOA_BASE 0x40010800UL
#define GPIOB_BASE 0x40010C00UL
#define GPIOC_BASE 0x40011000UL
#define GPIOD_BASE 0x40011400UL
#define GPIOE_BASE 0x40011800UL
#define GPIOF_BASE 0x40011C00UL
#define GPIOG_BASE 0x40012000UL
#define USART1_BASE 0x40013800UL
#define USART3_BASE 0x40004800UL

#define RCC_AHBENR REG32(RCC_BASE + 0x14)
#define RCC_APB2ENR REG32(RCC_BASE + 0x18)
#define RCC_APB1ENR REG32(RCC_BASE + 0x1C)

#define AFIO_MAPR REG32(0x40010004UL)

#define SYST_CSR REG32(0xE000E010UL)
#define SYST_RVR REG32(0xE000E014UL)
#define SYST_CVR REG32(0xE000E018UL)
#define NVIC_ICER1 REG32(0xE000E184UL)
#define NVIC_ISER1 REG32(0xE000E104UL)

#define GPIOA_BSRR REG32(GPIOA_BASE + 0x10)
#define GPIOA_BRR REG32(GPIOA_BASE + 0x14)
#define GPIOB_BSRR REG32(GPIOB_BASE + 0x10)
#define GPIOB_BRR REG32(GPIOB_BASE + 0x14)
#define GPIOA_IDR REG32(GPIOA_BASE + 0x08)
#define GPIOB_IDR REG32(GPIOB_BASE + 0x08)
#define GPIOC_IDR REG32(GPIOC_BASE + 0x08)
#define GPIOD_BSRR REG32(GPIOD_BASE + 0x10)
#define GPIOD_BRR REG32(GPIOD_BASE + 0x14)
#define GPIOD_IDR REG32(GPIOD_BASE + 0x08)
#define GPIOE_BSRR REG32(GPIOE_BASE + 0x10)
#define GPIOE_IDR REG32(GPIOE_BASE + 0x08)
#define GPIOF_BSRR REG32(GPIOF_BASE + 0x10)
#define GPIOF_BRR REG32(GPIOF_BASE + 0x14)
#define GPIOF_IDR REG32(GPIOF_BASE + 0x08)
#define GPIOG_BSRR REG32(GPIOG_BASE + 0x10)
#define GPIOG_BRR REG32(GPIOG_BASE + 0x14)
#define GPIOG_IDR REG32(GPIOG_BASE + 0x08)

#define FSMC_BCR4 REG32(FSMC_BASE + 0x18)
#define FSMC_BTR4 REG32(FSMC_BASE + 0x1C)
#define FSMC_BWTR4 REG32(FSMC_BASE + 0x11C)

#define USART3_SR REG32(USART3_BASE + 0x00)
#define USART3_DR REG32(USART3_BASE + 0x04)
#define USART3_BRR REG32(USART3_BASE + 0x08)
#define USART3_CR1 REG32(USART3_BASE + 0x0C)
#define USART3_CR3 REG32(USART3_BASE + 0x14)
#define USART1_SR REG32(USART1_BASE + 0x00)
#define USART1_DR REG32(USART1_BASE + 0x04)
#define USART1_BRR REG32(USART1_BASE + 0x08)
#define USART1_CR1 REG32(USART1_BASE + 0x0C)
#define USART1_CR3 REG32(USART1_BASE + 0x14)

#define RCC_AHBENR_FSMCEN (1U << 8)
#define RCC_APB2ENR_AFIOEN (1U << 0)
#define RCC_APB2ENR_IOPAEN (1U << 2)
#define RCC_APB2ENR_IOPBEN (1U << 3)
#define RCC_APB2ENR_IOPCEN (1U << 4)
#define RCC_APB2ENR_IOPDEN (1U << 5)
#define RCC_APB2ENR_IOPEEN (1U << 6)
#define RCC_APB2ENR_IOPFEN (1U << 7)
#define RCC_APB2ENR_IOPGEN (1U << 8)
#define RCC_APB2ENR_USART1EN (1U << 14)
#define RCC_APB1ENR_USART3EN (1U << 18)

#define AFIO_MAPR_PD01_REMAP (1U << 15)

#define USART_SR_RXNE (1U << 5)
#define USART_SR_TC (1U << 6)
#define USART_SR_TXE (1U << 7)
#define USART_SR_ERR_MASK 0x0FU
#define USART_CR1_RE (1U << 2)
#define USART_CR1_TE (1U << 3)
#define USART_CR1_RXNEIE (1U << 5)
#define USART_CR1_UE (1U << 13)
#define USART_CR3_EIE (1U << 0)

#define CPU_HZ 8000000U
#define SYSTICK_1MS_RELOAD ((CPU_HZ / 1000U) - 1U)
#define USART3_BRR_38400 0x00D0U
#define USART1_BRR_115200 0x0045U
#define RSE_PIN 12U
#define RS485_TX_GUARD_CYCLES 4000U
#define RS485_AFTER_TX_SETTLE_CYCLES 4000U
#define RS485_BETWEEN_NODES_CYCLES 4000U
#define RS485_BETWEEN_ROUNDS_CYCLES 4000U
#define RS485_LOST_AFTER_TIMEOUTS 3U
#define RS485_STARTUP_DIAG_FRAMES 0
#define RS485_QUERY_DEBUG_FRAMES 1
#define RS485_SUMMARY_FRAMES 1
#define RS485_GPIO_SCAN_DIAG 0
#define LCD_REFRESH_MS 5000U
#define GPIO_SCAN_PORTS 7U
#define GPIO_SCAN_TOP 4U
#define UART3_RX_RING_SIZE 256U
#define UART1_RX_RING_SIZE 128U
#define UART3_RX_OVERFLOW_DIAG 0x8000U
#define USART1_IRQN 37U
#define USART3_IRQN 39U
#define LED_PIN 5U
#define BUZZER_PIN 8U

#define LCD_WIDTH 480U
#define LCD_HEIGHT 800U
#define LCD_BLACK 0x0000U
#define LCD_WHITE 0xFFFFU
#define LCD_GREEN 0x07E0U
#define LCD_CYAN 0x07FFU
#define LCD_YELLOW 0xFFE0U
#define LCD_RED 0xF800U
#define LCD_PD_DATA_MASK ((1U << 0U) | (1U << 1U) | (1U << 8U) | (1U << 9U) | \
                          (1U << 10U) | (1U << 14U) | (1U << 15U))
#define LCD_PE_DATA_MASK (0x1FFU << 7U)

typedef struct {
    volatile uint16_t reg;
    volatile uint16_t ram;
} LcdBus;

#define LCD_BUS ((LcdBus *)(0x6C000000UL | 0x000007FEUL))

typedef struct {
    bool ok;
    bool seen;
    uint32_t samples;
    uint32_t timeouts;
    uint32_t consecutive_timeouts;
    char pir[8];
    char lux[12];
    char temp[12];
    char hum[12];
    char bh[12];
    char dht[12];
    uint32_t window_samples;
    uint32_t window_pir_high;
    int32_t window_lux10_sum;
    int32_t window_temp10_sum;
    int32_t window_hum10_sum;
} SensorNode;

typedef struct {
    int len;
    uint32_t rxne_count;
    uint32_t rx_edges;
    uint32_t rx_low_samples;
    uint32_t sr_errors;
    char top_ports[GPIO_SCAN_TOP];
    uint8_t top_pins[GPIO_SCAN_TOP];
    uint32_t top_edges[GPIO_SCAN_TOP];
    char preview[32];
} RxDiag;

typedef struct {
    bool seen;
    bool led;
    bool buzzer;
    bool fan_proxy;
    bool ac_proxy;
    uint32_t updates;
    uint32_t last_update_ms;
} MilpCommand;

static SensorNode node01;
static SensorNode node02;
static RxDiag rxdiag01;
static RxDiag rxdiag02;
static uint8_t rx_probe_float;
static uint8_t rx_probe_pulldown;
static uint8_t rx_probe_pullup;
static uint16_t lcd_id_00;
static uint16_t lcd_id_04_hi;
static uint16_t lcd_id_04_lo;
static uint16_t lcd_id_d3;
static uint16_t lcd_raw_04[4];
static uint16_t lcd_raw_d3[5];
static uint16_t lcd_raw_bf[6];
static uint16_t lcd_raw_d4[4];
static uint16_t lcd_raw_da;
static uint16_t lcd_raw_db;
static uint16_t lcd_raw_dc;
static uint16_t lcd_raw_a1[4];
static bool lcd_is_nt35510;
static bool lcd_data_bus_output;
static volatile uint32_t system_ms;
static volatile uint8_t uart3_rx_ring[UART3_RX_RING_SIZE];
static volatile uint16_t uart3_rx_head;
static volatile uint16_t uart3_rx_tail;
static volatile uint32_t uart3_rx_total;
static volatile uint32_t uart3_rx_overflows;
static volatile uint32_t uart3_rx_errors;
static volatile uint8_t uart1_rx_ring[UART1_RX_RING_SIZE];
static volatile uint16_t uart1_rx_head;
static volatile uint16_t uart1_rx_tail;
static volatile uint32_t uart1_rx_total;
static volatile uint32_t uart1_rx_overflows;
static MilpCommand pc_cmd;

void SysTick_Handler(void)
{
    system_ms++;
}

void USART3_IRQHandler(void)
{
    uint32_t sr = USART3_SR;

    if ((sr & (USART_SR_RXNE | USART_SR_ERR_MASK)) != 0U) {
        uint8_t ch = (uint8_t)USART3_DR;

        if ((sr & USART_SR_ERR_MASK) != 0U) {
            uart3_rx_errors |= sr & USART_SR_ERR_MASK;
        }
        if ((sr & USART_SR_RXNE) != 0U && (sr & USART_SR_ERR_MASK) == 0U) {
            uint16_t next = (uint16_t)((uart3_rx_head + 1U) & (UART3_RX_RING_SIZE - 1U));

            uart3_rx_total++;
            if (next == uart3_rx_tail) {
                uart3_rx_overflows++;
            } else {
                uart3_rx_ring[uart3_rx_head] = ch;
                uart3_rx_head = next;
            }
        }
    }
}

void USART1_IRQHandler(void)
{
    uint32_t sr = USART1_SR;

    if ((sr & (USART_SR_RXNE | USART_SR_ERR_MASK)) != 0U) {
        uint8_t ch = (uint8_t)USART1_DR;

        if ((sr & USART_SR_ERR_MASK) != 0U) {
            (void)ch;
        }
        if ((sr & USART_SR_RXNE) != 0U && (sr & USART_SR_ERR_MASK) == 0U) {
            uint16_t next = (uint16_t)((uart1_rx_head + 1U) & (UART1_RX_RING_SIZE - 1U));

            uart1_rx_total++;
            if (next == uart1_rx_tail) {
                uart1_rx_overflows++;
            } else {
                uart1_rx_ring[uart1_rx_head] = ch;
                uart1_rx_head = next;
            }
        }
    }
}

static void systick_init(void)
{
    SYST_RVR = SYSTICK_1MS_RELOAD;
    SYST_CVR = 0U;
    SYST_CSR = 0x7U;
}

static uint32_t millis(void)
{
    return system_ms;
}

static void delay_cycles(volatile uint32_t cycles)
{
    while (cycles--) {
        __asm__ volatile("nop");
    }
}

static void gpio_config_pin(uint32_t port_base, uint32_t pin, uint32_t config)
{
    volatile uint32_t *cr = (volatile uint32_t *)(port_base + ((pin < 8U) ? 0x00U : 0x04U));
    uint32_t shift = (pin & 7U) * 4U;

    *cr &= ~(0xFU << shift);
    *cr |= (config & 0xFU) << shift;
}

static void lcd_data_pins_output(void)
{
    static const uint8_t port_d_pins[] = {0U, 1U, 8U, 9U, 10U, 14U, 15U};
    static const uint8_t port_e_pins[] = {7U, 8U, 9U, 10U, 11U, 12U, 13U, 14U, 15U};

    if (lcd_data_bus_output) {
        return;
    }

    for (uint32_t i = 0; i < sizeof(port_d_pins); i++) {
        gpio_config_pin(GPIOD_BASE, port_d_pins[i], 0x3U);
    }
    for (uint32_t i = 0; i < sizeof(port_e_pins); i++) {
        gpio_config_pin(GPIOE_BASE, port_e_pins[i], 0x3U);
    }
    lcd_data_bus_output = true;
}

static void lcd_data_pins_input(void)
{
    static const uint8_t port_d_pins[] = {0U, 1U, 8U, 9U, 10U, 14U, 15U};
    static const uint8_t port_e_pins[] = {7U, 8U, 9U, 10U, 11U, 12U, 13U, 14U, 15U};

    if (!lcd_data_bus_output) {
        return;
    }

    for (uint32_t i = 0; i < sizeof(port_d_pins); i++) {
        gpio_config_pin(GPIOD_BASE, port_d_pins[i], 0x4U);
    }
    for (uint32_t i = 0; i < sizeof(port_e_pins); i++) {
        gpio_config_pin(GPIOE_BASE, port_e_pins[i], 0x4U);
    }
    lcd_data_bus_output = false;
}

static void lcd_gpio_write_bus(uint16_t value)
{
    uint32_t pd_set = 0U;
    uint32_t pe_set = ((uint32_t)((value >> 4U) & 0x1FFU)) << 7U;

    if ((value & (1U << 0U)) != 0U) {
        pd_set |= 1U << 14U;
    }
    if ((value & (1U << 1U)) != 0U) {
        pd_set |= 1U << 15U;
    }
    if ((value & (1U << 2U)) != 0U) {
        pd_set |= 1U << 0U;
    }
    if ((value & (1U << 3U)) != 0U) {
        pd_set |= 1U << 1U;
    }
    if ((value & (1U << 13U)) != 0U) {
        pd_set |= 1U << 8U;
    }
    if ((value & (1U << 14U)) != 0U) {
        pd_set |= 1U << 9U;
    }
    if ((value & (1U << 15U)) != 0U) {
        pd_set |= 1U << 10U;
    }

    GPIOD_BSRR = pd_set | ((LCD_PD_DATA_MASK & ~pd_set) << 16U);
    GPIOE_BSRR = pe_set | ((LCD_PE_DATA_MASK & ~pe_set) << 16U);
}

static uint16_t lcd_gpio_read_bus(void)
{
    uint32_t pd = GPIOD_IDR;
    uint32_t pe = GPIOE_IDR;
    uint16_t value = 0U;

    if ((pd & (1U << 14U)) != 0U) {
        value |= 1U << 0U;
    }
    if ((pd & (1U << 15U)) != 0U) {
        value |= 1U << 1U;
    }
    if ((pd & (1U << 0U)) != 0U) {
        value |= 1U << 2U;
    }
    if ((pd & (1U << 1U)) != 0U) {
        value |= 1U << 3U;
    }
    value |= (uint16_t)(((pe >> 7U) & 0x1FFU) << 4U);
    if ((pd & (1U << 8U)) != 0U) {
        value |= 1U << 13U;
    }
    if ((pd & (1U << 9U)) != 0U) {
        value |= 1U << 14U;
    }
    if ((pd & (1U << 10U)) != 0U) {
        value |= 1U << 15U;
    }

    return value;
}

static void lcd_write16(bool data, uint16_t value)
{
    lcd_data_pins_output();
    if (data) {
        GPIOG_BSRR = 1U << 0U;
    } else {
        GPIOG_BRR = 1U << 0U;
    }

    lcd_gpio_write_bus(value);
    GPIOG_BRR = 1U << 12U;
    GPIOD_BRR = 1U << 5U;
    delay_cycles(4U);
    GPIOD_BSRR = 1U << 5U;
    GPIOG_BSRR = 1U << 12U;
}

static void lcd_write_data_repeat(uint16_t data, uint32_t count)
{
    lcd_data_pins_output();
    GPIOG_BSRR = 1U << 0U;
    lcd_gpio_write_bus(data);

    while (count-- > 0U) {
        GPIOG_BRR = 1U << 12U;
        GPIOD_BRR = 1U << 5U;
        delay_cycles(4U);
        GPIOD_BSRR = 1U << 5U;
        GPIOG_BSRR = 1U << 12U;
    }
}

static void lcd_write_cmd(uint16_t cmd)
{
    lcd_write16(false, cmd);
}

static void lcd_write_data(uint16_t data)
{
    lcd_write16(true, data);
}

static uint16_t lcd_read_data(void)
{
    uint16_t value;

    lcd_data_pins_input();
    GPIOG_BSRR = 1U << 0U;
    GPIOG_BRR = 1U << 12U;
    GPIOD_BRR = 1U << 4U;
    delay_cycles(8U);
    value = lcd_gpio_read_bus();
    GPIOD_BSRR = 1U << 4U;
    GPIOG_BSRR = 1U << 12U;
    lcd_data_pins_output();
    return value;
}

static void lcd_write_cmd_data(uint16_t cmd, uint16_t data)
{
    lcd_write_cmd(cmd);
    lcd_write_data(data);
}

static void lcd_write_reg_series(uint16_t start_reg, const uint8_t *values, uint16_t count)
{
    for (uint16_t i = 0; i < count; i++) {
        lcd_write_cmd_data((uint16_t)(start_reg + i), values[i]);
    }
}

static void lcd_probe_ids(void)
{
    uint16_t b1;
    uint16_t b2;
    uint16_t b3;
    uint16_t b4;

    lcd_write_cmd(0x00U);
    delay_cycles(1000U);
    lcd_id_00 = lcd_read_data();

    lcd_write_cmd(0x04U);
    delay_cycles(1000U);
    lcd_raw_04[0] = lcd_read_data();
    lcd_raw_04[1] = lcd_read_data();
    lcd_raw_04[2] = lcd_read_data();
    lcd_raw_04[3] = lcd_read_data();
    b1 = lcd_raw_04[1];
    b2 = lcd_raw_04[2];
    b3 = lcd_raw_04[3];
    lcd_id_04_hi = (uint16_t)(((b1 & 0xFFU) << 8) | (b2 & 0xFFU));
    lcd_id_04_lo = b3 & 0xFFU;

    lcd_write_cmd(0xD3U);
    delay_cycles(1000U);
    lcd_raw_d3[0] = lcd_read_data();
    lcd_raw_d3[1] = lcd_read_data();
    lcd_raw_d3[2] = lcd_read_data();
    lcd_raw_d3[3] = lcd_read_data();
    lcd_raw_d3[4] = lcd_read_data();
    b1 = lcd_raw_d3[1];
    b2 = lcd_raw_d3[2];
    b3 = lcd_raw_d3[3];
    b4 = lcd_raw_d3[4];
    (void)b1;
    (void)b2;
    lcd_id_d3 = (uint16_t)(((b3 & 0xFFU) << 8) | (b4 & 0xFFU));

    lcd_write_cmd(0xBFU);
    delay_cycles(1000U);
    for (uint32_t i = 0; i < 6U; i++) {
        lcd_raw_bf[i] = lcd_read_data();
    }

    lcd_write_cmd(0xD4U);
    delay_cycles(1000U);
    for (uint32_t i = 0; i < 4U; i++) {
        lcd_raw_d4[i] = lcd_read_data();
    }

    lcd_write_cmd(0xDA00U);
    delay_cycles(1000U);
    lcd_raw_da = lcd_read_data();
    lcd_write_cmd(0xDB00U);
    delay_cycles(1000U);
    lcd_raw_db = lcd_read_data();
    lcd_write_cmd(0xDC00U);
    delay_cycles(1000U);
    lcd_raw_dc = lcd_read_data();

    lcd_write_cmd(0xA1U);
    delay_cycles(1000U);
    for (uint32_t i = 0; i < 4U; i++) {
        lcd_raw_a1[i] = lcd_read_data();
    }

    lcd_is_nt35510 = ((lcd_raw_da & 0xFFU) == 0x00U &&
                      (lcd_raw_db & 0xFFU) == 0x80U &&
                      (lcd_raw_dc & 0xFFU) == 0x00U);
}

static void lcd_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    if (lcd_is_nt35510) {
        lcd_write_cmd_data(0x2A00U, x0 >> 8);
        lcd_write_cmd_data(0x2A01U, x0 & 0xFFU);
        lcd_write_cmd_data(0x2A02U, x1 >> 8);
        lcd_write_cmd_data(0x2A03U, x1 & 0xFFU);
        lcd_write_cmd_data(0x2B00U, y0 >> 8);
        lcd_write_cmd_data(0x2B01U, y0 & 0xFFU);
        lcd_write_cmd_data(0x2B02U, y1 >> 8);
        lcd_write_cmd_data(0x2B03U, y1 & 0xFFU);
        lcd_write_cmd(0x2C00U);
        return;
    }

    lcd_write_cmd(0x2AU);
    lcd_write_data(x0 >> 8);
    lcd_write_data(x0 & 0xFFU);
    lcd_write_data(x1 >> 8);
    lcd_write_data(x1 & 0xFFU);

    lcd_write_cmd(0x2BU);
    lcd_write_data(y0 >> 8);
    lcd_write_data(y0 & 0xFFU);
    lcd_write_data(y1 >> 8);
    lcd_write_data(y1 & 0xFFU);

    lcd_write_cmd(0x2CU);
}

static void lcd_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT || w == 0U || h == 0U) {
        return;
    }

    if ((uint32_t)x + w > LCD_WIDTH) {
        w = (uint16_t)(LCD_WIDTH - x);
    }
    if ((uint32_t)y + h > LCD_HEIGHT) {
        h = (uint16_t)(LCD_HEIGHT - y);
    }

    lcd_set_window(x, y, (uint16_t)(x + w - 1U), (uint16_t)(y + h - 1U));
    lcd_write_data_repeat(color, (uint32_t)w * h);
}

static void font_set(uint8_t columns[5],
                     uint8_t c0,
                     uint8_t c1,
                     uint8_t c2,
                     uint8_t c3,
                     uint8_t c4)
{
    columns[0] = c0;
    columns[1] = c1;
    columns[2] = c2;
    columns[3] = c3;
    columns[4] = c4;
}

static void font_get(char ch, uint8_t columns[5])
{
    if (ch >= 'a' && ch <= 'z') {
        ch = (char)(ch - ('a' - 'A'));
    }

    switch (ch) {
    case '0': font_set(columns, 0x3EU, 0x51U, 0x49U, 0x45U, 0x3EU); break;
    case '1': font_set(columns, 0x00U, 0x42U, 0x7FU, 0x40U, 0x00U); break;
    case '2': font_set(columns, 0x42U, 0x61U, 0x51U, 0x49U, 0x46U); break;
    case '3': font_set(columns, 0x21U, 0x41U, 0x45U, 0x4BU, 0x31U); break;
    case '4': font_set(columns, 0x18U, 0x14U, 0x12U, 0x7FU, 0x10U); break;
    case '5': font_set(columns, 0x27U, 0x45U, 0x45U, 0x45U, 0x39U); break;
    case '6': font_set(columns, 0x3CU, 0x4AU, 0x49U, 0x49U, 0x30U); break;
    case '7': font_set(columns, 0x01U, 0x71U, 0x09U, 0x05U, 0x03U); break;
    case '8': font_set(columns, 0x36U, 0x49U, 0x49U, 0x49U, 0x36U); break;
    case '9': font_set(columns, 0x06U, 0x49U, 0x49U, 0x29U, 0x1EU); break;
    case 'A': font_set(columns, 0x7EU, 0x11U, 0x11U, 0x11U, 0x7EU); break;
    case 'B': font_set(columns, 0x7FU, 0x49U, 0x49U, 0x49U, 0x36U); break;
    case 'C': font_set(columns, 0x3EU, 0x41U, 0x41U, 0x41U, 0x22U); break;
    case 'D': font_set(columns, 0x7FU, 0x41U, 0x41U, 0x22U, 0x1CU); break;
    case 'E': font_set(columns, 0x7FU, 0x49U, 0x49U, 0x49U, 0x41U); break;
    case 'F': font_set(columns, 0x7FU, 0x09U, 0x09U, 0x09U, 0x01U); break;
    case 'G': font_set(columns, 0x3EU, 0x41U, 0x49U, 0x49U, 0x7AU); break;
    case 'H': font_set(columns, 0x7FU, 0x08U, 0x08U, 0x08U, 0x7FU); break;
    case 'I': font_set(columns, 0x00U, 0x41U, 0x7FU, 0x41U, 0x00U); break;
    case 'J': font_set(columns, 0x20U, 0x40U, 0x41U, 0x3FU, 0x01U); break;
    case 'K': font_set(columns, 0x7FU, 0x08U, 0x14U, 0x22U, 0x41U); break;
    case 'L': font_set(columns, 0x7FU, 0x40U, 0x40U, 0x40U, 0x40U); break;
    case 'M': font_set(columns, 0x7FU, 0x02U, 0x0CU, 0x02U, 0x7FU); break;
    case 'N': font_set(columns, 0x7FU, 0x04U, 0x08U, 0x10U, 0x7FU); break;
    case 'O': font_set(columns, 0x3EU, 0x41U, 0x41U, 0x41U, 0x3EU); break;
    case 'P': font_set(columns, 0x7FU, 0x09U, 0x09U, 0x09U, 0x06U); break;
    case 'Q': font_set(columns, 0x3EU, 0x41U, 0x51U, 0x21U, 0x5EU); break;
    case 'R': font_set(columns, 0x7FU, 0x09U, 0x19U, 0x29U, 0x46U); break;
    case 'S': font_set(columns, 0x46U, 0x49U, 0x49U, 0x49U, 0x31U); break;
    case 'T': font_set(columns, 0x01U, 0x01U, 0x7FU, 0x01U, 0x01U); break;
    case 'U': font_set(columns, 0x3FU, 0x40U, 0x40U, 0x40U, 0x3FU); break;
    case 'V': font_set(columns, 0x1FU, 0x20U, 0x40U, 0x20U, 0x1FU); break;
    case 'W': font_set(columns, 0x3FU, 0x40U, 0x38U, 0x40U, 0x3FU); break;
    case 'X': font_set(columns, 0x63U, 0x14U, 0x08U, 0x14U, 0x63U); break;
    case 'Y': font_set(columns, 0x07U, 0x08U, 0x70U, 0x08U, 0x07U); break;
    case 'Z': font_set(columns, 0x61U, 0x51U, 0x49U, 0x45U, 0x43U); break;
    case ':': font_set(columns, 0x00U, 0x36U, 0x36U, 0x00U, 0x00U); break;
    case '.': font_set(columns, 0x00U, 0x60U, 0x60U, 0x00U, 0x00U); break;
    case '-': font_set(columns, 0x08U, 0x08U, 0x08U, 0x08U, 0x08U); break;
    case '_': font_set(columns, 0x40U, 0x40U, 0x40U, 0x40U, 0x40U); break;
    case '=': font_set(columns, 0x14U, 0x14U, 0x14U, 0x14U, 0x14U); break;
    case '/': font_set(columns, 0x20U, 0x10U, 0x08U, 0x04U, 0x02U); break;
    case ' ': font_set(columns, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U); break;
    default: font_set(columns, 0x02U, 0x01U, 0x51U, 0x09U, 0x06U); break;
    }
}

static void lcd_draw_char(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t scale)
{
    uint8_t columns[5];
    font_get(ch, columns);

    for (uint16_t col = 0; col < 5U; col++) {
        for (uint16_t row = 0; row < 7U; row++) {
            if ((columns[col] & (1U << row)) != 0U) {
                lcd_fill_rect((uint16_t)(x + col * scale),
                              (uint16_t)(y + row * scale),
                              scale,
                              scale,
                              color);
            }
        }
    }
}

static void lcd_draw_text(uint16_t x, uint16_t y, const char *text, uint16_t color, uint16_t scale)
{
    while (*text != '\0' && x + 5U * scale < LCD_WIDTH) {
        lcd_draw_char(x, y, *text++, color, scale);
        x = (uint16_t)(x + 6U * scale);
    }
}

static void lcd_draw_line(uint16_t y, const char *text, uint16_t color)
{
    lcd_fill_rect(0U, y, LCD_WIDTH, 18U, LCD_BLACK);
    lcd_draw_text(4U, (uint16_t)(y + 2U), text, color, 2U);
}

static void lcd_fsmc_gpio_init(void)
{
    RCC_APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPDEN |
                   RCC_APB2ENR_IOPEEN | RCC_APB2ENR_IOPGEN;
    RCC_AHBENR |= RCC_AHBENR_FSMCEN;
    AFIO_MAPR |= AFIO_MAPR_PD01_REMAP;

    gpio_config_pin(GPIOB_BASE, 0U, 0x3U);
    gpio_config_pin(GPIOD_BASE, 4U, 0x3U);
    gpio_config_pin(GPIOD_BASE, 5U, 0x3U);
    gpio_config_pin(GPIOD_BASE, 7U, 0x3U);
    gpio_config_pin(GPIOG_BASE, 0U, 0x3U);
    gpio_config_pin(GPIOG_BASE, 9U, 0x3U);
    gpio_config_pin(GPIOG_BASE, 10U, 0x3U);
    gpio_config_pin(GPIOG_BASE, 12U, 0x3U);

    lcd_data_pins_output();

    GPIOB_BSRR = 1U << 0U;
    GPIOD_BSRR = (1U << 4U) | (1U << 5U);
    GPIOD_BRR = 1U << 7U;
    GPIOG_BSRR = (1U << 0U) | (1U << 9U) | (1U << 10U) | (1U << 12U);
}

static void lcd_ili9341_init(void)
{
    lcd_write_cmd(0x01U);
    delay_cycles(800000U);

    lcd_write_cmd(0xCFU);
    lcd_write_data(0x00U);
    lcd_write_data(0xC1U);
    lcd_write_data(0x30U);

    lcd_write_cmd(0xEDU);
    lcd_write_data(0x64U);
    lcd_write_data(0x03U);
    lcd_write_data(0x12U);
    lcd_write_data(0x81U);

    lcd_write_cmd(0xE8U);
    lcd_write_data(0x85U);
    lcd_write_data(0x00U);
    lcd_write_data(0x78U);

    lcd_write_cmd(0xCBU);
    lcd_write_data(0x39U);
    lcd_write_data(0x2CU);
    lcd_write_data(0x00U);
    lcd_write_data(0x34U);
    lcd_write_data(0x02U);

    lcd_write_cmd(0xF7U);
    lcd_write_data(0x20U);

    lcd_write_cmd(0xEAU);
    lcd_write_data(0x00U);
    lcd_write_data(0x00U);

    lcd_write_cmd_data(0xC0U, 0x23U);
    lcd_write_cmd_data(0xC1U, 0x10U);

    lcd_write_cmd(0xC5U);
    lcd_write_data(0x3EU);
    lcd_write_data(0x28U);

    lcd_write_cmd_data(0xC7U, 0x86U);
    lcd_write_cmd_data(0x36U, 0x48U);
    lcd_write_cmd_data(0x3AU, 0x55U);

    lcd_write_cmd(0xB1U);
    lcd_write_data(0x00U);
    lcd_write_data(0x18U);

    lcd_write_cmd(0xB6U);
    lcd_write_data(0x08U);
    lcd_write_data(0x82U);
    lcd_write_data(0x27U);

    lcd_write_cmd_data(0xF2U, 0x00U);
    lcd_write_cmd_data(0x26U, 0x01U);

    lcd_write_cmd(0xE0U);
    lcd_write_data(0x0FU);
    lcd_write_data(0x31U);
    lcd_write_data(0x2BU);
    lcd_write_data(0x0CU);
    lcd_write_data(0x0EU);
    lcd_write_data(0x08U);
    lcd_write_data(0x4EU);
    lcd_write_data(0xF1U);
    lcd_write_data(0x37U);
    lcd_write_data(0x07U);
    lcd_write_data(0x10U);
    lcd_write_data(0x03U);
    lcd_write_data(0x0EU);
    lcd_write_data(0x09U);
    lcd_write_data(0x00U);

    lcd_write_cmd(0xE1U);
    lcd_write_data(0x00U);
    lcd_write_data(0x0EU);
    lcd_write_data(0x14U);
    lcd_write_data(0x03U);
    lcd_write_data(0x11U);
    lcd_write_data(0x07U);
    lcd_write_data(0x31U);
    lcd_write_data(0xC1U);
    lcd_write_data(0x48U);
    lcd_write_data(0x08U);
    lcd_write_data(0x0FU);
    lcd_write_data(0x0CU);
    lcd_write_data(0x31U);
    lcd_write_data(0x36U);
    lcd_write_data(0x0FU);

    lcd_write_cmd(0x11U);
    delay_cycles(960000U);
    lcd_write_cmd(0x29U);
}

static void lcd_nt35510_init(void)
{
    static const uint8_t ini01[] = {0x55U, 0xAAU, 0x52U, 0x08U, 0x01U};
    static const uint8_t ini03[] = {0x34U, 0x34U, 0x34U};
    static const uint8_t ini02[] = {0x0DU, 0x0DU, 0x0DU};
    static const uint8_t ini07[] = {0x24U, 0x24U, 0x24U};
    static const uint8_t ini09[] = {0x0FU, 0x0FU, 0x0FU};
    static const uint8_t ini12[] = {0x08U, 0x08U};
    static const uint8_t ini15[] = {0x00U, 0x78U, 0x00U};
    static const uint8_t ini17[] = {0x00U, 0x89U};
    static const uint8_t gamma[] = {
        0x00U, 0x2DU, 0x00U, 0x2EU, 0x00U, 0x32U, 0x00U, 0x44U,
        0x00U, 0x53U, 0x00U, 0x88U, 0x00U, 0xB6U, 0x00U, 0xF3U,
        0x01U, 0x22U, 0x01U, 0x64U, 0x01U, 0x92U, 0x01U, 0xD4U,
        0x02U, 0x07U, 0x02U, 0x08U, 0x02U, 0x34U, 0x02U, 0x5FU,
        0x02U, 0x78U, 0x02U, 0x94U, 0x02U, 0xA6U, 0x02U, 0xBBU,
        0x02U, 0xCAU, 0x02U, 0xDBU, 0x02U, 0xE8U, 0x02U, 0xF9U,
        0x03U, 0x1FU, 0x03U, 0x7FU
    };
    static const uint8_t ini21[] = {0x55U, 0xAAU, 0x52U, 0x08U, 0x00U};
    static const uint8_t ini22[] = {0x08U, 0x05U, 0x02U, 0x05U, 0x02U};
    static const uint8_t ini24[] = {0x00U, 0x00U};
    static const uint8_t ini25[] = {0x01U, 0x05U, 0x05U, 0x05U};
    static const uint8_t ini26[] = {0x00U, 0x00U, 0x00U};
    static const uint8_t ini27[] = {0x03U, 0x00U, 0x00U};
    static const uint8_t ini28[] = {0x01U, 0x84U, 0x07U, 0x31U, 0x00U, 0x01U};
    static const uint8_t ini30[] = {0xAAU, 0x55U, 0x25U, 0x01U};

    lcd_write_reg_series(0xF000U, ini01, sizeof(ini01));
    lcd_write_reg_series(0xB600U, ini03, sizeof(ini03));
    lcd_write_reg_series(0xB000U, ini02, sizeof(ini02));
    lcd_write_reg_series(0xB700U, ini03, sizeof(ini03));
    lcd_write_reg_series(0xB100U, ini02, sizeof(ini02));
    lcd_write_reg_series(0xB800U, ini07, sizeof(ini07));
    lcd_write_reg_series(0xB900U, ini03, sizeof(ini03));
    lcd_write_reg_series(0xB300U, ini09, sizeof(ini09));
    lcd_write_reg_series(0xBA00U, ini07, sizeof(ini07));
    lcd_write_reg_series(0xB500U, ini12, sizeof(ini12));
    lcd_write_reg_series(0xBC00U, ini15, sizeof(ini15));
    lcd_write_reg_series(0xBD00U, ini15, sizeof(ini15));
    lcd_write_reg_series(0xBE00U, ini17, sizeof(ini17));

    lcd_write_reg_series(0xD100U, gamma, sizeof(gamma));
    lcd_write_reg_series(0xD400U, gamma, sizeof(gamma));
    lcd_write_reg_series(0xD200U, gamma, sizeof(gamma));
    lcd_write_reg_series(0xD500U, gamma, sizeof(gamma));
    lcd_write_reg_series(0xD300U, gamma, sizeof(gamma));
    lcd_write_reg_series(0xD600U, gamma, sizeof(gamma));

    lcd_write_reg_series(0xF000U, ini21, sizeof(ini21));
    lcd_write_reg_series(0xB000U, ini22, sizeof(ini22));
    lcd_write_cmd_data(0xB600U, 0x08U);
    lcd_write_cmd_data(0xB500U, 0x50U);
    lcd_write_reg_series(0xB700U, ini24, sizeof(ini24));
    lcd_write_reg_series(0xB800U, ini25, sizeof(ini25));
    lcd_write_reg_series(0xBC00U, ini26, sizeof(ini26));
    lcd_write_reg_series(0xCC00U, ini27, sizeof(ini27));
    lcd_write_reg_series(0xBD00U, ini28, sizeof(ini28));
    lcd_write_reg_series(0xFF00U, ini30, sizeof(ini30));

    lcd_write_cmd_data(0x3500U, 0x00U);
    lcd_write_cmd_data(0x3A00U, 0x55U);
    lcd_write_cmd(0x1100U);
    delay_cycles(960000U);
    lcd_write_cmd(0x2900U);
    delay_cycles(960000U);
}

static void lcd_init(void)
{
    lcd_fsmc_gpio_init();
    delay_cycles(800000U);
    lcd_probe_ids();
    if (lcd_is_nt35510) {
        lcd_nt35510_init();
    } else {
        lcd_ili9341_init();
    }
    GPIOB_BSRR = 1U;
    lcd_fill_rect(0U, 0U, LCD_WIDTH, LCD_HEIGHT, LCD_BLACK);
}

static void rs485_set_tx(bool enabled)
{
    if (enabled) {
        GPIOF_BSRR = 1U << RSE_PIN;
    } else {
        GPIOF_BRR = 1U << RSE_PIN;
    }
}

static void uart3_rx_irq_enable(bool enabled)
{
    if (enabled) {
        NVIC_ISER1 = 1U << (USART3_IRQN - 32U);
    } else {
        NVIC_ICER1 = 1U << (USART3_IRQN - 32U);
    }
}

static void uart2_putc(char ch)
{
    while ((USART3_SR & USART_SR_TXE) == 0) {
    }
    USART3_DR = (uint32_t)(uint8_t)ch;
}

static void rs485_send(const char *text)
{
    uart3_rx_irq_enable(false);
    rs485_set_tx(true);
    delay_cycles(RS485_TX_GUARD_CYCLES);

    while (*text != '\0') {
        uart2_putc(*text++);
    }

    while ((USART3_SR & USART_SR_TC) == 0) {
    }
    delay_cycles(RS485_TX_GUARD_CYCLES);
    rs485_set_tx(false);
    (void)USART3_SR;
    (void)USART3_DR;
    uart3_rx_irq_enable(true);
}

static void rs485_send_forced_direction(const char *text, bool tx_enabled)
{
    uart3_rx_irq_enable(false);
    rs485_set_tx(tx_enabled);
    delay_cycles(RS485_TX_GUARD_CYCLES);

    while (*text != '\0') {
        uart2_putc(*text++);
    }

    while ((USART3_SR & USART_SR_TC) == 0) {
    }
    delay_cycles(RS485_TX_GUARD_CYCLES);
    rs485_set_tx(false);
    (void)USART3_SR;
    (void)USART3_DR;
    uart3_rx_irq_enable(true);
}

static bool contains_data_frame_for_node(const char *text, const char *node_id);
static bool text_starts_with(const char *text, const char *prefix);
static const char *find_field(const char *text, const char *key);
static void lcd_render_status(void);
static char *append_char(char *out, char ch);
static char *append_uint(char *out, uint32_t value);

static int copy_response_line(char *buffer, int capacity, const char *line, int length)
{
    int copy_len = length;

    if (capacity <= 0) {
        return 0;
    }

    if (copy_len > capacity - 1) {
        copy_len = capacity - 1;
    }
    for (int i = 0; i < copy_len; i++) {
        buffer[i] = line[i];
    }
    buffer[copy_len] = '\0';
    return copy_len;
}

static void rxdiag_reset(RxDiag *diag)
{
    diag->len = 0;
    diag->rxne_count = 0U;
    diag->rx_edges = 0U;
    diag->rx_low_samples = 0U;
    diag->sr_errors = 0U;
    for (uint32_t i = 0; i < GPIO_SCAN_TOP; i++) {
        diag->top_ports[i] = '-';
        diag->top_pins[i] = 0U;
        diag->top_edges[i] = 0U;
    }
    diag->preview[0] = '\0';
}

static void rxdiag_set_preview(RxDiag *diag, const char *data, int length)
{
    int out = 0;

    for (int i = 0; i < length && out < (int)sizeof(diag->preview) - 1; i++) {
        uint8_t ch = (uint8_t)data[i];
        diag->preview[out++] = (ch >= 0x20U && ch <= 0x7EU) ? (char)ch : '?';
    }
    diag->preview[out] = '\0';
}

static RxDiag *rxdiag_by_id(const char *node_id)
{
    if (node_id[0] == '0' && node_id[1] == '1') {
        return &rxdiag01;
    }
    if (node_id[0] == '0' && node_id[1] == '2') {
        return &rxdiag02;
    }
    return 0;
}

static void uart3_rx_ring_reset(void)
{
    uart3_rx_head = 0U;
    uart3_rx_tail = 0U;
    uart3_rx_total = 0U;
    uart3_rx_overflows = 0U;
    uart3_rx_errors = 0U;
}

static bool uart3_rx_pop(uint8_t *ch)
{
    uint16_t tail = uart3_rx_tail;

    if (tail == uart3_rx_head) {
        return false;
    }

    *ch = uart3_rx_ring[tail];
    uart3_rx_tail = (uint16_t)((tail + 1U) & (UART3_RX_RING_SIZE - 1U));
    return true;
}

static uint32_t gpio_scan_read_port(uint32_t port)
{
    switch (port) {
    case 0U: return GPIOA_IDR & 0xFFFFU;
    case 1U: return GPIOB_IDR & 0xFFFFU;
    case 2U: return GPIOC_IDR & 0xFFFFU;
    case 3U: return GPIOD_IDR & 0xFFFFU;
    case 4U: return GPIOE_IDR & 0xFFFFU;
    case 5U: return GPIOF_IDR & 0xFFFFU;
    case 6U: return GPIOG_IDR & 0xFFFFU;
    default: return 0U;
    }
}

static void gpio_scan_start(uint32_t last[GPIO_SCAN_PORTS],
                            uint32_t edges[GPIO_SCAN_PORTS][16])
{
    for (uint32_t port = 0; port < GPIO_SCAN_PORTS; port++) {
        last[port] = gpio_scan_read_port(port);
        for (uint32_t pin = 0; pin < 16U; pin++) {
            edges[port][pin] = 0U;
        }
    }
}

static void gpio_scan_sample(uint32_t last[GPIO_SCAN_PORTS],
                             uint32_t edges[GPIO_SCAN_PORTS][16])
{
    for (uint32_t port = 0; port < GPIO_SCAN_PORTS; port++) {
        uint32_t now = gpio_scan_read_port(port);
        uint32_t changed = now ^ last[port];

        if (changed != 0U) {
            for (uint32_t pin = 0; pin < 16U; pin++) {
                if ((changed & (1U << pin)) != 0U) {
                    edges[port][pin]++;
                }
            }
            last[port] = now;
        }
    }
}

static void rxdiag_set_gpio_scan(RxDiag *diag,
                                 const uint32_t edges[GPIO_SCAN_PORTS][16])
{
    static const char port_names[GPIO_SCAN_PORTS] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};

    if (diag == 0) {
        return;
    }

    for (uint32_t rank = 0; rank < GPIO_SCAN_TOP; rank++) {
        diag->top_ports[rank] = '-';
        diag->top_pins[rank] = 0U;
        diag->top_edges[rank] = 0U;
    }

    for (uint32_t port = 0; port < GPIO_SCAN_PORTS; port++) {
        for (uint32_t pin = 0; pin < 16U; pin++) {
            uint32_t count = edges[port][pin];

            if (count == 0U) {
                continue;
            }

            for (int rank = 0; rank < (int)GPIO_SCAN_TOP; rank++) {
                if (count > diag->top_edges[rank]) {
                    for (int move = (int)GPIO_SCAN_TOP - 1; move > rank; move--) {
                        diag->top_ports[move] = diag->top_ports[move - 1];
                        diag->top_pins[move] = diag->top_pins[move - 1];
                        diag->top_edges[move] = diag->top_edges[move - 1];
                    }
                    diag->top_ports[rank] = port_names[port];
                    diag->top_pins[rank] = (uint8_t)pin;
                    diag->top_edges[rank] = count;
                    break;
                }
            }
        }
    }
}

static void uart1_rx_ring_reset(void)
{
    uart1_rx_head = 0U;
    uart1_rx_tail = 0U;
    uart1_rx_total = 0U;
    uart1_rx_overflows = 0U;
}

static void uart1_putc(char ch)
{
    while ((USART1_SR & USART_SR_TXE) == 0U) {
    }
    USART1_DR = (uint32_t)(uint8_t)ch;
}

static void uart1_puts(const char *text)
{
    while (*text != '\0') {
        uart1_putc(*text++);
    }
}

static bool uart1_rx_pop(uint8_t *ch)
{
    uint16_t tail = uart1_rx_tail;

    if (tail == uart1_rx_head) {
        return false;
    }

    *ch = uart1_rx_ring[tail];
    uart1_rx_tail = (uint16_t)((tail + 1U) & (UART1_RX_RING_SIZE - 1U));
    return true;
}

static bool uart1_read_line(char *line, uint32_t capacity)
{
    static char partial[128];
    static uint32_t partial_len;
    uint8_t ch;

    if (capacity == 0U) {
        return false;
    }

    while (uart1_rx_pop(&ch)) {
        if (ch == '\r') {
            continue;
        }
        if (ch == '\n') {
            uint32_t copy_len = partial_len;
            if (copy_len >= capacity) {
                copy_len = capacity - 1U;
            }
            for (uint32_t i = 0U; i < copy_len; i++) {
                line[i] = partial[i];
            }
            line[copy_len] = '\0';
            partial_len = 0U;
            return true;
        }
        if (partial_len + 1U < sizeof(partial)) {
            partial[partial_len++] = (char)ch;
        }
    }

    return false;
}

static bool parse_control_bool(const char *frame, const char *key, bool *value)
{
    const char *field = find_field(frame, key);

    if (field == 0 || value == 0) {
        return false;
    }

    while (*field != '\0' && *field != '=') {
        field++;
    }
    if (*field == '=') {
        field++;
    }
    if (*field == '1') {
        *value = true;
        return true;
    }
    if (*field == '0') {
        *value = false;
        return true;
    }
    return false;
}

static void milp_apply_control(void)
{
    if (pc_cmd.led) {
        GPIOB_BRR = 1U << LED_PIN;
    } else {
        GPIOB_BSRR = 1U << LED_PIN;
    }
    if (pc_cmd.buzzer) {
        GPIOB_BSRR = 1U << BUZZER_PIN;
    } else {
        GPIOB_BRR = 1U << BUZZER_PIN;
    }
}

static void handle_pc_control_frame(const char *frame)
{
    bool led;
    bool buzzer;
    bool fan;
    bool ac;

    if (!text_starts_with(frame, "@C")) {
        uart1_puts("ERR NOT @C\r\n");
        return;
    }
    if (parse_control_bool(frame, "LED=", &led)) {
        pc_cmd.led = led;
    }
    if (parse_control_bool(frame, "BUZZER=", &buzzer)) {
        pc_cmd.buzzer = buzzer;
    }
    if (parse_control_bool(frame, "FAN=", &fan)) {
        pc_cmd.fan_proxy = fan;
    }
    if (parse_control_bool(frame, "AC=", &ac)) {
        pc_cmd.ac_proxy = ac;
    }
    pc_cmd.seen = true;
    pc_cmd.updates++;
    pc_cmd.last_update_ms = millis();
    milp_apply_control();
    lcd_render_status();
    uart1_puts("ACK @C UPD=");
    {
        char count_text[12];
        append_uint(count_text, pc_cmd.updates);
        uart1_puts(count_text);
    }
    uart1_puts("\r\n");
}

static void milp_parse_frame(void)
{
    char line[128];

    while (uart1_read_line(line, sizeof(line))) {
        handle_pc_control_frame(line);
    }
}

static int uart2_collect_response(const char *node_id, char *buffer, int capacity)
{
    char line[192];
    int line_length = 0;
    int last_length = 0;
    uint32_t first_byte_wait_loops = 800000U;
    uint32_t idle_wait_loops = 120000U;
    uint32_t wait_loops = first_byte_wait_loops;
    RxDiag *diag = rxdiag_by_id(node_id);
    uint32_t last_rx = (GPIOB_IDR >> 11U) & 1U;
    uint32_t gpio_last[GPIO_SCAN_PORTS];
    uint32_t gpio_edges[GPIO_SCAN_PORTS][16];
    uint32_t overflow_start = uart3_rx_overflows;

    if (diag != 0) {
        rxdiag_reset(diag);
    }
    if (RS485_GPIO_SCAN_DIAG) {
        gpio_scan_start(gpio_last, gpio_edges);
    }

    if (capacity > 0) {
        buffer[0] = '\0';
    }

    while (wait_loops-- > 0) {
        uint8_t rx_ch;

        if (uart3_rx_pop(&rx_ch)) {
            char ch = (char)rx_ch;
            if (diag != 0) {
                diag->rxne_count++;
            }
            wait_loops = idle_wait_loops;
            if (line_length < (int)sizeof(line) - 1) {
                line[line_length++] = ch;
            }

            if (ch == '\n') {
                line[line_length] = '\0';
                last_length = copy_response_line(buffer, capacity, line, line_length);
                if (diag != 0) {
                    diag->len = last_length;
                    diag->sr_errors |= uart3_rx_errors;
                    if (uart3_rx_overflows != overflow_start) {
                        diag->sr_errors |= UART3_RX_OVERFLOW_DIAG;
                    }
                    rxdiag_set_preview(diag, buffer, last_length);
                    if (RS485_GPIO_SCAN_DIAG) {
                        rxdiag_set_gpio_scan(diag, gpio_edges);
                    }
                }
                if (contains_data_frame_for_node(line, node_id)) {
                    return last_length;
                }
                line_length = 0;
                wait_loops = first_byte_wait_loops;
                continue;
            }

            if (line_length >= (int)sizeof(line) - 1) {
                line[line_length] = '\0';
                last_length = copy_response_line(buffer, capacity, line, line_length);
                if (diag != 0) {
                    diag->len = last_length;
                    diag->sr_errors |= uart3_rx_errors;
                    if (uart3_rx_overflows != overflow_start) {
                        diag->sr_errors |= UART3_RX_OVERFLOW_DIAG;
                    }
                    rxdiag_set_preview(diag, buffer, last_length);
                    if (RS485_GPIO_SCAN_DIAG) {
                        rxdiag_set_gpio_scan(diag, gpio_edges);
                    }
                }
                line_length = 0;
                wait_loops = first_byte_wait_loops;
            }
        } else {
            uint32_t rx = (GPIOB_IDR >> 11U) & 1U;
            if (diag != 0) {
                if (rx == 0U) {
                    diag->rx_low_samples++;
                }
                if (rx != last_rx) {
                    diag->rx_edges++;
                    last_rx = rx;
                }
                diag->sr_errors |= uart3_rx_errors;
                if (uart3_rx_overflows != overflow_start) {
                    diag->sr_errors |= UART3_RX_OVERFLOW_DIAG;
                }
            }
            if (RS485_GPIO_SCAN_DIAG) {
                gpio_scan_sample(gpio_last, gpio_edges);
            }
        }
    }

    if (line_length > 0) {
        line[line_length] = '\0';
        last_length = copy_response_line(buffer, capacity, line, line_length);
        if (diag != 0) {
            diag->len = last_length;
            diag->sr_errors |= uart3_rx_errors;
            if (uart3_rx_overflows != overflow_start) {
                diag->sr_errors |= UART3_RX_OVERFLOW_DIAG;
            }
            rxdiag_set_preview(diag, buffer, last_length);
            if (RS485_GPIO_SCAN_DIAG) {
                rxdiag_set_gpio_scan(diag, gpio_edges);
            }
        }
        return last_length;
    }
    if (diag != 0) {
        diag->len = last_length;
        diag->sr_errors |= uart3_rx_errors;
        if (uart3_rx_overflows != overflow_start) {
            diag->sr_errors |= UART3_RX_OVERFLOW_DIAG;
        }
        rxdiag_set_preview(diag, buffer, last_length);
        if (RS485_GPIO_SCAN_DIAG) {
            rxdiag_set_gpio_scan(diag, gpio_edges);
        }
    }
    return last_length;
}

static void uart2_flush_rx(void)
{
    uart3_rx_irq_enable(false);
    uart3_rx_ring_reset();
    for (uint32_t i = 0; i < 16U; i++) {
        uint32_t sr = USART3_SR;
        if ((sr & (USART_SR_RXNE | USART_SR_ERR_MASK)) == 0U) {
            break;
        }
        (void)USART3_DR;
    }
    uart3_rx_irq_enable(true);
}

static bool contains_data_frame_for_node(const char *text, const char *node_id)
{
    while (text[0] != '\0') {
        if (text[0] == '@' && text[1] == 'D') {
            const char *p = text + 2;

            while (*p != '\0' && *p != '\r' && *p != '\n') {
                if (p[0] == ',' &&
                    p[1] == 'I' &&
                    p[2] == 'D' &&
                    p[3] == '=' &&
                    p[4] == node_id[0] &&
                    p[5] == node_id[1]) {
                    char after_id = p[6];
                    return after_id == ',' || after_id == '*' || after_id == '\0' ||
                           after_id == '\r' || after_id == '\n';
                }
                p++;
            }
        }
        text++;
    }
    return false;
}

static bool text_starts_with(const char *text, const char *prefix)
{
    while (*prefix != '\0') {
        if (*text++ != *prefix++) {
            return false;
        }
    }
    return true;
}

static SensorNode *node_by_id(const char *node_id)
{
    if (node_id[0] == '0' && node_id[1] == '1') {
        return &node01;
    }
    if (node_id[0] == '0' && node_id[1] == '2') {
        return &node02;
    }
    return 0;
}

static const char *find_field(const char *text, const char *key)
{
    while (*text != '\0') {
        if (text[0] == ',' && text_starts_with(text + 1, key)) {
            return text + 1;
        }
        text++;
    }
    return 0;
}

static char *append_char(char *out, char ch);
static char *append_uint(char *out, uint32_t value);

static void copy_field_value(char *dst, uint32_t capacity, const char *frame, const char *key)
{
    const char *field = find_field(frame, key);
    uint32_t i = 0;

    if (capacity == 0U) {
        return;
    }

    if (field == 0) {
        dst[0] = '-';
        dst[1] = '\0';
        return;
    }

    while (*field != '\0' && *field != '=') {
        field++;
    }
    if (*field == '=') {
        field++;
    }

    while (*field != '\0' && *field != ',' && *field != '\r' && *field != '\n' && i + 1U < capacity) {
        dst[i++] = *field++;
    }
    dst[i] = '\0';
}

static bool parse_field_fixed1(const char *frame, const char *key, int32_t *value10)
{
    const char *field = find_field(frame, key);
    int32_t sign = 1;
    int32_t whole = 0;
    int32_t frac = 0;
    bool have_digit = false;

    if (field == 0 || value10 == 0) {
        return false;
    }

    while (*field != '\0' && *field != '=') {
        field++;
    }
    if (*field == '=') {
        field++;
    }
    if (*field == '-') {
        sign = -1;
        field++;
    }

    while (*field >= '0' && *field <= '9') {
        have_digit = true;
        whole = whole * 10 + (*field - '0');
        field++;
    }

    if (*field == '.') {
        field++;
        if (*field >= '0' && *field <= '9') {
            frac = *field - '0';
        }
    }

    if (!have_digit) {
        return false;
    }

    *value10 = sign * (whole * 10 + frac);
    return true;
}

static bool parse_field_uint(const char *frame, const char *key, uint32_t *value)
{
    const char *field = find_field(frame, key);
    uint32_t out = 0U;
    bool have_digit = false;

    if (field == 0 || value == 0) {
        return false;
    }

    while (*field != '\0' && *field != '=') {
        field++;
    }
    if (*field == '=') {
        field++;
    }

    while (*field >= '0' && *field <= '9') {
        have_digit = true;
        out = out * 10U + (uint32_t)(*field - '0');
        field++;
    }

    if (!have_digit) {
        return false;
    }

    *value = out;
    return true;
}

static void format_fixed1(char *dst, int32_t value10)
{
    char *out = dst;
    uint32_t abs_value;

    if (value10 < 0) {
        out = append_char(out, '-');
        abs_value = (uint32_t)(-value10);
    } else {
        abs_value = (uint32_t)value10;
    }

    out = append_uint(out, abs_value / 10U);
    out = append_char(out, '.');
    append_uint(out, abs_value % 10U);
}

static void format_pir_window(char *dst, uint32_t high_count, uint32_t total_count)
{
    char *out = dst;

    out = append_uint(out, high_count);
    out = append_char(out, '/');
    append_uint(out, total_count);
}

static void reset_node(SensorNode *node)
{
    node->ok = false;
    node->seen = false;
    node->samples = 0U;
    node->timeouts = 0U;
    node->consecutive_timeouts = 0U;
    node->pir[0] = '-';
    node->pir[1] = '\0';
    node->lux[0] = '-';
    node->lux[1] = '\0';
    node->temp[0] = '-';
    node->temp[1] = '\0';
    node->hum[0] = '-';
    node->hum[1] = '\0';
    node->bh[0] = '-';
    node->bh[1] = '\0';
    node->dht[0] = '-';
    node->dht[1] = '\0';
    node->window_samples = 0U;
    node->window_pir_high = 0U;
    node->window_lux10_sum = 0;
    node->window_temp10_sum = 0;
    node->window_hum10_sum = 0;
}

static void mark_node_timeout(const char *node_id)
{
    SensorNode *node = node_by_id(node_id);
    if (node == 0) {
        return;
    }

    node->seen = true;
    node->timeouts++;
    if (node->consecutive_timeouts < 0xFFFFFFFFU) {
        node->consecutive_timeouts++;
    }
    if (node->consecutive_timeouts < RS485_LOST_AFTER_TIMEOUTS && node->samples > 0U) {
        return;
    }
    node->ok = false;
}

static void update_node_from_frame(const char *node_id, const char *frame)
{
    SensorNode *node = node_by_id(node_id);
    int32_t value10;
    uint32_t pir;

    if (node == 0) {
        return;
    }

    node->ok = true;
    node->seen = true;
    node->samples++;
    node->consecutive_timeouts = 0U;
    node->window_samples++;

    if (parse_field_uint(frame, "PIR=", &pir) && pir != 0U) {
        node->window_pir_high++;
    }
    if (parse_field_fixed1(frame, "LUX=", &value10)) {
        node->window_lux10_sum += value10;
    }
    if (parse_field_fixed1(frame, "T=", &value10)) {
        node->window_temp10_sum += value10;
    }
    if (parse_field_fixed1(frame, "H=", &value10)) {
        node->window_hum10_sum += value10;
    }

    copy_field_value(node->bh, sizeof(node->bh), frame, "BH=");
    copy_field_value(node->dht, sizeof(node->dht), frame, "DHT=");
}

static void finalize_node_window(SensorNode *node)
{
    uint32_t count = node->window_samples;

    if (count == 0U) {
        return;
    }

    format_pir_window(node->pir, node->window_pir_high, count);
    format_fixed1(node->lux, node->window_lux10_sum / (int32_t)count);
    format_fixed1(node->temp, node->window_temp10_sum / (int32_t)count);
    format_fixed1(node->hum, node->window_hum10_sum / (int32_t)count);

    node->window_samples = 0U;
    node->window_pir_high = 0U;
    node->window_lux10_sum = 0;
    node->window_temp10_sum = 0;
    node->window_hum10_sum = 0;
}

static char *append_char(char *out, char ch)
{
    *out++ = ch;
    *out = '\0';
    return out;
}

static char to_hex(uint8_t value);

static char *append_text(char *out, const char *text)
{
    while (*text != '\0') {
        out = append_char(out, *text++);
    }
    return out;
}

static char *append_uint(char *out, uint32_t value)
{
    char digits[10];
    int pos = 0;

    if (value == 0) {
        return append_char(out, '0');
    }

    while (value > 0 && pos < (int)sizeof(digits)) {
        digits[pos++] = (char)('0' + (value % 10U));
        value /= 10U;
    }

    while (pos > 0) {
        out = append_char(out, digits[--pos]);
    }

    return out;
}

static char *append_hex4(char *out, uint16_t value)
{
    out = append_char(out, to_hex((uint8_t)(value >> 12)));
    out = append_char(out, to_hex((uint8_t)(value >> 8)));
    out = append_char(out, to_hex((uint8_t)(value >> 4)));
    out = append_char(out, to_hex((uint8_t)value));
    return out;
}

static void format_node_title(char *line, const char *id, const SensorNode *node)
{
    char *out = line;

    out = append_text(out, "ID");
    out = append_text(out, id);
    out = append_text(out, node->ok ? " OK " : " LOST ");
    out = append_text(out, "PIR:");
    out = append_text(out, node->ok ? node->pir : "-");
}

static void format_node_env(char *line, const SensorNode *node)
{
    char *out = line;

    if (!node->ok) {
        append_text(out, "L:- T:- H:-");
        return;
    }

    out = append_text(out, "L:");
    out = append_text(out, node->lux);
    out = append_text(out, " T:");
    out = append_text(out, node->temp);
    out = append_text(out, " H:");
    out = append_text(out, node->hum);
}

static void format_node_diag(char *line, const SensorNode *node)
{
    char *out = line;

    if (!node->ok) {
        append_text(out, "NO REPLY");
        return;
    }

    out = append_text(out, "BH:");
    out = append_text(out, node->bh);
    out = append_text(out, " DHT:");
    out = append_text(out, node->dht);
}

static void format_rx_diag(char *line, const char *id, const RxDiag *diag)
{
    char *out = line;

    out = append_text(out, "R");
    out = append_text(out, id);
    out = append_text(out, " L:");
    out = append_uint(out, (uint32_t)diag->len);
    out = append_text(out, " N:");
    out = append_uint(out, diag->rxne_count);
    out = append_text(out, " E:");
    out = append_uint(out, diag->rx_edges);
    out = append_text(out, " Z:");
    out = append_uint(out, diag->rx_low_samples);
    out = append_text(out, " S:");
    append_hex4(out, (uint16_t)diag->sr_errors);
}

static void lcd_render_status(void)
{
    char line[80];
    char *out;

    lcd_draw_line(0U, "RS485 SENSOR BUS", LCD_CYAN);
    lcd_draw_line(22U, "38400 5S AVG", LCD_WHITE);

    format_node_title(line, "01", &node01);
    lcd_draw_line(56U, line, node01.ok ? LCD_GREEN : LCD_RED);
    format_node_env(line, &node01);
    lcd_draw_line(78U, line, LCD_WHITE);
    format_node_diag(line, &node01);
    lcd_draw_line(100U, line, node01.ok ? LCD_GREEN : LCD_RED);

    format_node_title(line, "02", &node02);
    lcd_draw_line(138U, line, node02.ok ? LCD_GREEN : LCD_RED);
    format_node_env(line, &node02);
    lcd_draw_line(160U, line, LCD_WHITE);
    format_node_diag(line, &node02);
    lcd_draw_line(182U, line, node02.ok ? LCD_GREEN : LCD_RED);

    line[0] = '\0';
    out = append_text(line, "S01:");
    out = append_uint(out, node01.samples);
    out = append_text(out, " T01:");
    append_uint(out, node01.timeouts);
    lcd_draw_line(232U, line, LCD_WHITE);

    line[0] = '\0';
    out = append_text(line, "S02:");
    out = append_uint(out, node02.samples);
    out = append_text(out, " T02:");
    append_uint(out, node02.timeouts);
    lcd_draw_line(254U, line, LCD_WHITE);

    format_rx_diag(line, "01", &rxdiag01);
    lcd_draw_line(300U, line, LCD_WHITE);
    lcd_draw_line(322U, rxdiag01.preview, LCD_YELLOW);

    format_rx_diag(line, "02", &rxdiag02);
    lcd_draw_line(354U, line, LCD_WHITE);
    lcd_draw_line(376U, rxdiag02.preview, LCD_YELLOW);

    line[0] = '\0';
    out = append_text(line, "MILP CTRL UPD:");
    out = append_uint(out, pc_cmd.updates);
    if (pc_cmd.seen) {
        out = append_text(out, pc_cmd.led ? " LED:ON" : " LED:OFF");
        out = append_text(out, pc_cmd.buzzer ? " BZ:ON" : " BZ:OFF");
    } else {
        out = append_text(out, " LED:-- BZ:--");
    }
    lcd_draw_line(420U, line, LCD_CYAN);

    line[0] = '\0';
    out = append_text(line, "FAN:");
    out = append_text(out, pc_cmd.seen ? (pc_cmd.fan_proxy ? "ON" : "OFF") : "--");
    out = append_text(out, " AC:");
    out = append_text(out, pc_cmd.seen ? (pc_cmd.ac_proxy ? "ON" : "OFF") : "--");
    lcd_draw_line(442U, line, LCD_WHITE);

    line[0] = '\0';
    out = append_text(line, "U1 RX:");
    out = append_uint(out, uart1_rx_total);
    out = append_text(out, " OV:");
    append_uint(out, uart1_rx_overflows);
    lcd_draw_line(464U, line, LCD_YELLOW);
}

static char to_hex(uint8_t value)
{
    value &= 0x0FU;
    if (value < 10U) {
        return (char)('0' + (int)value);
    }
    return (char)('A' + (int)value - 10);
}

static char *append_visible_data(char *out, const char *data, int length)
{
    for (int i = 0; i < length; i++) {
        uint8_t ch = (uint8_t)data[i];
        if (ch >= 0x20U && ch <= 0x7EU && ch != '<' && ch != '>') {
            out = append_char(out, (char)ch);
        } else {
            out = append_char(out, '<');
            out = append_char(out, to_hex(ch >> 4));
            out = append_char(out, to_hex(ch));
            out = append_char(out, '>');
        }
    }
    return out;
}

static void send_lcd_diag(void)
{
    char line[120];
    char *out = line;

    out = append_text(out, "@A,LCD1,ID00=0x");
    out = append_hex4(out, lcd_id_00);
    out = append_text(out, ",ID04=0x");
    out = append_hex4(out, lcd_id_04_hi);
    out = append_char(out, ':');
    out = append_hex4(out, lcd_id_04_lo);
    out = append_text(out, ",IDD3=0x");
    out = append_hex4(out, lcd_id_d3);
    out = append_text(out, "\r\n");
    rs485_send(line);

    out = line;
    out = append_text(out, "@A,LCD2,R04=");
    out = append_hex4(out, lcd_raw_04[0]);
    out = append_char(out, '/');
    out = append_hex4(out, lcd_raw_04[1]);
    out = append_char(out, '/');
    out = append_hex4(out, lcd_raw_04[2]);
    out = append_char(out, '/');
    out = append_hex4(out, lcd_raw_04[3]);
    out = append_text(out, ",RD3=");
    out = append_hex4(out, lcd_raw_d3[0]);
    out = append_char(out, '/');
    out = append_hex4(out, lcd_raw_d3[1]);
    out = append_char(out, '/');
    out = append_hex4(out, lcd_raw_d3[2]);
    out = append_char(out, '/');
    out = append_hex4(out, lcd_raw_d3[3]);
    out = append_char(out, '/');
    out = append_hex4(out, lcd_raw_d3[4]);
    out = append_text(out, "\r\n");
    rs485_send(line);

    out = line;
    out = append_text(out, "@A,LCD3");
    out = append_text(out, ",BF=");
    for (uint32_t i = 0; i < 6U; i++) {
        if (i != 0U) {
            out = append_char(out, '/');
        }
        out = append_hex4(out, lcd_raw_bf[i]);
    }
    out = append_text(out, "\r\n");
    rs485_send(line);

    out = line;
    out = append_text(out, "@A,LCD4");
    out = append_text(out, ",D4=");
    for (uint32_t i = 0; i < 4U; i++) {
        if (i != 0U) {
            out = append_char(out, '/');
        }
        out = append_hex4(out, lcd_raw_d4[i]);
    }
    out = append_text(out, ",DADC=");
    out = append_hex4(out, lcd_raw_da);
    out = append_char(out, '/');
    out = append_hex4(out, lcd_raw_db);
    out = append_char(out, '/');
    out = append_hex4(out, lcd_raw_dc);
    out = append_text(out, "\r\n");
    rs485_send(line);

    out = line;
    out = append_text(out, "@A,LCD5");
    out = append_text(out, ",A1=");
    for (uint32_t i = 0; i < 4U; i++) {
        if (i != 0U) {
            out = append_char(out, '/');
        }
        out = append_hex4(out, lcd_raw_a1[i]);
    }
    out = append_text(out, "\r\n");

    rs485_send(line);
}

static void send_direction_diag(void)
{
    rs485_send("@Q,DIR=NORMAL1\r\n");
    delay_cycles(400000U);
    rs485_send_forced_direction("@Q,DIR=LOW\r\n", false);
    delay_cycles(400000U);
    rs485_send_forced_direction("@Q,DIR=HIGH\r\n", true);
    delay_cycles(400000U);
    rs485_send("@Q,DIR=NORMAL2\r\n");
}

static uint8_t rx_read_level(void)
{
    return (uint8_t)((GPIOB_IDR >> 11U) & 1U);
}

static void rx_bias_probe(void)
{
    gpio_config_pin(GPIOB_BASE, 11U, 0x4U);
    delay_cycles(20000U);
    rx_probe_float = rx_read_level();

    GPIOB_BRR = 1U << 11U;
    gpio_config_pin(GPIOB_BASE, 11U, 0x8U);
    delay_cycles(20000U);
    rx_probe_pulldown = rx_read_level();

    GPIOB_BSRR = 1U << 11U;
    gpio_config_pin(GPIOB_BASE, 11U, 0x8U);
    delay_cycles(20000U);
    rx_probe_pullup = rx_read_level();

    gpio_config_pin(GPIOB_BASE, 11U, 0x4U);
}

static void send_rx_probe_diag(void)
{
    char line[64];
    char *out = line;

    out = append_text(out, "@Q,PB11,F=");
    out = append_uint(out, rx_probe_float);
    out = append_text(out, ",D=");
    out = append_uint(out, rx_probe_pulldown);
    out = append_text(out, ",U=");
    out = append_uint(out, rx_probe_pullup);
    out = append_text(out, "\r\n");
    rs485_send(line);
}

static char *append_rx_diag_fields(char *out, const RxDiag *diag)
{
    if (diag == 0) {
        return out;
    }

    out = append_text(out, ",N=");
    out = append_uint(out, diag->rxne_count);
    out = append_text(out, ",E=");
    out = append_uint(out, diag->rx_edges);
    out = append_text(out, ",Z=");
    out = append_uint(out, diag->rx_low_samples);
    out = append_text(out, ",S=");
    out = append_hex4(out, (uint16_t)diag->sr_errors);
    out = append_text(out, ",G=");
    for (uint32_t i = 0; i < GPIO_SCAN_TOP; i++) {
        if (i != 0U) {
            out = append_char(out, '/');
        }
        out = append_char(out, diag->top_ports[i]);
        if (diag->top_edges[i] == 0U) {
            out = append_char(out, '-');
            continue;
        }
        out = append_uint(out, diag->top_pins[i]);
        out = append_char(out, ':');
        out = append_uint(out, diag->top_edges[i]);
    }
    return out;
}

static void send_node_result(const char *node_id, bool ok, const char *rx_line, int len)
{
    char tx_line[340];
    char *out = tx_line;
    const RxDiag *diag = rxdiag_by_id(node_id);

    out = append_text(out, "@A,ID=");
    out = append_text(out, node_id);
    out = append_text(out, ok ? ",OK,LEN=" : ",TIMEOUT,LEN=");
    out = append_uint(out, (uint32_t)len);
    out = append_text(out, ",DATA=");
    out = append_visible_data(out, rx_line, len);
    out = append_text(out, "\r\n");

    rs485_send(tx_line);

    out = tx_line;
    out = append_text(out, "@Q,DBG=");
    out = append_text(out, node_id);
    out = append_text(out, ok ? ",OK,L=" : ",TO,L=");
    out = append_uint(out, (uint32_t)len);
    out = append_rx_diag_fields(out, diag);
    out = append_text(out, "\r\n");
    rs485_send(tx_line);
}

static void query_node(const char *node_id)
{
    char request[24];
    char rx_line[192];
    char *out = request;

    out = append_text(out, "@Q,ID=");
    out = append_text(out, node_id);
    out = append_text(out, "*00\r\n");

    uart2_flush_rx();
    rs485_send(request);
    delay_cycles(RS485_AFTER_TX_SETTLE_CYCLES);

    int len = uart2_collect_response(node_id, rx_line, (int)sizeof(rx_line));
    bool ok = len > 0 && contains_data_frame_for_node(rx_line, node_id);
    if (RS485_QUERY_DEBUG_FRAMES) {
        send_node_result(node_id, ok, rx_line, len);
    }
    if (ok) {
        update_node_from_frame(node_id, rx_line);
    } else {
        mark_node_timeout(node_id);
    }
}

static char *append_node_summary(char *out, const char *id, const SensorNode *node)
{
    out = append_text(out, id);
    out = append_char(out, '=');
    out = append_text(out, node->ok ? "OK" : "LOST");
    out = append_text(out, ",S=");
    out = append_uint(out, node->samples);
    out = append_text(out, ",T=");
    out = append_uint(out, node->timeouts);
    out = append_text(out, ",P=");
    out = append_text(out, node->pir);
    out = append_text(out, ",L=");
    out = append_text(out, node->lux);
    out = append_text(out, ",C=");
    out = append_text(out, node->temp);
    out = append_text(out, ",H=");
    out = append_text(out, node->hum);
    return out;
}

static void uart1_send_node_frame(const char *id, const SensorNode *node)
{
    char line[160];
    char *out = line;

    if (!node->ok) {
        return;
    }

    out = append_text(out, "@D,ID=");
    out = append_text(out, id);
    out = append_text(out, ",PIR=");
    out = append_char(out, (node->pir[0] != '-' && node->pir[0] != '0') ? '1' : '0');
    out = append_text(out, ",LUX=");
    out = append_text(out, node->lux);
    out = append_text(out, ",T=");
    out = append_text(out, node->temp);
    out = append_text(out, ",H=");
    out = append_text(out, node->hum);
    out = append_text(out, ",BH=");
    out = append_text(out, node->bh);
    out = append_text(out, ",DHT=");
    out = append_text(out, node->dht);
    out = append_text(out, "\r\n");
    uart1_puts(line);
}

static void send_summary_diag(void)
{
    char line[220];
    char *out = line;

    out = append_text(out, "@A,SUM,");
    out = append_node_summary(out, "01", &node01);
    out = append_text(out, ";");
    out = append_node_summary(out, "02", &node02);
    out = append_text(out, "\r\n");
    rs485_send(line);
}

static void update_display_if_due(uint32_t *last_lcd_ms)
{
    uint32_t now = millis();

    if ((uint32_t)(now - *last_lcd_ms) < LCD_REFRESH_MS) {
        return;
    }

    finalize_node_window(&node01);
    finalize_node_window(&node02);
    lcd_render_status();
    uart1_send_node_frame("01", &node01);
    uart1_send_node_frame("02", &node02);
    if (RS485_SUMMARY_FRAMES) {
        send_summary_diag();
    }
    *last_lcd_ms = now;
}

static void usart1_init(void)
{
    RCC_APB2ENR |= RCC_APB2ENR_USART1EN;
    gpio_config_pin(GPIOA_BASE, 9U, 0xBU);
    gpio_config_pin(GPIOA_BASE, 10U, 0x4U);
    uart1_rx_ring_reset();
    USART1_BRR = USART1_BRR_115200;
    USART1_CR3 = 0U;
    USART1_CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;
    NVIC_ISER1 = 1U << (USART1_IRQN - 32U);
    uart1_puts("STM32 MILP UART1 READY\r\n");
}

static void board_init(void)
{
    RCC_APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN |
                   RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPFEN;
    RCC_APB1ENR |= RCC_APB1ENR_USART3EN;

    // PB10: USART3_TX alternate function push-pull, 50 MHz.
    gpio_config_pin(GPIOB_BASE, 10U, 0xBU);

    // PB11: USART3_RX floating input.
    gpio_config_pin(GPIOB_BASE, 11U, 0x4U);

    // PF12: RS485 RSE direction output, push-pull, 50 MHz.
    gpio_config_pin(GPIOF_BASE, RSE_PIN, 0x3U);
    rs485_set_tx(false);

    // PB5: LED0 output, active low. PB8: onboard buzzer output.
    gpio_config_pin(GPIOB_BASE, LED_PIN, 0x3U);
    gpio_config_pin(GPIOB_BASE, BUZZER_PIN, 0x3U);
    GPIOB_BSRR = 1U << LED_PIN;
    GPIOB_BRR = 1U << BUZZER_PIN;

    // USART3 at 38400 8N1, PCLK1 defaults to HSI 8 MHz after reset.
    uart3_rx_ring_reset();
    USART3_BRR = USART3_BRR_38400;
    USART3_CR3 = USART_CR3_EIE;
    USART3_CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;
    uart3_rx_irq_enable(true);
}

int main(void)
{
    uint32_t last_lcd_ms;

    systick_init();
    reset_node(&node01);
    reset_node(&node02);
    board_init();
    usart1_init();
    lcd_init();
    lcd_render_status();
    if (RS485_STARTUP_DIAG_FRAMES) {
        rx_bias_probe();
        send_lcd_diag();
        send_rx_probe_diag();
        send_direction_diag();
    }
    last_lcd_ms = millis();

    while (1) {
        milp_parse_frame();
        delay_cycles(RS485_BETWEEN_NODES_CYCLES);
        query_node("01");
        milp_parse_frame();
        update_display_if_due(&last_lcd_ms);
        delay_cycles(RS485_BETWEEN_NODES_CYCLES);
        query_node("02");
        milp_parse_frame();
        update_display_if_due(&last_lcd_ms);
        delay_cycles(RS485_BETWEEN_ROUNDS_CYCLES);
    }
}

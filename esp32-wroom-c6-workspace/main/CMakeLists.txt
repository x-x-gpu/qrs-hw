idf_component_register(
    SRCS "app_main.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES driver esp_timer esp_system spi_flash esp_driver_uart
)

idf_component_register(SRCS "main.c" "esp_io.c" "../../../core/st7735s.c" "../../../core/st7735s_reg.c"
                    INCLUDE_DIRS "." "../../../core/"
                    PRIV_REQUIRES esp_driver_spi esp_driver_gpio)
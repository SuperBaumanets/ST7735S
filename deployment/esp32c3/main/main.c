#include <stdio.h>
#include "esp_io.h"
#include "st7735s.h"

// Глобальный объект дисплея
static ST7735S_Object_t Display;

void init_display(void)
{
    ST7735S_IO_t io = {
        .Init =     ESPC3_IO_SPI_Init,
        .DeInit =   ESPC3_IO_SPI_DeInit,
        .WriteReg = ESPC3_IO_SPI_WriteDataReg,
        .SendData = ESPC3_IO_SPI_SendData,
        .Delay =    ESPC3_IO_Delay,
    };

    // Регистрация функций ввода-вывода
    ST7735S_RegisterBusIO(&Display, &io);

    // Инициализация дисплея
    ST7735S_Init(&Display, ST7735S_FORMAT_DEFAULT, ST7735S_ORIENTATION_PORTRAIT);
}

void draw_example(void)
{
    for (int i = 0; i < 25; i++){
        for (int j = 0; j < 25; j++){
            ST7735S_SetPixel(&Display, i, j, 0x00AA);
        }
    }
}

void app_main(void)
{
    init_display();
    while (1) {
        draw_example();
  }
}
#include <stdio.h>
#include "esp_io.h"
#include "st7735s.h"

// Глобальный объект дисплея
ST7735S_Object_t Display;

void init_display(void)
{
    ST7735S_IO_t io = {
        .Init = SPI_Init,
        .DeInit = SPI_DeInit,
        .Address = 0,  // Не используется для SPI
        .WriteReg = SPI_WriteReg,
        .ReadReg = SPI_RecvReg,
        .SendData = SPI_SendData,
        .RecvData = SPI_RecvData,
        .GetTick = GetTick,
    };

    // Регистрация функций ввода-вывода
    ST7735S_RegisterBusIO(&Display, &io);

    // Инициализация дисплея
    ST7735S_Init(&Display, ST7735S_FORMAT_DEFAULT, ST7735S_ORIENTATION_PORTRAIT);
    
    // Включение дисплея
    ST7735S_DisplayOn(&Display);
}

void draw_example(void)
{
    // Установка пикселя
    ST7735S_SetPixel(&Display, 10, 10, 0xFFFF); // Белый цвет
    
    // Рисование линии
    ST7735S_DrawHLine(&Display, 0, 0, 100, 0xF800); // Красная линия
    
    // Заполнение прямоугольника
    ST7735S_FillRect(&Display, 20, 20, 50, 30, 0x07E0); // Зеленый прямоугольник
    
    // Рисование окружности
    ST7735S_FillCircle(&Display, 60, 60, 20, 0x001F); // Синий круг
}

void app_main(void)
{
    init_display();

    draw_example();
}
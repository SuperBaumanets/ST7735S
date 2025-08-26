/**
  ******************************************************************************
  * @file    esp_io.c
  * @author  Slava Kochergin
  * @brief   This file includes functions for spi interface and timer.
  ******************************************************************************
**/
/* Includes ------------------------------------------------------------------ */
#include "esp_io.h"
#include <stdlib.h>

/** @addtogroup Interface
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup SPI
  * @{
  */

/** @defgroup SPI_Private_Variables Private Variables
  * @{
  */
static spi_device_handle_t hspi;
/**
  * @}
  */
  
/** @addtogroup SPI_Exported_Functions
  * @{
  */   

/**
  * @brief Initialize SPI bus and add device with predefined configuration.
  * @return Component status
  */
int32_t SPI_Init(void)
{
    esp_err_t ret;
    
    spi_bus_config_t busCfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4094,
        .flags = SPICOMMON_BUSFLAG_MASTER
    };

    spi_device_interface_config_t devCfg = {
        .command_bits = 8,
        .address_bits = 0,
        .dummy_bits = 0,
        .mode = 0,
        .duty_cycle_pos = 0,
        .cs_ena_pretrans = 0,
        .cs_ena_posttrans = 0,
        .clock_speed_hz = SPI_CLOCK_SPEED_HZ,
        .input_delay_ns = 0,
        .flags = 0,
        .queue_size = 7,
        .pre_cb = NULL,
        .post_cb = NULL
    };

    ret = spi_bus_initialize(SPI_HOST, &busCfg, SPI_DMA_CH_AUTO);
    ret += spi_bus_add_device(SPI_HOST, &devCfg, &hspi);

    if (ret != ESP_OK) {
        spi_bus_free(SPI_HOST);
        return ret;
    }

    return ESP_OK;
}

/**
  * @brief Deinitialize SPI bus and device
  * @return Component status
  */
int32_t SPI_DeInit(void)
{
    esp_err_t ret;

    if (hspi == NULL) {
        return ESP_ERR_INVALID_STATE;
    }

    ret = spi_bus_remove_device(hspi);
    ret += spi_bus_free(SPI_HOST);

    if (ret != ESP_OK) {
        return ret;
    }

    return ESP_OK;
}

/**
  * @brief  Write data to SPI device register
  * @param  reg    Register to write
  * @param  pdata  Data to write
  * @param  length Length of data in bytes
  * @return Component status
  */
int32_t SPI_WriteReg(uint8_t reg, uint8_t *pdata, uint32_t length)
{   
    esp_err_t ret;

    if (hspi == NULL) {
        return ESP_ERR_INVALID_STATE;
    }

    gpio_set_level(PIN_NUM_CS, 0);
    gpio_set_level(PIN_NUM_DC, 0);

    spi_transaction_t tspi = {
        .cmd = reg,
        .length = length * 8,
        .tx_buffer = pdata,
        .rx_buffer = NULL
    };

    ret = spi_device_transmit(hspi, &tspi);
    gpio_set_level(PIN_NUM_CS, 1);

    return ret;
}

/**
  * @brief  Read data from SPI device register
  * @param  reg    Register to read
  * @param  pdata  Buffer for read data
  * @param  length Length of data to read in bytes
  * @return Component status
  */
int32_t SPI_RecvReg(uint8_t reg, uint8_t *pdata)
{
    esp_err_t ret;

    if (hspi == NULL) {
        return ESP_ERR_INVALID_STATE;
    }

    spi_transaction_t tspi = {
        .cmd = reg,
        .length = 8,
        .tx_buffer = NULL,
        .rx_buffer = pdata
    };

    ret = spi_device_transmit(hspi, &tspi);

    return ret;
}

/**
  * @brief Send data to SPI device.
  * @param pdata  Data to write
  * @param length Length of data to write in bytes
  * @return Component status
  */
int32_t SPI_SendData(uint8_t *pdata, uint32_t length)
{   
    esp_err_t ret;

    if (hspi == NULL) {
        return ESP_ERR_INVALID_STATE;
    }

    gpio_set_level(PIN_NUM_CS, 0);
    gpio_set_level(PIN_NUM_DC, 1);

    spi_transaction_t tspi = {
        .length = length * 8,
        .tx_buffer = pdata,
        .rx_buffer = NULL
    };

    ret = spi_device_transmit(hspi, &tspi);
    gpio_set_level(PIN_NUM_CS, 1);

    return ret;
}

/**
  * @brief  Recieve data
  * @param  pdata  data to read
  * @param  length length of data to read
  * @retval Component status
  */
int32_t SPI_RecvData(uint8_t *pdata, uint32_t length)
{
    esp_err_t ret;

    if (hspi == NULL) {
        return ESP_ERR_INVALID_STATE;
    }

    spi_transaction_t tspi = {
        .length = length * 8,
        .tx_buffer = NULL,
        .rx_buffer = pdata
    };

    gpio_set_level(PIN_NUM_DC, 1);

    ret = spi_device_transmit(hspi, &tspi);

    return ret;
}

/**
  * @brief  Get current tick count in milliseconds
  * @return Current tick count
  */
int32_t GetTick(void)
{
    return (int32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
}

/**
  * @brief  Delay execution for specified milliseconds
  * @param  delay_ms  Delay time in milliseconds
  * @return Always returns 0 (success)
  */
int32_t Delay_Func(uint32_t delay_ms)
{
    vTaskDelay(pdMS_TO_TICKS(delay_ms));
    return 0;
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
/**
  * @}
  */ 

/**
  * @}
  */
  
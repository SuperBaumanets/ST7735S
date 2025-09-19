/**
  ******************************************************************************
  * @file    esp_io.c
  * @author  Slava Kochergin
  * @brief   This file includes functions for spi interface and timer.
  ******************************************************************************
**/
/* Includes ------------------------------------------------------------------*/
#include "esp_io.h"
#include <stdlib.h>
#include <string.h>

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
int32_t ESPC3_IO_SPI_Init(void)
{
  esp_err_t ret; 

  // Initialize SPI bus
  spi_bus_config_t busCfg = {
      .mosi_io_num = PIN_NUM_MOSI,
      .miso_io_num = -1,  // Not used
      .sclk_io_num = PIN_NUM_CLK,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = SOC_SPI_MAXIMUM_BUFFER_SIZE,
      .flags = SPICOMMON_BUSFLAG_MASTER
  };
  ret = spi_bus_initialize(SPI_HOST, &busCfg, SPI_DMA_DISABLED);
  if (ret != ESP_OK) {
    return ret;
  }

  // Initialize SPI device
  spi_device_interface_config_t devCfg = {
    .clock_speed_hz = SPI_CLOCK_SPEED_HZ,
    .mode = 0,
    .queue_size = 1,
    .spics_io_num = -1,
    .flags = SPI_DEVICE_NO_DUMMY
  };

  ret = spi_bus_add_device(SPI_HOST, &devCfg, &hspi);
  if (ret != ESP_OK) {
      spi_bus_free(SPI_HOST);
      return ret;
  }

  gpio_set_level(PIN_NUM_BLK, 1); // Backlight on
  
  // Reset display
  gpio_set_level(PIN_NUM_RST, 1);
  vTaskDelay(pdMS_TO_TICKS(5));

  return ESP_OK;
}

/**
  * @brief Deinitialize SPI bus and device
  * @return Component status
  */
int32_t ESPC3_IO_SPI_DeInit(void)
{
  esp_err_t ret;

  if (hspi == NULL) {
      return ESP_ERR_INVALID_STATE;
  }

  ret = spi_bus_remove_device(hspi);
  if (ret != ESP_OK) {
      return ret;
  }

  ret = spi_bus_free(SPI_HOST);
  if (ret != ESP_OK) {
      return ret;
  }

  hspi = NULL;
  return ESP_OK;
}

/**
  * @brief  Write data to SPI device register
  * @param  reg    Register to write
  * @param  pdata  Data to write
  * @param  length Length of data in bytes
  * @return Component status
  */
int32_t ESPC3_IO_SPI_WriteDataReg(uint8_t reg, uint8_t *pdata, uint32_t length)
{
  esp_err_t ret;

  if (hspi == NULL) {
      return ESP_ERR_INVALID_STATE;
  }

  // Set CS low
  gpio_set_level(PIN_NUM_CS, 0);

  // Send command (DC low)
  gpio_set_level(PIN_NUM_DC, 0);
  
  spi_transaction_t cmd_trans = {
    .length = 8,
    .tx_buffer = &reg,
  };
  
  ret = spi_device_transmit(hspi, &cmd_trans);
  if (ret != ESP_OK) {
      return ret;
  }

  // Send data (DC high)
  if (pdata != NULL && length > 0) {
    gpio_set_level(PIN_NUM_DC, 1);
    
    spi_transaction_t data_trans;
    memset(&data_trans, 0, sizeof(data_trans));

    for(int i = 0; i < length; i++){
      data_trans.length = 8,
      data_trans.tx_buffer = pdata,
      pdata++;

      ret = spi_device_transmit(hspi, &data_trans);
      if (ret != ESP_OK) {
        return ret;
      }
    }
  }

  // Set CS high
  gpio_set_level(PIN_NUM_CS, 1);

  return ESP_OK;
}

/**
  * @brief Send data to SPI device.
  * @param pdata  Data to write
  * @param length Length of data to write in bytes
  * @return Component status
  */
int32_t ESPC3_IO_SPI_SendData(uint8_t *pdata, uint32_t length)
{
  esp_err_t ret;
  if (hspi == NULL) {
      return ESP_ERR_INVALID_STATE;
  }

  // Set CS low and DC high for data
  gpio_set_level(PIN_NUM_CS, 0);
  gpio_set_level(PIN_NUM_DC, 1);

  spi_transaction_t data_trans;
  memset(&data_trans, 0, sizeof(data_trans));

  for(int i = 0; i < length; i++){
    data_trans.length = 8,
    data_trans.tx_buffer = pdata,
    pdata++;

    ret = spi_device_transmit(hspi, &data_trans);
    if (ret != ESP_OK) {
      return ret;
    }
  }

  // Set CS high
  gpio_set_level(PIN_NUM_CS, 1);

  return ESP_OK;
}

/**
  * @brief  Delay execution for specified milliseconds
  * @param  delay_ms  Delay time in milliseconds
  * @return Always returns 0 (success)
  */
int32_t ESPC3_IO_Delay(uint32_t delay_ms)
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
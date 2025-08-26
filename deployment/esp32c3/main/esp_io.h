/**
  ******************************************************************************
  * @file    esp_io.h
  * @author  Slava Kochergin
  * @brief   This file contains all the functions prototypes for the st7735.c.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SPI_H
#define SPI_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

/** @addtogroup Interface
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @defgroup SPI SPI
  * @{
  */ 

/** @defgroup SPI_Exported_Constants Exported Constants
  * @{
  */

/** 
  * @brief SPI peripheral that controls this bus
  */
#define SPI_HOST SPI2_HOST
  
/** 
  * @brief  SPI pinout  
  */  
#define PIN_NUM_MOSI 6   // GPIO7 SPI MOSI
#define PIN_NUM_CLK  4   // GPIO4 SPI CLK
#define PIN_NUM_RST  0   // GPIO0 Reset device
#define PIN_NUM_DC   1   // GPIO1 Data/Command
#define PIN_NUM_CS   2   // GPIO2 Chip Select
#define PIN_NUM_BLK  3   
/** 
  * @brief  SPI Clock 
  */  
#define SPI_CLOCK_SPEED_HZ 1000000  // 1 MHz

/**
  * @}
  */
  
/** @defgroup ST7735_Exported_Functions Exported Functions
  * @{
  */ 
int32_t SPI_Init(void);
int32_t SPI_DeInit(void);
int32_t SPI_RecvReg(uint8_t reg, uint8_t *pdata);
int32_t SPI_WriteReg(uint8_t reg, uint8_t *pdata, uint32_t length);
int32_t SPI_SendData(uint8_t *pdata, uint32_t length);
int32_t SPI_RecvData(uint8_t *pdata, uint32_t length);
int32_t Delay_Func(uint32_t delay_ms);
int32_t GetTick(void);
/**
  * @}
  */ 
      
#ifdef __cplusplus
}
#endif

#endif /* SPI_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */
  

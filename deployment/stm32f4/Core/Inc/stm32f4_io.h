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
#include <stdint.h>

/** @addtogroup Interface
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @defgroup SPI SPI
  * @{
  */ 

/** 
* @brief  ST7735S Pinout
*/
#define ST7735_RESET_PORT                                               (GPIOD)
#define ST7735_RESET_PIN                                                (GPIO_PIN_2)

#define ST7735_CS_PORT                                                  (GPIOD)
#define ST7735_CS_PIN                                                   (GPIO_PIN_1)

#define ST7735_DC_PORT                                                  (GPIOD)
#define ST7735_DC_PIN                                                   (GPIO_PIN_3)

/**
  * @}
  */
  
/** @defgroup ST7735_Exported_Functions Exported Functions
  * @{
  */ 
int32_t STM32F4_IO_SPI_Init(void);
int32_t STM32F4_IO_SPI_DeInit(void);
int32_t STM32F4_IO_SPI_WriteDataReg(uint8_t reg, uint8_t *pdata, uint32_t length);
int32_t STM32F4_IO_SPI_SendData(uint8_t *pdata, uint32_t length);
int32_t STM32F4_IO_Delay_Func(uint32_t delay_ms);
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
  
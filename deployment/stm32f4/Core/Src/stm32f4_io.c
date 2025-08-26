/**
  ******************************************************************************
  * @file    stm32f4_io.c
  * @author  Slava Kochergin
  * @brief   This file includes functions for spi interface and timer.
  ******************************************************************************
**/
/* Includes ------------------------------------------------------------------ */
#include "stm32f4_io.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"

/** @addtogroup Interface
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup STM32F4_IO
  * @{
  */
  
/** @defgroup STM32F4_IO_Private_FunctionsPrototypes Private Functions Prototypes
 * @{
 */
void IO_SendByte(uint8_t data);
void IO_WaitLastData();
/**
 * @}
 */

/** @addtogroup STM32F4_IO_Exported_Functions
  * @{
  */   

/**
  * @brief Initialize SPI bus and add device with predefined configuration.
  * @return Component status
  */
int32_t STM32F4_IO_SPI_Init(void)
{ 
  SPI1->CR1 |= SPI_CR1_SPE;

  // TO DO: Remove the pin setting from this function as it is logically incorrect
  HAL_GPIO_WritePin(ST7735_RESET_PORT, ST7735_RESET_PIN, GPIO_PIN_SET);
  HAL_Delay(5);


  return 0;
}

/**
  * @brief Deinitialize SPI bus and device
  * @return Component status
  */
int32_t STM32F4_IO_SPI_DeInit(void)
{
  return 0;
}

/**
  * @brief  Write data to SPI device register
  * @param  reg    Register to write
  * @param  pdata  Data to write
  * @param  length Length of data in bytes
  * @return Component status
  */
int32_t STM32F4_IO_SPI_WriteDataReg(uint8_t reg, uint8_t *pdata, uint32_t length)
{
  HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_RESET);

  /*Send Command*/
  HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_RESET);
  
  IO_SendByte(reg);
  IO_WaitLastData();

  /*Send Data*/
  HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_SET);

  for (uint32_t i = 0; i < length; i++)
  {
    IO_SendByte(*pdata);
    pdata++;
  }
  IO_WaitLastData();

  HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_SET);

  return 0;
}

/**
  * @brief Send data to SPI device.
  * @param pdata  Data to write
  * @param length Length of data to write in bytes
  * @return Component status
  */
int32_t STM32F4_IO_SPI_SendData(uint8_t *pdata, uint32_t length)
{   
  HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_RESET);
  
  /*Send Data*/
  HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC_PIN, GPIO_PIN_SET);

  for (uint32_t i = 0; i < length; i++)
  {
    IO_SendByte(*pdata);
    pdata++;
  }
  IO_WaitLastData();

  HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS_PIN, GPIO_PIN_SET);
  
  return 0;
}

/**
  * @brief  Delay execution for specified milliseconds
  * @param  delay_ms  Delay time in milliseconds
  * @return Always returns 0 (success)
  */
int32_t STM32F4_IO_Delay_Func(uint32_t delay_ms)
{
  HAL_Delay(delay_ms);

  return 0;
}

/** @defgroup ST7735S_Private_Functions  Private Functions
  * @{
  */

void IO_SendByte(uint8_t data)
{
  while((SPI1->SR & SPI_SR_TXE) == RESET);  
  SPI1->DR = data;
}

void IO_WaitLastData()
{
  while((SPI1->SR & SPI_SR_TXE) == RESET);
  while((SPI1->SR & SPI_SR_BSY) != RESET);
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

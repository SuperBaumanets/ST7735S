/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ST7735S_REG_H
#define ST7735S_REG_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
 
/** 
  * @brief  ST7735S System function command registers
  */
#define ST7735S_NOP                          0x00U  /* No Operation: NOP                           */
#define ST7735S_SW_RESET                     0x01U  /* Software reset: SWRESET                     */
#define ST7735S_READ_ID                      0x04U  /* Read Display ID: RDDID                      */
#define ST7735S_READ_STATUS                  0x09U  /* Read Display Statu: RDDST                   */
#define ST7735S_READ_POWER_MODE              0x0AU  /* Read Display Power: RDDPM                   */
#define ST7735S_READ_MADCTL                  0x0BU  /* Read Display: RDDMADCTL                     */
#define ST7735S_READ_PIXEL_FORMAT            0x0CU  /* Read Display Pixel: RDDCOLMOD               */  
#define ST7735S_READ_IMAGE_MODE              0x0DU  /* Read Display Image: RDDIM                   */
#define ST7735S_READ_SIGNAL_MODE             0x0EU  /* Read Display Signal: RDDSM                  */           
#define ST7735S_READ_SELF_DIAGNOSTIC         0x0FU  /* Read Display Self-Diagnostic result: RDDSDR */           
#define ST7735S_SLEEP_IN                     0x10U  /* Sleep in & booster off: SLPIN               */ 
#define ST7735S_SLEEP_OUT                    0x11U  /* Sleep out & booster on: SLPOUT              */
#define ST7735S_PARTIAL_DISPLAY_ON           0x12U  /* Partial mode on: PTLON                      */ 
#define ST7735S_NORMAL_DISPLAY_OFF           0x13U  /* Partial off (Normal): NORON                 */
#define ST7735S_DISPLAY_INVERSION_OFF        0x20U  /* Display inversion off: INVOFF               */
#define ST7735S_DISPLAY_INVERSION_ON         0x21U  /* Display inversion on: INVON                 */
#define ST7735S_GAMMA_SET                    0x26U  /* Gamma curve select: GAMSET                  */
#define ST7735S_DISPLAY_OFF                  0x28U  /* Display off: DISPOFF                        */
#define ST7735S_DISPLAY_ON                   0x29U  /* Display on: DISPON                          */
#define ST7735S_CASET                        0x2AU  /* Column address set: CASET                   */ 
#define ST7735S_RASET                        0x2BU  /* Row address set: RASET                      */
#define ST7735S_WRITE_RAM                    0x2CU  /* Memory write: RAMWR                         */  
#define ST7735S_RGBSET                       0x2DU  /* LUT for 4k,65k,262k color: RGBSET           */
#define ST7735S_READ_RAM                     0x2EU  /* Memory read: RAMRD                          */
#define ST7735S_PTLAR                        0x30U  /* Partial start/end address set: PTLAR        */ 
#define ST7735S_SCRLAR                       0x33U  /* Scroll area set: SCRLAR                     */ 
#define ST7735S_TE_LINE_OFF                  0x34U  /* Tearing effect line off: TEOFF              */ 
#define ST7735S_TE_LINE_ON                   0x35U  /* Tearing effect mode set & on: TEON          */ 
#define ST7735S_MADCTL                       0x36U  /* Memory data access control: MADCTL          */
#define ST7735S_VSCSAD                       0x37U  /* Scroll Start Address of RAM: VSCSAD         */ 
#define ST7735S_IDLE_MODE_OFF                0x38U  /* Idle mode off: IDMOFF                       */ 
#define ST7735S_IDLE_MODE_ON                 0x39U  /* Idle mode on: IDMON                         */ 
#define ST7735S_COLOR_MODE                   0x3AU  /* Interface pixel format: COLMOD              */
#define ST7735S_READ_RDID1                   0xDAU  /* Read ID1: RDID1                             */
#define ST7735S_READ_RDID2                   0xDBU  /* Read ID2: RDID2                             */
#define ST7735S_READ_RDID3                   0xDCU  /* Read ID3: RDID3                             */
/**
 * @}
 */

/** 
  * @brief  ST7735S Panel function command registers
  */
#define ST7735S_FRAME_RATE_CTRL1             0xB1U  /* In normal mode (Full colors): FRMCTR1       */
#define ST7735S_FRAME_RATE_CTRL2             0xB2U  /* In Idle mode (8-colors): FRMCTR2            */   
#define ST7735S_FRAME_RATE_CTRL3             0xB3U  /* In partial mode + Full colors: FRMCTR3      */
#define ST7735S_FRAME_INVERSION_CTRL         0xB4U  /* Display inversion control: INVCTR           */
#define ST7735S_PWR_CTRL1                    0xC0U  /* Power control setting: PWCTR1               */ 
#define ST7735S_PWR_CTRL2                    0xC1U  /* Power control setting: PWCTR2               */ 
#define ST7735S_PWR_CTRL3                    0xC2U  /* In normal mode (Full colors): PWCTR3        */
#define ST7735S_PWR_CTRL4                    0xC3U  /* In Idle mode (8-colors): PWCTR4             */ 
#define ST7735S_PWR_CTRL5                    0xC4U  /* In partial mode + Full colors: PWCTR5       */
#define ST7735S_VCOMH_VCOML_CTRL1            0xC5U  /* VCOM control 1: VMCTR1                      */ 
#define ST7735S_VMOF_CTRL                    0xC7U  /* Set VCOM offset control: VMOFCTR            */ 
#define ST7735S_WRID2                        0xD1U  /* Set LCM version code: WRID2                 */ 
#define ST7735S_WRID3                        0xD2U  /* Customer Project code: WRID3                */ 
#define ST7735S_NV_CTRL1                     0xD9U  /* NVM control status: NVCTR1                  */
#define ST7735S_NV_CTRL2                     0xDEH  /* NVM Read Command: NVCTR2                    */ 
#define ST7735S_NV_CTRL3                     0xDFU  /* NVM Write Command: NVCTR3                   */
#define ST7735S_PV_GAMMA_CTRL                0xE0U  /* Set Gamma adjustment (+ polarity): GAMCTRP1 */
#define ST7735S_NV_GAMMA_CTRL                0xE1U  /* Set Gamma adjustment (- polarity): GAMCTRN1 */
#define ST7735S_EXT_CTRL                     0xF0U  /* Extension command control                   */
#define ST7735S_PWR_CTRL6                    0xFCU  /* In partial mode + Idle mode: PWCTR6         */                        !
/**
 * @}
 */

  
/** @defgroup ST7735S_REG_Exported_Types Exported Types
  * @{
  */ 
typedef int32_t (*ST7735S_Write_Func)(void *, uint8_t, uint8_t*, uint32_t);
typedef int32_t (*ST7735S_Send_Func)(void *, uint8_t*, uint32_t);

typedef struct
{
  ST7735S_Write_Func   WriteReg;
  ST7735S_Send_Func    SendData;
  void                 *handle;
} ST7735S_ctx_t;
/**
 * @}
 */


/** @defgroup ST7735S_REG_Exported_Functions Exported Functions
  * @{
  */ 
int32_t ST7735S_write_reg(ST7735S_ctx_t *ctx, uint8_t reg, uint8_t *pdata, uint32_t length);
int32_t ST7735S_send_data(ST7735S_ctx_t *ctx, uint8_t *pdata, uint32_t length);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* ST7735S_REG_H */
/* Includes ------------------------------------------------------------------*/
#include "st7735s.h"

/** @defgroup ST7735S_Private_Types Private Types
 * @{
 */
typedef struct {
  uint32_t Width;
  uint32_t Height;
  uint32_t Orientation;
} ST7735S_Ctx_t;
/**
 * @}
 */

/** @defgroup ST7735S_Private_Variables Private Variables
 * @{
 */
ST7735S_LCD_Drv_t ST7735S_LCD_Driver = {
    ST7735S_Init,
    ST7735S_SetOrientation, 
    ST7735S_GetOrientation,
    ST7735S_GetWidth,
    ST7735S_GetHeight,
    ST7735S_DeInit,
    ST7735S_DisplayOn, 
    ST7735S_DisplayOff, 
    ST7735S_SetCursor,  
    ST7735S_SetPixel,
    ST7735S_DrawHLine,  
    ST7735S_DrawVLine,  
    ST7735S_FillRect
};

/* The below table handle the different values to be set to Memory Data Access
   Control depending on the orientation and pbm image writing where the data
   order is inverted
*/
static uint32_t OrientationTab[4][2] = {
    {0x40U, 0xC0U}, /* Portrait orientation choice of LCD screen */
    {0x80U, 0x00U}, /* Portrait rotated 180� orientation choice of LCD screen */
    {0x20U, 0x60U}, /* Landscape orientation choice of LCD screen */
    {0xE0U, 0xA0U}  /* Landscape rotated 180� orientation choice of LCD screen */
};

static ST7735S_Ctx_t ST7735SCtx;
/**
 * @}
 */

/** @defgroup ST7735S_Private_FunctionsPrototypes Private Functions Prototypes
 * @{
 */
static int32_t ST7735S_WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint32_t Length);
static int32_t ST7735S_SendDataWrap(void *Handle, uint8_t *pData, uint32_t Length);
static int32_t ST7735S_IO_Delay(void *Handle, uint32_t Delay);
static int32_t ST7735S_SetDisplayWindow(ST7735S_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height);
/**
 * @}
 */

/** @addtogroup ST7735S_Exported_Functions
 * @{
 */
/**
 * @brief  Register component IO bus
 * @param  pObj Component object pointer
 * @param  pIO  Component IO structure pointer
 * @retval Component status
 */
int32_t ST7735S_RegisterBusIO(ST7735S_Object_t *pObj, ST7735S_IO_t *pIO) {
  int32_t ret;

  if (pObj == NULL) {
    ret = ST7735S_ERROR;
  } else {
    pObj->IO.Init = pIO->Init;
    pObj->IO.DeInit = pIO->DeInit;
    pObj->IO.WriteReg = pIO->WriteReg;
    pObj->IO.SendData = pIO->SendData;
    pObj->IO.Delay = pIO->Delay;

    pObj->Ctx.WriteReg = ST7735S_WriteRegWrap;
    pObj->Ctx.SendData = ST7735S_SendDataWrap;
    pObj->Ctx.handle = pObj;

    if (pObj->IO.Init != NULL) {
      ret = pObj->IO.Init();
    } else {
      ret = ST7735S_ERROR;
    }
  }

  return ret;
}

/**
 * @brief  Initialize the ST7735S LCD Component.
 * @param  pObj Component object
 * @param  ColorCoding RGB mode
 * @param  Orientation Display orientation
 * @retval Component status
 */
int32_t ST7735S_Init(ST7735S_Object_t *pObj, uint32_t ColorCoding, uint32_t Orientation) {
  uint8_t tmp = 0x00U;
  int32_t ret;

  if (pObj == NULL) {
    ret = ST7735S_ERROR;
  } else {

    ret = ST7735S_write_reg(&pObj->Ctx, ST7735S_SW_RESET, &tmp, 0);
    ST7735S_IO_Delay(pObj, 150);

    /* Out of sleep mode, 0 args, no delay */
    tmp = 0x00U;
    ret = ST7735S_write_reg(&pObj->Ctx, ST7735S_SLEEP_OUT, &tmp, 0);
    ST7735S_IO_Delay(pObj, 500);

    /* Frame rate ctrl - normal mode, 3 args:Rate = fosc/(1x2+40) *
     * (LINE+2C+2D)*/
    tmp = 0x01U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_FRAME_RATE_CTRL1, &tmp, 1);
    tmp = 0x2CU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_FRAME_RATE_CTRL1, &tmp, 1);
    tmp = 0x2DU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_FRAME_RATE_CTRL1, &tmp, 1);

    /* Frame rate control - idle mode, 3 args:Rate = fosc/(1x2+40) *
     * (LINE+2C+2D) */
    tmp = 0x01U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_FRAME_RATE_CTRL2, &tmp, 1);
    tmp = 0x2CU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_FRAME_RATE_CTRL2, &tmp, 1);
    tmp = 0x2DU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_FRAME_RATE_CTRL2, &tmp, 1);

    /* Frame rate ctrl - partial mode, 6 args: Dot inversion mode, Line
     * inversion mode */
    tmp = 0x01U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_FRAME_RATE_CTRL3, &tmp, 1);
    tmp = 0x2CU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_FRAME_RATE_CTRL3, &tmp, 1);
    tmp = 0x2DU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_FRAME_RATE_CTRL3, &tmp, 1);
    tmp = 0x01U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_FRAME_RATE_CTRL3, &tmp, 1);
    tmp = 0x2CU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_FRAME_RATE_CTRL3, &tmp, 1);
    tmp = 0x2DU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_FRAME_RATE_CTRL3, &tmp, 1);

    /* Display inversion ctrl, 1 arg, no delay: No inversion */
    tmp = 0x07U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_FRAME_INVERSION_CTRL, &tmp, 1);

    /* Power control, 3 args, no delay: -4.6V , AUTO mode */
    tmp = 0xA2U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PWR_CTRL1, &tmp, 1);
    tmp = 0x02U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PWR_CTRL1, &tmp, 1);
    tmp = 0x84U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PWR_CTRL1, &tmp, 1);

    /* Power control, 1 arg, no delay: VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
     */
    tmp = 0xC5U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PWR_CTRL2, &tmp, 1);

    /* Power control, 2 args, no delay: Opamp current small, Boost frequency */
    tmp = 0x0AU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PWR_CTRL3, &tmp, 1);
    tmp = 0x00U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PWR_CTRL3, &tmp, 1);

    /* Power control, 2 args, no delay: BCLK/2, Opamp current small & Medium low
     */
    tmp = 0x8AU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PWR_CTRL4, &tmp, 1);
    tmp = 0x2AU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PWR_CTRL4, &tmp, 1);

    /* Power control, 2 args, no delay */
    tmp = 0x8AU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PWR_CTRL5, &tmp, 1);
    tmp = 0xEEU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PWR_CTRL5, &tmp, 1);

    /* Power control, 1 arg, no delay */
    tmp = 0x0EU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_VCOMH_VCOML_CTRL1, &tmp, 1);

    /* Don't invert display, no args, no delay */
    ret +=
        ST7735S_write_reg(&pObj->Ctx, ST7735S_DISPLAY_INVERSION_OFF, &tmp, 0);

    /* Set color mode, 1 arg, no delay */
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_COLOR_MODE,
                             (uint8_t *)&ColorCoding, 1);

    /* Sparkles and rainbows, 16 args, no delay */
    tmp = 0x02U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x1CU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x07U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x12U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x37U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x32U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x29U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x2DU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x29U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x25U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x2BU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x39U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x00U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x01U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x03U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x10U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_PV_GAMMA_CTRL, &tmp, 1);

    /* Sparkles and rainbows, 16 args, no delay */
    tmp = 0x03U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_NV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x1DU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_NV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x07U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_NV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x06U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_NV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x2EU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_NV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x2CU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_NV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x29U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_NV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x2DU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_NV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x2EU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_NV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x2EU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_NV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x37U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_NV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x3FU;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_NV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x00U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_NV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x00U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_NV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x02U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_NV_GAMMA_CTRL, &tmp, 1);
    tmp = 0x10U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_NV_GAMMA_CTRL, &tmp, 1);

    /* Normal display on, no args, no delay */
    tmp = 0x00U;
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_NORMAL_DISPLAY_OFF, &tmp, 1);

    /* Main screen turn on, no delay */
    ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_DISPLAY_ON, &tmp, 1);

    /* Set the display Orientation and the default display window */
    ret += ST7735S_SetOrientation(pObj, Orientation);
  }

  if (ret != ST7735S_OK) {
    ret = ST7735S_ERROR;
  }

  return ret;
}

/**
 * @brief  De-Initialize the ST7735S LCD Component.
 * @param  pObj Component object
 * @retval Component status
 */
int32_t ST7735S_DeInit(ST7735S_Object_t *pObj) {
  (void)(pObj);

  return ST7735S_OK;
}

/**
 * @brief  Set the Display Orientation.
 * @param  pObj Component object
 * @param  Orientation ST7735S_ORIENTATION_PORTRAIT,
 * ST7735S_ORIENTATION_PORTRAIT_ROT180 ST7735S_ORIENTATION_LANDSCAPE or
 * ST7735S_ORIENTATION_LANDSCAPE_ROT180
 * @retval The component status
 */
int32_t ST7735S_SetOrientation(ST7735S_Object_t *pObj, uint32_t Orientation) {
  int32_t ret;
  uint8_t tmp;

  if ((Orientation == ST7735S_ORIENTATION_PORTRAIT) ||
      (Orientation == ST7735S_ORIENTATION_PORTRAIT_ROT180)) {
    ST7735SCtx.Width = ST7735S_WIDTH;
    ST7735SCtx.Height = ST7735S_HEIGHT;
  } else {
    ST7735SCtx.Width = ST7735S_HEIGHT;
    ST7735SCtx.Height = ST7735S_WIDTH;
  }

  ret = ST7735S_SetDisplayWindow(pObj, 0U, 0U, ST7735SCtx.Width,
                                 ST7735SCtx.Height);

  tmp = (uint8_t)OrientationTab[Orientation][1];
  ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_MADCTL, &tmp, 1);

  ST7735SCtx.Orientation = Orientation;

  if (ret != ST7735S_OK) {
    ret = ST7735S_ERROR;
  }

  return ret;
}

/**
 * @brief  Get the LCD pixel Width.
 * @param  pObj Component object
 * @retval The Lcd Pixel Width
 */
int32_t ST7735S_GetWidth(ST7735S_Object_t * pObj, uint32_t *XSize) {
  (void)pObj;
  *XSize = ST7735SCtx.Width;
  return ST7735S_OK;
}

/**
 * @brief  Get the LCD pixel Height.
 * @param  pObj Component object
 * @retval The Lcd Pixel Height
 */
int32_t ST7735S_GetHeight(ST7735S_Object_t * pObj, uint32_t *YSize) {
  (void)pObj;
  *YSize = ST7735SCtx.Height;
  return ST7735S_OK;
}

/**
 * @brief  Set the Display Orientation.
 * @param  pObj Component object
 * @param  Orientation ST7735S_ORIENTATION_PORTRAIT,
 * ST7735S_ORIENTATION_LANDSCAPE or ST7735S_ORIENTATION_LANDSCAPE_ROT180
 * @retval The component status
 */
int32_t ST7735S_GetOrientation(ST7735S_Object_t *pObj, uint32_t *Orientation) {

  *Orientation = ST7735SCtx.Orientation;

  return ST7735S_OK;
}

/**
 * @brief  Enables the Display.
 * @param  pObj Component object
 * @retval The component status
 */
int32_t ST7735S_DisplayOn(ST7735S_Object_t *pObj) {
  int32_t ret;
  uint8_t tmp = 0;

  ret = ST7735S_write_reg(&pObj->Ctx, ST7735S_NORMAL_DISPLAY_OFF, &tmp, 0);
  (void)ST7735S_IO_Delay(pObj, 10);
  ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_DISPLAY_ON, &tmp, 0);
  (void)ST7735S_IO_Delay(pObj, 10);
  ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_MADCTL, &tmp, 0);
  tmp = 0xC0U;
  ret += ST7735S_send_data(&pObj->Ctx, &tmp, 1);
  if (ret != ST7735S_OK) {
    ret = ST7735S_ERROR;
  }

  return ret;
}

/**
 * @brief  Disables the Display.
 * @param  pObj Component object
 * @retval The component status
 */
int32_t ST7735S_DisplayOff(ST7735S_Object_t *pObj) {
  int32_t ret;
  uint8_t tmp = 0;

  ret = ST7735S_write_reg(&pObj->Ctx, ST7735S_NORMAL_DISPLAY_OFF, &tmp, 0);
  (void)ST7735S_IO_Delay(pObj, 10);
  ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_DISPLAY_OFF, &tmp, 0);
  (void)ST7735S_IO_Delay(pObj, 10);
  ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_MADCTL, &tmp, 0);
  tmp = 0xC0U;
  ret += ST7735S_send_data(&pObj->Ctx, &tmp, 1);
  if (ret != ST7735S_OK) {
    ret = ST7735S_ERROR;
  }

  return ret;
}

/**
 * @brief  Set Cursor position.
 * @param  pObj Component object
 * @param  Xpos specifies the X position.
 * @param  Ypos specifies the Y position.
 * @retval The component status
 */
int32_t ST7735S_SetCursor(ST7735S_Object_t *pObj, uint32_t Xpos,
                          uint32_t Ypos) {
  int32_t ret;
  uint8_t data[2];
  
  ret = ST7735S_write_reg(&pObj->Ctx, ST7735S_CASET, data, 0);
  data[0] = (uint8_t)(Xpos >> 8U);
  data[1] = (uint8_t)(Xpos & 0xFFU);
  ret += ST7735S_send_data(&pObj->Ctx, data, 2);

  ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_RASET, data, 0);
  data[0] = (uint8_t)(Ypos >> 8U);
  data[1] = (uint8_t)(Ypos & 0xFFU);
  ret += ST7735S_send_data(&pObj->Ctx, data, 2);

  ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_WRITE_RAM, data, 0);

  if (ret != ST7735S_OK) {
    ret = ST7735S_ERROR;
  }

  return ret;
}

  /**
   * @brief  Write pixel.
   * @param  pObj Component object
   * @param  Xpos specifies the X position.
   * @param  Ypos specifies the Y position.
   * @param  Color the RGB pixel color in RGB565 format
   * @retval The component status
   */
  int32_t ST7735S_SetPixel(ST7735S_Object_t * pObj, uint32_t Xpos,
                           uint32_t Ypos, uint16_t Color) {
    int32_t ret = ST7735S_OK;
    uint8_t color[2];

    /* Exchange LSB and MSB to fit LCD specification */
    color[0] = (Color & 0xFF00) >> 8;
    color[1] = Color & 0x00FF;
    if ((Xpos >= ST7735SCtx.Width) || (Ypos >= ST7735SCtx.Height)) {
      ret = ST7735S_ERROR;
    }
    /* Set Cursor */
    ST7735S_SetCursor(pObj, Xpos, Ypos);
    /* Write RAM data */
    ST7735S_send_data(&pObj->Ctx, color, 2);

    return ret;
  }

  /**
   * @brief  Draw Horizontal line.
   * @param  pObj Component object
   * @param  Xpos   specifies the X position.
   * @param  Ypos   specifies the Y position.
   * @param  Length specifies the Line length.
   * @param  Color  Specifies the RGB color in RGB565 format
   * @retval The component status
   */
  int32_t ST7735S_DrawHLine(ST7735S_Object_t * pObj, uint32_t Xpos,
                            uint32_t Ypos, uint32_t Length, uint16_t Color) {
    int32_t ret = ST7735S_OK;
    uint32_t i;
    static uint8_t pdata[640];

    if ((Xpos + Length) > ST7735SCtx.Width) {
      ret = ST7735S_ERROR;
    } 
    /* Set Cursor */
    else if (ST7735S_SetCursor(pObj, Xpos, Ypos) != ST7735S_OK) {
      ret = ST7735S_ERROR;
    } else {
      for (i = 0; i < Length; i++) {
        pdata[2U * i] = (uint8_t)((Color & 0xFF00) >> 8);
        pdata[(2U * i) + 1U] = (uint8_t)(Color & 0x00FF);
      }
      if (ST7735S_send_data(&pObj->Ctx, (uint8_t *)&pdata[0], 2U * Length) !=
          ST7735S_OK) {
        ret = ST7735S_ERROR;
      }
    }

    return ret;
  }

    /**
   * @brief  Draw vertical line.
   * @param  pObj Component object
   * @param  Color    Specifies the RGB color
   * @param  Xpos     specifies the X position.
   * @param  Ypos     specifies the Y position.
   * @param  Length   specifies the Line length.
   * @retval The component status
   */
  int32_t ST7735S_DrawVLine(ST7735S_Object_t * pObj, uint32_t Xpos,
                            uint32_t Ypos, uint32_t Length, uint16_t Color) {
    int32_t ret = ST7735S_OK;
    uint32_t counter;

    if ((Ypos + Length) > ST7735SCtx.Height) {
      ret = ST7735S_ERROR;
    } else {
      for (counter = 0; counter < Length; counter++) {
        if (ST7735S_SetPixel(pObj, Xpos, Ypos + counter, Color) != ST7735S_OK) {
          ret = ST7735S_ERROR;
          break;
        }
      }
    }

    return ret;
  }

  /**
   * @brief  Fill rectangle
   * @param  pObj Component object
   * @param  Xpos X position
   * @param  Ypos Y position
   * @param  Width Rectangle width
   * @param  Height Rectangle height
   * @param  Color Draw color
   * @retval Component status
   */
int32_t ST7735S_FillRect(ST7735S_Object_t * pObj, uint32_t Xpos,
                           uint32_t Ypos, uint32_t Width, uint32_t Height,
                           uint16_t Color) {
  int32_t ret = ST7735S_OK;
  uint32_t i, y_pos = Ypos;

  for (i = 0; i < Height; i++) {
    if (ST7735S_DrawHLine(pObj, Xpos, y_pos, Width, Color) != ST7735S_OK) {
      ret = ST7735S_ERROR;
      break;
    }
    y_pos++;
  }

  return ret;
}
/**
 * @}
 */


/** @defgroup ST7735S_Private_Functions  Private Functions
 * @{
 */

/**
 * @brief  Wrap component WriteReg to Bus Write function
 * @param  handle  Component object handle
 * @param  Reg  The target register address to write
 * @param  pData  The target register value to be written
 * @param  Length  buffer size to be written
 * @retval Component error status
 */
static int32_t ST7735S_WriteRegWrap(void *Handle, uint8_t Reg, uint8_t *pData, uint32_t Length) {
  ST7735S_Object_t *pObj = (ST7735S_Object_t *)Handle;
  return pObj->IO.WriteReg(Reg, pData, Length);
}

/**
 * @brief  Wrap component SendData to Bus Write function
 * @param  handle  Component object handle
 * @param  pData  The target register value to be written
 * @retval Component error status
 */
static int32_t ST7735S_SendDataWrap(void *Handle, uint8_t *pData, uint32_t Length) {
  ST7735S_Object_t *pObj = (ST7735S_Object_t *)Handle;
  return pObj->IO.SendData(pData, Length);
}

/**
 * @brief  ST7735S delay
 * @param  Delay  Delay in ms
 * @retval Component error status
 */
static int32_t ST7735S_IO_Delay(void *Handle, uint32_t Delay) {
  ST7735S_Object_t *pObj = (ST7735S_Object_t *)Handle;
  return pObj->IO.Delay(Delay);
}

static int32_t ST7735S_SetDisplayWindow(ST7735S_Object_t * pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height) {
  int32_t ret;
  uint8_t data[4];
  /* Column addr set, 4 args, no delay: XSTART = Xpos, XEND = (Xpos + Width -
   * 1) */
  ret = ST7735S_write_reg(&pObj->Ctx, ST7735S_CASET, data, 0);
  data[0] = (uint8_t)(Xpos >> 8U);
  data[1] = (uint8_t)(Xpos & 0xFFU);
  data[2] = (uint8_t)((Xpos + Width - 1U) >> 8U);
  data[3] = (uint8_t)((Xpos + Width - 1U) & 0xFFU);
  ret += ST7735S_send_data(&pObj->Ctx, data, 4);
  /* Row addr set, 4 args, no delay: YSTART = Ypos, YEND = (Ypos + Height - 1)
   */
  ret += ST7735S_write_reg(&pObj->Ctx, ST7735S_RASET, data, 0);
  data[0] = (uint8_t)(Ypos >> 8U);
  data[1] = (uint8_t)(Ypos & 0xFFU);
  data[2] = (uint8_t)((Ypos + Height - 1U) >> 8U);
  data[3] = (uint8_t)((Ypos + Height - 1U) & 0xFFU);
  ret += ST7735S_send_data(&pObj->Ctx, data, 4);
  if (ret != ST7735S_OK) {
    ret = ST7735S_ERROR;
  }
  return ret;
}
/**
 * @}
 */

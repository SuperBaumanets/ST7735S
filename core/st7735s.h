/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ST7735S_H
#define ST7735S_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "st7735s_reg.h"
#include <stddef.h>

/** @defgroup ST7735S_Exported_Types Exported Types
  * @{
  */
typedef int32_t (*ST7735S_Init_Func)     (void);
typedef int32_t (*ST7735S_DeInit_Func)   (void);
typedef int32_t (*ST7735S_WriteReg_Func) (uint8_t, uint8_t*, uint32_t);
typedef int32_t (*ST7735S_SendData_Func) (uint8_t*, uint32_t);
typedef int32_t (*ST7735S_Delay_Func)    (uint32_t);

typedef struct
{
  ST7735S_Init_Func          Init;
  ST7735S_DeInit_Func        DeInit;
  ST7735S_WriteReg_Func      WriteReg;
  ST7735S_SendData_Func      SendData;
  ST7735S_Delay_Func         Delay; 
} ST7735S_IO_t;

 
typedef struct
{
  ST7735S_IO_t         IO;
  ST7735S_ctx_t        Ctx;   
  uint8_t              IsInitialized;
} ST7735S_Object_t;

typedef struct
{
  /* Control functions */
  int32_t (*Init             )(ST7735S_Object_t*, uint32_t, uint32_t);
  int32_t (*SetOrientation   )(ST7735S_Object_t*, uint32_t);
  int32_t (*GetOrientation   )(ST7735S_Object_t*, uint32_t*);
  int32_t (*GetWidth         )(ST7735S_Object_t*, uint32_t*);
  int32_t (*GetHeight        )(ST7735S_Object_t*, uint32_t*);
  int32_t (*DeInit           )(ST7735S_Object_t*);
  int32_t (*DisplayOn        )(ST7735S_Object_t*);
  int32_t (*DisplayOff       )(ST7735S_Object_t*);

  /* Drawing functions*/
  int32_t (*SetCursor       ) (ST7735S_Object_t*, uint32_t, uint32_t); 
  int32_t (*SetPixel        ) (ST7735S_Object_t*, uint32_t, uint32_t, uint16_t);
  int32_t (*DrawHLine       ) (ST7735S_Object_t*, uint32_t, uint32_t, uint32_t, uint16_t);
  int32_t (*DrawVLine       ) (ST7735S_Object_t*, uint32_t, uint32_t, uint32_t, uint16_t);
  int32_t (*FillRect        ) (ST7735S_Object_t*, uint32_t, uint32_t, uint32_t, uint32_t, uint16_t);
}ST7735S_LCD_Drv_t;
/**
 * @}
 */

/** 
  * @brief  ST7735S Status  
  */  
#define ST7735S_OK                (0)
#define ST7735S_ERROR             (-1)
/**
 * @}
 */

/** 
  * @brief  ST7735S Size  
  */  
#define  ST7735S_WIDTH           128U
#define  ST7735S_HEIGHT          160U
/**
 * @}
 */

/**
 *  @brief LCD_OrientationTypeDef
 *  Possible values of Display Orientation
 */
#define ST7735S_ORIENTATION_PORTRAIT         0x00U /* Portrait orientation choice of LCD screen               */
#define ST7735S_ORIENTATION_PORTRAIT_ROT180  0x01U /* Portrait rotated 180� orientation choice of LCD screen  */
#define ST7735S_ORIENTATION_LANDSCAPE        0x02U /* Landscape orientation choice of LCD screen              */
#define ST7735S_ORIENTATION_LANDSCAPE_ROT180 0x03U /* Landscape rotated 180� orientation choice of LCD screen */
/**
 * @}
 */

/**
 *  @brief  Possible values of pixel data format (ie color coding) 
 */
#define ST7735S_FORMAT_RBG444                0x03U /* Pixel format chosen is RGB444 : 12 bpp */   
#define ST7735S_FORMAT_RBG565                0x05U /* Pixel format chosen is RGB565 : 16 bpp */
#define ST7735S_FORMAT_RBG666                0x06U /* Pixel format chosen is RGB666 : 18 bpp */
#define ST7735S_FORMAT_DEFAULT               ST7735S_FORMAT_RBG565
/**
  * @}
  */
  
/** @defgroup ST7735S_Exported_Functions Exported Functions
  * @{
  */ 
int32_t ST7735S_RegisterBusIO (ST7735S_Object_t *pObj, ST7735S_IO_t *pIO);
int32_t ST7735S_Init(ST7735S_Object_t *pObj, uint32_t ColorCoding, uint32_t Orientation);
int32_t ST7735S_SetOrientation(ST7735S_Object_t *pObj, uint32_t Orientation);
int32_t ST7735S_GetOrientation(ST7735S_Object_t *pObj, uint32_t *Orientation);
int32_t ST7735S_GetWidth(ST7735S_Object_t *pObj, uint32_t *XSize);
int32_t ST7735S_GetHeight(ST7735S_Object_t *pObj, uint32_t *YSize);
int32_t ST7735S_DeInit(ST7735S_Object_t *pObj);
int32_t ST7735S_DisplayOn(ST7735S_Object_t *pObj);
int32_t ST7735S_DisplayOff(ST7735S_Object_t *pObj);

int32_t ST7735S_SetCursor(ST7735S_Object_t *pObj, uint32_t Xpos, uint32_t Ypos);
int32_t ST7735S_SetPixel(ST7735S_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint16_t Color);
int32_t ST7735S_DrawHLine(ST7735S_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint16_t Color);
int32_t ST7735S_DrawVLine(ST7735S_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint16_t Color);
int32_t ST7735S_FillRect(ST7735S_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint16_t Color);
/**
 * @}
 */


/** @defgroup ST7735S_Exported_Variable Exported Variable
  * @{
  */ 
extern ST7735S_LCD_Drv_t   ST7735S_LCD_Driver;
/**
 * @}
 */
      
#ifdef __cplusplus
}
#endif

#endif /* ST7735S_H */
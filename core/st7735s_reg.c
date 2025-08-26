/* Includes ------------------------------------------------------------------*/
#include "st7735s_reg.h"


/**
  * @brief  Write ST7735S register
  * @param  ctx    Component context
  * @param  reg    Register to write
  * @param  pdata  data to write to the register
  * @param  length length of data to write to the register
  * @retval Component status
  */
int32_t ST7735S_write_reg(ST7735S_ctx_t *ctx, uint8_t reg, uint8_t *pdata, uint32_t length)
{
  return ctx->WriteReg(ctx->handle, reg, pdata, length);
}

/**
  * @brief  Send data
  * @param  ctx    Component context
  * @param  pdata  data to write
  * @param  length length of data to write
  * @retval Component status
  */
int32_t ST7735S_send_data(ST7735S_ctx_t *ctx, uint8_t *pdata, uint32_t length)
{
  return ctx->SendData(ctx->handle, pdata, length);
}
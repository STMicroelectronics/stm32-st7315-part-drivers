/**
  ******************************************************************************
  * @file    st7315.c
  * @author  MCD Application Team
  * @brief   This file includes the LCD driver for st7315 LCD.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "st7315.h"

static st7315_status_t st7315_apply_default_configuration(st7315_obj_t *pobj);

/**
  * @brief  Initialise the st7315 LCD Component.
  * @param  pobj : Pointer to part object.
  * @retval error status.
  */
st7315_status_t st7315_init(st7315_obj_t *pobj, uint32_t dev_id)
{
  if (pobj->is_initialized == 0U)
  {
    pobj->is_initialized = 1U;
    /* assign the instance ID */
    pobj->io.id = dev_id;
    /* no frame buffer attached yet */
    pobj->frame_buffer = NULL;

    /* Initialise st7315 low level bus layer */
    if (st7315_io_init(&(pobj->io)) != 0)
    {
      return ST7315_ERROR;
    }
    /* Perform hardware reset sequence (RST pin cycle) */
    if (st7315_io_hw_init_sequence(&(pobj->io)) != 0)
    {
      return ST7315_ERROR;
    }
    /* Initialisation command sequence */
    if (st7315_apply_default_configuration(pobj) != ST7315_OK)
    {
      return ST7315_ERROR;
    }
  }

  return ST7315_OK;
}

/**
  * @brief  De-initialise the st7315 LCD Component.
  * @param  pobj : Pointer to part object.
  * @retval error status.
  */
st7315_status_t st7315_deinit(st7315_obj_t *pobj)
{
  if (pobj->is_initialized == 1U)
  {
    st7315_status_t ret;

    /* Turn off the display */
    ret = st7315_display_off(pobj);

    pobj->is_initialized = 0U;
    pobj->frame_buffer = NULL;

    if (ret != ST7315_OK)
    {
      return ST7315_ERROR;
    }
  }

  return ST7315_OK;
}

/**
  * @brief  Enables the Display.
  * @param  pobj : Pointer to part object.
  * @retval error status.
  */
st7315_status_t st7315_display_on(st7315_obj_t *pobj)
{
  /* Power Control command (8Dh): configure the charge pump */
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_POWER_CONTROL) != 0)
  {
    return ST7315_ERROR;
  }
  /* VB=1, CP=00: enable built-in charge pump at 7.5 V */
  if (st7315_io_write_command(&(pobj->io), ST7315_POWER_CTRL_PUMP_ON_7_5V) != 0)
  {
    return ST7315_ERROR;
  }
  /* Display ON command (AFh): activate panel output */
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_DISPLAY_ON) != 0)
  {
    return ST7315_ERROR;
  }

  return ST7315_OK;
}

/**
  * @brief  Disables the Display.
  * @param  pobj : Pointer to part object.
  * @retval error status.
  */
st7315_status_t st7315_display_off(st7315_obj_t *pobj)
{
  /* Power Control command (8Dh): configure the charge pump */
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_POWER_CONTROL) != 0)
  {
    return ST7315_ERROR;
  }
  /* VB=0: disable built-in charge pump */
  if (st7315_io_write_command(&(pobj->io), ST7315_POWER_CTRL_PUMP_OFF) != 0)
  {
    return ST7315_ERROR;
  }
  /* Display OFF command (AEh): deactivate panel output */
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_DISPLAY_OFF) != 0)
  {
    return ST7315_ERROR;
  }

  return ST7315_OK;
}

/**
  * @brief  Select normal or inverse display.
  * @param  pobj : Pointer to part object.
  * @param  mode : ST7315_DISPLAY_NORMAL or ST7315_DISPLAY_INVERSE.
  * @retval error status.
  */
st7315_status_t st7315_inverse_display(st7315_obj_t *pobj, st7315_display_mode_t mode)
{
  if (st7315_io_write_command(&(pobj->io), (uint8_t)mode) != 0)
  {
    return ST7315_ERROR;
  }

  return ST7315_OK;
}

/**
  * @brief  Set the physical scan orientation of the panel.
  * @param  pobj        : Pointer to part object.
  * @param  orientation : One of ST7315_ORIENTATION_NORMAL, _FLIP_H, _FLIP_V, _ROTATE_180.
  * @retval error status.
  * @note   Takes effect immediately on the controller output; the frame buffer
  *         content and bit layout are not changed by this call.
  */
st7315_status_t st7315_set_orientation(st7315_obj_t *pobj, st7315_orientation_t orientation)
{
  uint8_t seg_cmd;
  uint8_t com_cmd;

  /* NORMAL is the upright baseline (SEG remap + COM remap) matching the power-on
     default; each flip toggles one axis relative to that orientation. */

  /* Select SEG scan direction (horizontal axis) */
  if ((orientation == ST7315_ORIENTATION_FLIP_H) || (orientation == ST7315_ORIENTATION_ROTATE_180))
  {
    seg_cmd = ST7315_CMD_SEG_NORMAL;        /* A0h: SEG0 to SEG127 */
  }
  else
  {
    seg_cmd = ST7315_CMD_SEG_REMAP;         /* A1h: SEG127 to SEG0 */
  }

  /* Select COM scan direction (vertical axis) */
  if ((orientation == ST7315_ORIENTATION_FLIP_V) || (orientation == ST7315_ORIENTATION_ROTATE_180))
  {
    com_cmd = ST7315_CMD_COM_SCAN_NORMAL;   /* C0h: COM0 to COM63 */
  }
  else
  {
    com_cmd = ST7315_CMD_COM_SCAN_REMAP;    /* C8h: COM63 to COM0 */
  }

  if (st7315_io_write_command(&(pobj->io), seg_cmd) != 0)
  {
    return ST7315_ERROR;
  }
  if (st7315_io_write_command(&(pobj->io), com_cmd) != 0)
  {
    return ST7315_ERROR;
  }

  return ST7315_OK;
}

/**
  * @brief  Set Page position.
  * @param  pobj : Pointer to part object.
  * @param  page : Specifies the Page position (0-7).
  * @retval error status.
  */
st7315_status_t st7315_set_page(st7315_obj_t *pobj, uint8_t page)
{
  if (page > (ST7315_PAGE_NUMBER - 1U))
  {
    return ST7315_ERROR;
  }
  if (st7315_io_write_command(&(pobj->io), (uint8_t)(ST7315_CMD_SET_PAGE | page)) != 0)
  {
    return ST7315_ERROR;
  }

  return ST7315_OK;
}

/**
  * @brief  Set Column position.
  * @param  pobj   : Pointer to part object.
  * @param  column : Specifies the Column position (0-127).
  * @retval error status.
  */
st7315_status_t st7315_set_column(st7315_obj_t *pobj, uint8_t column)
{
  if (column > (ST7315_COLUMN_NUMBER - 1U))
  {
    return ST7315_ERROR;
  }
  /* Send lower bits of column address REG[3:0] */
  if (st7315_io_write_command(&(pobj->io),
                              (uint8_t)(ST7315_CMD_SET_LOWER_COLUMN | (column & ST7315_COLUMN_LOW_NIBBLE_MASK))) != 0)
  {
    return ST7315_ERROR;
  }
  /* Send upper bits of column address REG[6:4], shifted down into bits [2:0] */
  if (st7315_io_write_command(&(pobj->io),
                              (uint8_t)(ST7315_CMD_SET_HIGHER_COLUMN
                                        | ((column >> 4U) & ST7315_COLUMN_LOW_NIBBLE_MASK))) != 0)
  {
    return ST7315_ERROR;
  }

  return ST7315_OK;
}

/**
  * @brief  Clean the display DDRAM using the dedicated protected command sequence.
  * @param  pobj : Pointer to part object.
  * @retval error status.
  */
st7315_status_t st7315_clean_ddram(st7315_obj_t *pobj)
{
  /* Protected 5-byte sequence that instructs the controller to zero all DDRAM cells */
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_CLEAN_DDRAM_1) != 0)
  {
    return ST7315_ERROR;
  }
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_CLEAN_DDRAM_2) != 0)
  {
    return ST7315_ERROR;
  }
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_CLEAN_DDRAM_3) != 0)
  {
    return ST7315_ERROR;
  }
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_CLEAN_DDRAM_4) != 0)
  {
    return ST7315_ERROR;
  }
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_CLEAN_DDRAM_5) != 0)
  {
    return ST7315_ERROR;
  }
  /* Restore normal RAM-driven display mode */
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_ALL_PIXEL_NORMAL) != 0)
  {
    return ST7315_ERROR;
  }

  return ST7315_OK;
}

/**
  * @brief  Configure the horizontal scrolling parameters.
  * @param  pobj        : Pointer to part object.
  * @param  scroll_mode : ST7315_SCROLL_RIGHT or ST7315_SCROLL_LEFT.
  * @param  start_page  : Start page for scrolling (0-7).
  * @param  end_page    : End page for scrolling; must be >= start_page (0-7).
  * @param  frequency   : ST7315_SCROLL_FREQ_2FRAMES to ST7315_SCROLL_FREQ_128FRAMES.
  * @retval error status.
  */
st7315_status_t st7315_horizontal_scrolling_setup(st7315_obj_t *pobj, st7315_scroll_mode_t scroll_mode,
                                                  uint8_t start_page, uint8_t end_page,
                                                  st7315_scroll_freq_t frequency)
{
  if ((start_page > (ST7315_PAGE_NUMBER - 1U)) || (end_page > (ST7315_PAGE_NUMBER - 1U)) || (end_page < start_page))
  {
    return ST7315_ERROR;
  }
  /* Byte 1: scroll direction command (26h = left-to-right, 27h = right-to-left) */
  if (st7315_io_write_command(&(pobj->io), (uint8_t)scroll_mode) != 0)
  {
    return ST7315_ERROR;
  }
  /* Byte 2: mandatory dummy byte, must be 0x00 */
  if (st7315_io_write_command(&(pobj->io), ST7315_SCROLL_DUMMY_BYTE) != 0)
  {
    return ST7315_ERROR;
  }
  /* Byte 3: SPS - start page address of the scroll region */
  if (st7315_io_write_command(&(pobj->io), start_page) != 0)
  {
    return ST7315_ERROR;
  }
  /* Byte 4: ST - time interval between each scroll step (frames) */
  if (st7315_io_write_command(&(pobj->io), (uint8_t)frequency) != 0)
  {
    return ST7315_ERROR;
  }
  /* Byte 5: SPE - end page address of the scroll region */
  if (st7315_io_write_command(&(pobj->io), end_page) != 0)
  {
    return ST7315_ERROR;
  }
  /* Byte 6: SCS - column start address (0x00) */
  if (st7315_io_write_command(&(pobj->io), ST7315_SCROLL_START_COLUMN) != 0)
  {
    return ST7315_ERROR;
  }
  /* Byte 7: SCE - column end address (0x7F, full width) */
  if (st7315_io_write_command(&(pobj->io), ST7315_SCROLL_END_COLUMN) != 0)
  {
    return ST7315_ERROR;
  }

  return ST7315_OK;
}

/**
  * @brief  Start Display Scrolling.
  * @param  pobj : Pointer to part object.
  * @retval error status.
  */
st7315_status_t st7315_scrolling_start(st7315_obj_t *pobj)
{
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_START_SCROLL) != 0)
  {
    return ST7315_ERROR;
  }

  return ST7315_OK;
}

/**
  * @brief  Stop Display Scrolling.
  * @param  pobj : Pointer to part object.
  * @retval error status.
  */
st7315_status_t st7315_scrolling_stop(st7315_obj_t *pobj)
{
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_STOP_SCROLL) != 0)
  {
    return ST7315_ERROR;
  }

  return ST7315_OK;
}

/**
  * @brief  Configure the vertical scrolling parameters.
  * @param  pobj        : Pointer to part object.
  * @param  dir         : ST7315_VSCROLL_LEFT_TO_RIGHT or ST7315_VSCROLL_RIGHT_TO_LEFT.
  * @param  start_page  : Start page of the scroll region (0-7).
  * @param  end_page    : End page of the scroll region; must be >= start_page (0-7).
  * @param  frequency   : ST7315_SCROLL_FREQ_2FRAMES to ST7315_SCROLL_FREQ_128FRAMES.
  * @param  vert_offset : Vertical scroll offset in rows (0-ST7315_VERT_SCROLL_OFFSET_MAX).
  * @retval error status.
  */
st7315_status_t st7315_vertical_scrolling_setup(st7315_obj_t *pobj, st7315_vscroll_dir_t dir,
                                                uint8_t start_page, uint8_t end_page,
                                                st7315_scroll_freq_t frequency, uint8_t vert_offset)
{
  if ((start_page > (ST7315_PAGE_NUMBER - 1U)) || (end_page > (ST7315_PAGE_NUMBER - 1U))
      || (end_page < start_page) || (vert_offset > ST7315_VERT_SCROLL_OFFSET_MAX))
  {
    return ST7315_ERROR;
  }
  /* Vertical Scroll Area command (A3h): define fixed and scrolling row regions */
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_SET_VERT_SCROLL_AREA) != 0)
  {
    return ST7315_ERROR;
  }
  /* SVF: number of fixed rows at the top (0 = none) */
  if (st7315_io_write_command(&(pobj->io), 0x00U) != 0)
  {
    return ST7315_ERROR;
  }
  /* SVN: number of scroll-area rows, max field value 63 (ST7315_PIXEL_HEIGHT - 1) */
  if (st7315_io_write_command(&(pobj->io), (uint8_t)(ST7315_PIXEL_HEIGHT - 1U)) != 0)
  {
    return ST7315_ERROR;
  }
  /* Vertical+Horizontal Scroll command: VHS[1:0] scroll direction */
  if (st7315_io_write_command(&(pobj->io), (uint8_t)dir) != 0)
  {
    return ST7315_ERROR;
  }
  /* SHC: horizontal scroll enable bit (0 = no horizontal component) */
  if (st7315_io_write_command(&(pobj->io), ST7315_SCROLL_DUMMY_BYTE) != 0)
  {
    return ST7315_ERROR;
  }
  /* SPS: start page address of the scroll region */
  if (st7315_io_write_command(&(pobj->io), start_page) != 0)
  {
    return ST7315_ERROR;
  }
  /* ST: time interval between each scroll step (frames) */
  if (st7315_io_write_command(&(pobj->io), (uint8_t)frequency) != 0)
  {
    return ST7315_ERROR;
  }
  /* SPE: end page address of the scroll region */
  if (st7315_io_write_command(&(pobj->io), end_page) != 0)
  {
    return ST7315_ERROR;
  }
  /* SVO: vertical scroll offset in rows */
  if (st7315_io_write_command(&(pobj->io), vert_offset) != 0)
  {
    return ST7315_ERROR;
  }
  /* SCS: column start address (0x00) */
  if (st7315_io_write_command(&(pobj->io), ST7315_SCROLL_START_COLUMN) != 0)
  {
    return ST7315_ERROR;
  }
  /* SCE: column end address (0x7F, full width) */
  if (st7315_io_write_command(&(pobj->io), ST7315_SCROLL_END_COLUMN) != 0)
  {
    return ST7315_ERROR;
  }

  return ST7315_OK;
}

/**
  * @brief  Set the ST7315 contrast level.
  * @param  pobj     : Pointer to part object.
  * @param  contrast : Contrast value (0x00->0xFF). Higher value = brighter.
  * @retval error status.
  */
st7315_status_t st7315_set_contrast(st7315_obj_t *pobj, uint8_t contrast)
{
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_SET_CONTRAST) != 0)
  {
    return ST7315_ERROR;
  }
  if (st7315_io_write_command(&(pobj->io), contrast) != 0)
  {
    return ST7315_ERROR;
  }

  return ST7315_OK;
}

/**
  * @brief  Set the display start line (RAM row mapped to the first panel row).
  * @param  pobj : Pointer to part object.
  * @param  line : Start line address (0->ST7315_START_LINE_MAX).
  * @retval error status.
  */
st7315_status_t st7315_set_start_line(st7315_obj_t *pobj, uint8_t line)
{
  if (line > ST7315_START_LINE_MAX)
  {
    return ST7315_ERROR;
  }
  if (st7315_io_write_command(&(pobj->io), (uint8_t)(ST7315_CMD_SET_START_LINE | line)) != 0)
  {
    return ST7315_ERROR;
  }

  return ST7315_OK;
}

/**
  * @brief  Force all pixels ON or restore normal GDDRAM-driven display.
  * @param  pobj : Pointer to part object.
  * @param  mode : ST7315_ALL_PIXEL_ON_MODE or ST7315_ALL_PIXEL_NORMAL_MODE.
  * @retval error status.
  * @note   GDDRAM content is not changed; only the display output is affected.
  */
st7315_status_t st7315_all_pixel_on(st7315_obj_t *pobj, st7315_all_pixel_mode_t mode)
{
  if (st7315_io_write_command(&(pobj->io), (uint8_t)(ST7315_CMD_ALL_PIXEL_NORMAL | (uint8_t)mode)) != 0)
  {
    return ST7315_ERROR;
  }

  return ST7315_OK;
}

/**
  * @brief  Enable or disable the zoom-in display mode.
  * @param  pobj : Pointer to part object.
  * @param  mode : ST7315_ZOOM_ENABLE or ST7315_ZOOM_DISABLE.
  * @retval error status.
  */
st7315_status_t st7315_set_zoom(st7315_obj_t *pobj, st7315_zoom_mode_t mode)
{
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_ZOOM_IN) != 0)
  {
    return ST7315_ERROR;
  }
  if (st7315_io_write_command(&(pobj->io), (uint8_t)mode) != 0)
  {
    return ST7315_ERROR;
  }

  return ST7315_OK;
}

/**
  * @brief  Configure the fade-out or blinking display effect.
  * @param  pobj     : Pointer to part object.
  * @param  mode     : ST7315_FADE_BLINK_DISABLED, ST7315_FADE_OUT_ENABLED, or ST7315_BLINK_ENABLED.
  * @param  interval : Fade step interval; one of the st7315_fade_interval_t values
  *                    (ST7315_FADE_INTERVAL_8FRAMES to ST7315_FADE_INTERVAL_128FRAMES).
  *                    Ignored when mode is DISABLED.
  * @retval error status.
  */
st7315_status_t st7315_set_fade_blinking(st7315_obj_t *pobj, st7315_fade_blink_mode_t mode,
                                         st7315_fade_interval_t interval)
{
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_SET_FADE_BLINKING) != 0)
  {
    return ST7315_ERROR;
  }
  if (st7315_io_write_command(&(pobj->io), (uint8_t)((uint8_t)mode | (uint8_t)interval)) != 0)
  {
    return ST7315_ERROR;
  }

  return ST7315_OK;
}

/**
  * @brief  Attach an application frame buffer to the part object.
  * @param  pobj         : Pointer to part object.
  * @param  frame_buffer : Pointer to the frame buffer used by st7315_synchronize().
  * @retval error status.
  * @note   The buffer must be at least ST7315_FRAME_BUFFER_SIZE bytes (1024 bytes).
  */
st7315_status_t st7315_set_frame_buffer(st7315_obj_t *pobj, uint8_t *frame_buffer)
{
  if (frame_buffer == NULL)
  {
    return ST7315_ERROR;
  }

  pobj->frame_buffer = frame_buffer;

  return ST7315_OK;
}

/**
  * @brief  Refresh the display from the frame buffer attached to the part object.
  * @param  pobj : Pointer to part object.
  * @retval error status.
  */
st7315_status_t st7315_synchronize(st7315_obj_t *pobj)
{
  if (pobj->frame_buffer == NULL)
  {
    return ST7315_ERROR;
  }

  /* Set Display Start Line to 0 */
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_SET_START_LINE) != 0)
  {
    return ST7315_ERROR;
  }

  /* Set Column Address: start (0) and end (127) */
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_SET_COLUMN_ADDRESS) != 0)
  {
    return ST7315_ERROR;
  }
  if (st7315_io_write_command(&(pobj->io), ST7315_COLUMN_ADDR_START) != 0)
  {
    return ST7315_ERROR;
  }
  if (st7315_io_write_command(&(pobj->io), ST7315_COLUMN_ADDR_END) != 0)
  {
    return ST7315_ERROR;
  }

  /* Set Page Address: start (0) and end (7) */
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_SET_PAGE_ADDRESS) != 0)
  {
    return ST7315_ERROR;
  }
  if (st7315_io_write_command(&(pobj->io), ST7315_PAGE_ADDR_START) != 0)
  {
    return ST7315_ERROR;
  }
  if (st7315_io_write_command(&(pobj->io), ST7315_PAGE_ADDR_END) != 0)
  {
    return ST7315_ERROR;
  }

  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_SET_LOWER_COLUMN) != 0)
  {
    return ST7315_ERROR;
  }
  if (st7315_io_write_command(&(pobj->io), ST7315_CMD_SET_HIGHER_COLUMN) != 0)
  {
    return ST7315_ERROR;
  }

  /* Fill GDDRAM of LCD page by page */
  for (uint8_t page = 0U; page < (uint8_t)ST7315_PAGE_NUMBER; page++)
  {
    if (st7315_io_write_command(&(pobj->io), (uint8_t)(ST7315_CMD_SET_PAGE + page)) != 0)
    {
      return ST7315_ERROR;
    }
    if (st7315_io_write_command(&(pobj->io), ST7315_CMD_SET_LOWER_COLUMN) != 0)
    {
      return ST7315_ERROR;
    }
    if (st7315_io_write_command(&(pobj->io), ST7315_CMD_SET_HIGHER_COLUMN) != 0)
    {
      return ST7315_ERROR;
    }
    if (st7315_io_write_multiple_data(&(pobj->io),
                                      &pobj->frame_buffer[ST7315_COLUMN_NUMBER * page],
                                      ST7315_COLUMN_NUMBER) != 0)
    {
      return ST7315_ERROR;
    }
  }

  return ST7315_OK;
}


/**
  * @brief  Send the default configuration command sequence to the st7315.
  * @param  pobj : Pointer to part object.
  * @retval error status.
  */
static st7315_status_t st7315_apply_default_configuration(st7315_obj_t *pobj)
{
  static const uint8_t init_seq[] =
  {
    ST7315_CMD_DISPLAY_OFF,
    ST7315_CMD_SET_LOWER_COLUMN,
    ST7315_CMD_SET_HIGHER_COLUMN,
    ST7315_CMD_SET_START_LINE,
    ST7315_CMD_SET_PAGE,
    ST7315_CMD_SET_CONTRAST,
    ST7315_CONTRAST_DEFAULT,
    ST7315_CMD_SEG_REMAP,
    ST7315_CMD_ALL_PIXEL_NORMAL,
    ST7315_CMD_NORMAL_DISPLAY,
    ST7315_CMD_SET_DUTY,
    ST7315_DUTY_1_64,
    ST7315_CMD_COM_SCAN_REMAP,
    ST7315_CMD_SET_DISPLAY_OFFSET,
    ST7315_DISPLAY_OFFSET_NONE,
    ST7315_CMD_SET_FRAME_RATE,
    ST7315_FRAME_RATE_DEFAULT,
    ST7315_CMD_SET_PRECHARGE_PERIOD,
    ST7315_PRECHARGE_DEFAULT,
    ST7315_CMD_SET_COM_PINS,
    ST7315_COM_PINS_DEFAULT,
    ST7315_CMD_SET_VCOMH,
    ST7315_VCOMH_0_83_VOLED,
    ST7315_CMD_POWER_CONTROL,
    ST7315_POWER_CTRL_PUMP_ON_7_5V,
    ST7315_CMD_DISPLAY_ON
  };

  for (uint32_t i = 0U; i < (uint32_t)(sizeof(init_seq) / sizeof(init_seq[0])); i++)
  {
    if (st7315_io_write_command(&(pobj->io), init_seq[i]) != 0)
    {
      return ST7315_ERROR;
    }
  }
  HAL_Delay(100); /* Display ON delay 100ms */

  return ST7315_OK;
}


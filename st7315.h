/**
  ******************************************************************************
  * @file    st7315.h
  * @brief   This file is the header of st7315.c
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ST7315_H
#define ST7315_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "st7315_io.h"

/**
  * @brief  ST7315 display resolution in pixels (128 x 64)
  */
#define ST7315_PIXEL_WIDTH    128U /*!< Number of pixel columns */
#define ST7315_PIXEL_HEIGHT   64U  /*!< Number of pixel rows */

/**
  * @brief  ST7315 GDDRAM layout: 128 columns, 8 pages of 8 rows each
  */
#define ST7315_COLUMN_NUMBER         128U /*!< Number of pixel columns */
#define ST7315_PAGE_NUMBER           8U   /*!< Number of pages */
#define ST7315_PIXELS_PER_PAGE_ROW   8U   /*!< Number of pixel rows packed into one GDDRAM page byte */

/**
  * @brief  ST7315 frame buffer size in bytes (1 bit per pixel monochrome: width x height / 8)
  */
#define ST7315_FRAME_BUFFER_SIZE  (ST7315_PIXEL_WIDTH * ST7315_PIXEL_HEIGHT / ST7315_PIXELS_PER_PAGE_ROW)

/**
  * @brief  ST7315 command set
  */
#define ST7315_CMD_SET_LOWER_COLUMN      0x00U /*!< Set lower column address (page mode), base */
#define ST7315_CMD_SET_HIGHER_COLUMN     0x10U /*!< Set higher column address (page mode), base */
#define ST7315_CMD_SET_COLUMN_ADDRESS    0x21U /*!< Set column address start/end (horizontal/vertical mode) */
#define ST7315_CMD_SET_PAGE_ADDRESS      0x22U /*!< Set page address start/end (horizontal/vertical mode) */
#define ST7315_CMD_STOP_SCROLL           0x2EU /*!< Stop scrolling */
#define ST7315_CMD_START_SCROLL          0x2FU /*!< Start scrolling */
#define ST7315_CMD_SET_START_LINE        0x40U /*!< Set display start line, base */
#define ST7315_CMD_SET_CONTRAST          0x81U /*!< Set contrast (followed by EV byte) */
#define ST7315_CMD_POWER_CONTROL         0x8DU /*!< Power control (followed by parameter byte) */
#define ST7315_CMD_SEG_NORMAL            0xA0U /*!< SEG direction: SEG0 to SEG127 (normal) */
#define ST7315_CMD_SEG_REMAP             0xA1U /*!< SEG direction: SEG127 to SEG0 (mirror) */
#define ST7315_CMD_ALL_PIXEL_NORMAL      0xA4U /*!< Resume to RAM content display (normal) */
#define ST7315_CMD_NORMAL_DISPLAY        0xA6U /*!< Normal (non-inverted) display */
#define ST7315_CMD_INVERSE_DISPLAY       0xA7U /*!< Inverse (inverted) display */
#define ST7315_CMD_SET_DUTY              0xA8U /*!< Set multiplex (duty) ratio (followed by DT byte) */
#define ST7315_CMD_DISPLAY_OFF           0xAEU /*!< Display OFF */
#define ST7315_CMD_DISPLAY_ON            0xAFU /*!< Display ON */
#define ST7315_CMD_SET_PAGE              0xB0U /*!< Set page address (page mode), base */
#define ST7315_CMD_COM_SCAN_NORMAL       0xC0U /*!< COM scan direction: COM0 to COM63 (normal) */
#define ST7315_CMD_COM_SCAN_REMAP        0xC8U /*!< COM scan direction: COM63 to COM0 (mirror) */
#define ST7315_CMD_SET_DISPLAY_OFFSET    0xD3U /*!< Set COM0 / display offset (followed by parameter byte) */
#define ST7315_CMD_SET_FRAME_RATE        0xD5U /*!< Set frame rate / oscillator divider (followed by parameter byte) */
#define ST7315_CMD_SET_PRECHARGE_PERIOD  0xD9U /*!< Set pre-charge period (followed by parameter byte) */
#define ST7315_CMD_SET_COM_PINS          0xDAU /*!< Set COM pins configuration (followed by parameter byte) */
#define ST7315_CMD_SET_VCOMH             0xDBU /*!< Set VCOMH level (followed by parameter byte) */

/**
  * @brief  ST7315 command parameter values used by this driver
  */
#define ST7315_CONTRAST_DEFAULT          0x8FU /*!< Default contrast level */
#define ST7315_DUTY_1_64                 0x3FU /*!< Multiplex duty = 1/64 */
#define ST7315_DISPLAY_OFFSET_NONE       0x00U /*!< No display offset */
#define ST7315_FRAME_RATE_DEFAULT        0x90U /*!< Default frame rate / oscillator divider */
#define ST7315_PRECHARGE_DEFAULT         0x22U /*!< Default pre-charge / discharge period */
#define ST7315_COM_PINS_DEFAULT          0x12U /*!< COM pins: VDIR normal, ALT normal */
#define ST7315_VCOMH_0_83_VOLED          0x30U /*!< VCOMH ~ 0.83 x VOLED */
#define ST7315_POWER_CTRL_PUMP_ON_7_5V   0x14U /*!< Charge pump ON, VOLED 7.5 V */
#define ST7315_POWER_CTRL_PUMP_OFF       0x10U /*!< Charge pump OFF (external VOLED) */

/**
  * @brief  ST7315 column and page window limits used during frame buffer refresh
  */
#define ST7315_COLUMN_ADDR_START         0x00U /*!< First column address */
#define ST7315_COLUMN_ADDR_END           0x7FU /*!< Last column address (127) */
#define ST7315_PAGE_ADDR_START           0x00U /*!< First page address */
#define ST7315_PAGE_ADDR_END             0x07U /*!< Last page address (7) */
#define ST7315_COLUMN_LOW_NIBBLE_MASK    0x0FU /*!< Lower column address nibble mask */

/**
  * @brief  ST7315 scrolling command parameter values used by this driver
  */
#define ST7315_SCROLL_DUMMY_BYTE         0x00U /*!< Dummy byte in scroll setup */
#define ST7315_SCROLL_START_COLUMN       0x00U /*!< Scroll start column */
#define ST7315_SCROLL_END_COLUMN         0x7FU /*!< Scroll end column (column 127) */

/**
  * @brief  ST7315 clean DDRAM protected command sequence (5 fixed bytes, must be sent with A0 = 0)
  */
#define ST7315_CMD_CLEAN_DDRAM_1         0x94U /*!< Clean DDRAM sequence byte 1 */
#define ST7315_CMD_CLEAN_DDRAM_2         0x5AU /*!< Clean DDRAM sequence byte 2 */
#define ST7315_CMD_CLEAN_DDRAM_3         0x80U /*!< Clean DDRAM sequence byte 3 */
#define ST7315_CMD_CLEAN_DDRAM_4         0x94U /*!< Clean DDRAM sequence byte 4 */
#define ST7315_CMD_CLEAN_DDRAM_5         0xA5U /*!< Clean DDRAM sequence byte 5 */

/**
  * @brief  ST7315 additional command codes
  */
#define ST7315_CMD_SET_FADE_BLINKING     0x23U /*!< Set fade out / blinking (followed by parameter byte) */
#define ST7315_CMD_SET_VERT_SCROLL_AREA  0xA3U /*!< Set vertical scroll area (followed by fixed-rows and scroll-rows bytes) */
#define ST7315_CMD_ZOOM_IN               0xD6U /*!< Zoom in (followed by enable byte) */

/**
  * @brief  ST7315 addressing limits
  */
#define ST7315_START_LINE_MAX            0x3FU /*!< Maximum display start line (63) */
#define ST7315_VERT_SCROLL_OFFSET_MAX    0x3FU /*!< Maximum vertical scroll row offset (63) */

/**
  * @brief  ST7315 pixel color values (monochrome: black = all bits 0, white = all bits 1)
  */
#define ST7315_COLOR_BLACK  0x00U /*!< Black pixel (all bits 0) */
#define ST7315_COLOR_WHITE  0xFFU /*!< White pixel (all bits 1) */

/**
  * @brief  ST7315 display orientation
  *         Controls the physical scan direction of SEG and COM drivers,
  *         allowing the panel to be mounted in any of four rotations.
  */
typedef enum
{
  ST7315_ORIENTATION_NORMAL     = 0U, /*!< SEG127->SEG0, COM63->COM0  (default, upright) */
  ST7315_ORIENTATION_FLIP_H     = 1U, /*!< SEG0->SEG127, COM63->COM0  (mirror X)         */
  ST7315_ORIENTATION_FLIP_V     = 2U, /*!< SEG127->SEG0, COM0->COM63  (mirror Y)         */
  ST7315_ORIENTATION_ROTATE_180 = 3U  /*!< SEG0->SEG127, COM0->COM63  (rotate 180)       */
} st7315_orientation_t;

/**
  * @brief  ST7315 display inversion mode (normal: GDDRAM bit 1 = pixel ON; inverse: GDDRAM bit 0 = pixel ON)
  */
typedef enum
{
  ST7315_DISPLAY_NORMAL  = ST7315_CMD_NORMAL_DISPLAY,
  ST7315_DISPLAY_INVERSE = ST7315_CMD_INVERSE_DISPLAY
} st7315_display_mode_t;

/**
  * @brief  ST7315 horizontal scrolling direction
  */
typedef enum
{
  ST7315_SCROLL_RIGHT = 0x26U, /*!< Content shifts right */
  ST7315_SCROLL_LEFT  = 0x27U  /*!< Content shifts left */
} st7315_scroll_mode_t;

/**
  * @brief  ST7315 scrolling step frequency (in display frames)
  */
typedef enum
{
  ST7315_SCROLL_FREQ_2FRAMES   = 0x07U, /*!<   2 frames per scroll step */
  ST7315_SCROLL_FREQ_3FRAMES   = 0x04U, /*!<   3 frames per scroll step */
  ST7315_SCROLL_FREQ_4FRAMES   = 0x05U, /*!<   4 frames per scroll step */
  ST7315_SCROLL_FREQ_5FRAMES   = 0x06U, /*!<   5 frames per scroll step */
  ST7315_SCROLL_FREQ_6FRAMES   = 0x00U, /*!<   6 frames per scroll step */
  ST7315_SCROLL_FREQ_32FRAMES  = 0x01U, /*!<  32 frames per scroll step */
  ST7315_SCROLL_FREQ_64FRAMES  = 0x02U, /*!<  64 frames per scroll step */
  ST7315_SCROLL_FREQ_128FRAMES = 0x03U  /*!< 128 frames per scroll step */
} st7315_scroll_freq_t;

/**
  * @brief  ST7315 vertical scrolling direction
  */
typedef enum
{
  ST7315_VSCROLL_LEFT_TO_RIGHT = 0x29U, /*!< Scroll vertically from left to right */
  ST7315_VSCROLL_RIGHT_TO_LEFT = 0x2AU  /*!< Scroll vertically from right to left */
} st7315_vscroll_dir_t;

/**
  * @brief  ST7315 all-pixel-on mode
  */
typedef enum
{
  ST7315_ALL_PIXEL_NORMAL_MODE = 0U, /*!< Normal display driven from GDDRAM */
  ST7315_ALL_PIXEL_ON_MODE     = 1U  /*!< Force all pixels ON regardless of GDDRAM */
} st7315_all_pixel_mode_t;

/**
  * @brief  ST7315 zoom-in mode
  */
typedef enum
{
  ST7315_ZOOM_DISABLE = 0U, /*!< Zoom in disabled */
  ST7315_ZOOM_ENABLE  = 1U  /*!< Zoom in enabled  */
} st7315_zoom_mode_t;

/**
  * @brief  ST7315 fade out / blinking mode
  */
typedef enum
{
  ST7315_FADE_BLINK_DISABLED = 0x00U, /*!< Fade out and blinking disabled */
  ST7315_FADE_OUT_ENABLED    = 0x20U, /*!< Fade out enabled */
  ST7315_BLINK_ENABLED       = 0x30U  /*!< Blinking enabled */
} st7315_fade_blink_mode_t;

/**
  * @brief  ST7315 fade step interval (number of frames per fade step).
  *         Each step adds 8 frames: interval_frames = (code + 1) x 8.
  */
typedef enum
{
  ST7315_FADE_INTERVAL_8FRAMES   = 0x00U, /*!<   8 frames per step */
  ST7315_FADE_INTERVAL_16FRAMES  = 0x01U, /*!<  16 frames per step */
  ST7315_FADE_INTERVAL_24FRAMES  = 0x02U, /*!<  24 frames per step */
  ST7315_FADE_INTERVAL_32FRAMES  = 0x03U, /*!<  32 frames per step */
  ST7315_FADE_INTERVAL_40FRAMES  = 0x04U, /*!<  40 frames per step */
  ST7315_FADE_INTERVAL_48FRAMES  = 0x05U, /*!<  48 frames per step */
  ST7315_FADE_INTERVAL_56FRAMES  = 0x06U, /*!<  56 frames per step */
  ST7315_FADE_INTERVAL_64FRAMES  = 0x07U, /*!<  64 frames per step */
  ST7315_FADE_INTERVAL_72FRAMES  = 0x08U, /*!<  72 frames per step */
  ST7315_FADE_INTERVAL_80FRAMES  = 0x09U, /*!<  80 frames per step */
  ST7315_FADE_INTERVAL_88FRAMES  = 0x0AU, /*!<  88 frames per step */
  ST7315_FADE_INTERVAL_96FRAMES  = 0x0BU, /*!<  96 frames per step */
  ST7315_FADE_INTERVAL_104FRAMES = 0x0CU, /*!< 104 frames per step */
  ST7315_FADE_INTERVAL_112FRAMES = 0x0DU, /*!< 112 frames per step */
  ST7315_FADE_INTERVAL_120FRAMES = 0x0EU, /*!< 120 frames per step */
  ST7315_FADE_INTERVAL_128FRAMES = 0x0FU  /*!< 128 frames per step */
} st7315_fade_interval_t;

/**
  * @brief  ST7315 driver return status
  */
typedef enum
{
  ST7315_OK = 0,
  ST7315_ERROR
} st7315_status_t;

/**
  * @brief  ST7315 driver object structure
  */
typedef struct
{
  st7315_io_t  io;             /*!< IO interface                            */
  uint8_t     *frame_buffer;   /*!< Pointer to the application frame buffer */
  uint8_t      is_initialized; /*!< Initialization flag                     */
} st7315_obj_t;

/**
  * @brief  ST7315 Exported Functions
  */
st7315_status_t st7315_init(st7315_obj_t *pobj, uint32_t dev_id);
st7315_status_t st7315_deinit(st7315_obj_t *pobj);

st7315_status_t st7315_display_on(st7315_obj_t *pobj);
st7315_status_t st7315_display_off(st7315_obj_t *pobj);
st7315_status_t st7315_inverse_display(st7315_obj_t *pobj, st7315_display_mode_t mode);
st7315_status_t st7315_set_orientation(st7315_obj_t *pobj, st7315_orientation_t orientation);
st7315_status_t st7315_set_frame_buffer(st7315_obj_t *pobj, uint8_t *frame_buffer);
st7315_status_t st7315_synchronize(st7315_obj_t *pobj);

st7315_status_t st7315_set_page(st7315_obj_t *pobj, uint8_t page);
st7315_status_t st7315_set_column(st7315_obj_t *pobj, uint8_t column);

st7315_status_t st7315_clean_ddram(st7315_obj_t *pobj);

st7315_status_t st7315_horizontal_scrolling_setup(st7315_obj_t *pobj, st7315_scroll_mode_t scroll_mode,
                                                  uint8_t start_page, uint8_t end_page,
                                                  st7315_scroll_freq_t frequency);
st7315_status_t st7315_vertical_scrolling_setup(st7315_obj_t *pobj, st7315_vscroll_dir_t dir,
                                                uint8_t start_page, uint8_t end_page,
                                                st7315_scroll_freq_t frequency, uint8_t vert_offset);
st7315_status_t st7315_scrolling_start(st7315_obj_t *pobj);
st7315_status_t st7315_scrolling_stop(st7315_obj_t *pobj);

st7315_status_t st7315_set_contrast(st7315_obj_t *pobj, uint8_t contrast);
st7315_status_t st7315_set_start_line(st7315_obj_t *pobj, uint8_t line);
st7315_status_t st7315_all_pixel_on(st7315_obj_t *pobj, st7315_all_pixel_mode_t mode);
st7315_status_t st7315_set_zoom(st7315_obj_t *pobj, st7315_zoom_mode_t mode);
st7315_status_t st7315_set_fade_blinking(st7315_obj_t *pobj, st7315_fade_blink_mode_t mode,
                                         st7315_fade_interval_t interval);

#ifdef __cplusplus
}
#endif

#endif /* ST7315_H */

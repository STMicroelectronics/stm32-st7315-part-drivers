/**
  ******************************************************************************
  * @file    st7315_io.h
  * @brief   This file contains all the functions prototypes for the
  *          st7315_io.c file.
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
#ifndef ST7315_IO_H
#define ST7315_IO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"


typedef struct
{
  uint32_t id; /*!< Instance ID */
  /* Add fields as needed to store HAL handles and other resources */
} st7315_io_t;

int32_t st7315_io_init(st7315_io_t *pio);
int32_t st7315_io_hw_init_sequence(st7315_io_t *pio);
int32_t st7315_io_write_command(st7315_io_t *pio, uint8_t cmd);
int32_t st7315_io_write_multiple_data(st7315_io_t *pio, uint8_t *pdata, uint32_t size);

#ifdef __cplusplus
}
#endif
#endif /* ST7315_IO_H */


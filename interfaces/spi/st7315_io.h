/**
  ******************************************************************************
  * @file    st7315_io.h
  * @brief   This file contains all the functions prototypes for st7315_io.c.
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


#if !defined(ST7315_SPI_TIMEOUT_VALUE)
#define ST7315_SPI_TIMEOUT_VALUE     1000U      /*!< SPI transmit timeout (1 second)   */
#endif /* ST7315_SPI_TIMEOUT_VALUE */

#define ST7315_GPIO_MAX_CHECK_COUNT    20U      /*!< max wait count for GPIO switching */

typedef struct
{
  uint32_t         id;       /*!< Instance ID - used for the resources attribution during the init sequence */
  hal_spi_handle_t *phspi;   /*!< Pointer to SPI handle                        */
  hal_gpio_t       rst_port; /*!< GPIO port used to drive the RESET pin         */
  uint32_t         rst_pin;  /*!< RESET pin                                     */
  hal_gpio_t       cs_port;  /*!< GPIO port used to drive the CS pin            */
  uint32_t         cs_pin;   /*!< CS pin                                        */
  hal_gpio_t       dc_port;  /*!< GPIO port used to drive the DC (D/C) pin      */
  uint32_t         dc_pin;   /*!< DC pin: LOW = command mode, HIGH = data mode  */
} st7315_io_t;


int32_t st7315_io_init(st7315_io_t *pio);
int32_t st7315_io_select(st7315_io_t *pio);
int32_t st7315_io_deselect(st7315_io_t *pio);
int32_t st7315_io_hw_init_sequence(st7315_io_t *pio);
int32_t st7315_io_write_command(st7315_io_t *pio, uint8_t cmd);
int32_t st7315_io_write_multiple_data(st7315_io_t *pio, uint8_t *pdata, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif /* ST7315_IO_H */

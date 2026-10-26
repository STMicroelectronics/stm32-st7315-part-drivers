# __ST7315 Part Drivers__

![tag](https://img.shields.io/badge/tag-2.0.0-brightgreen.svg)
[![release note](https://img.shields.io/badge/release_note-view_html-gold.svg)](https://htmlpreview.github.io/?https://github.com/STMicroelectronics/stm32-st7315-part-drivers/blob/main/Release_Notes.html)


## Overview

This part driver provides APIs to drive the ST7315 128×64 monochrome OLED display controller, used in OLED modules such as the GJX-2864TSWBG09.

## Description and Usage

### Peripherals initialization:

Part ST7315 assumes that the initialization of all needed peripherals (SPI, GPIOs) is done by the main application. The GPIO peripheral is usually initialized by the auto generated code produced by the STM32CubeMX tool, otherwise it is up to the user to correctly initialize the GPIO peripheral.


## How to use it?

To use the ST7315 part APIs, the application should define the `st7315_obj_t` object and assign HW resources to it.

```c
typedef struct
{
  st7315_io_t  io;             /*!< IO interface                             */
  uint8_t     *frame_buffer;   /*!< Pointer to the application frame buffer  */
  uint8_t      is_initialized; /*!< Initialization flag                      */
} st7315_obj_t;
```

The application should configure the following; here is an example using SPI IO:

```c
/** ST7315 IO obj found in st7315_io.h **/

typedef struct
{
  uint32_t         id;        /*!< Instance ID - used for resources attribution during the init sequence */
  hal_spi_handle_t *phspi;   /*!< Pointer to SPI handle                        */
  hal_gpio_t       rst_port; /*!< GPIO port used to drive the RESET pin         */
  uint32_t         rst_pin;  /*!< RESET pin                                     */
  hal_gpio_t       cs_port;  /*!< GPIO port used to drive the CS pin            */
  uint32_t         cs_pin;   /*!< CS pin                                        */
  hal_gpio_t       dc_port;  /*!< GPIO port used to drive the DC (D/C) pin      */
  uint32_t         dc_pin;   /*!< DC pin: LOW = command mode, HIGH = data mode  */
} st7315_io_t;
```

Configuration in application code:

```c
#define MX_ST7315_0     1       /* ST7315 instance ID */

st7315_obj_t         LcdObj; /* st7315 instance    */

(...)

int32_t st7315_io_init(st7315_io_t *pio)
{
  pio->phspi    = spi1_gethandle();
  pio->rst_port = HAL_GPIOA;
  pio->rst_pin  = HAL_GPIO_PIN_8;
  pio->cs_port  = HAL_GPIOA;
  pio->cs_pin   = HAL_GPIO_PIN_4;
  pio->dc_port  = HAL_GPIOA;
  pio->dc_pin   = HAL_GPIO_PIN_9;

  return 0;
}

(...)

st7315_init(&LcdObj, MX_ST7315_0);

/* Attach a frame buffer once and call synchronize when needed */
st7315_set_frame_buffer(&LcdObj, frame_buffer);
/* User application: fills frame buffer*/
(...)
/* Synchronize the display with the updated frame buffer */
st7315_synchronize(&LcdObj);
```
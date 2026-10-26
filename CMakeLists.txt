# origin-pack: STMicroelectronics::st7315_part_drivers@2.0.0
# file-format: 1.0.0
project(STMicroelectronics_st7315_part_drivers_2_0_0)
cmake_minimum_required(VERSION 3.30)
add_library(STMicroelectronics_st7315_part_drivers_2_0_0 INTERFACE)

# List of all CMSIS properties that influence conditions for this package


# Device specific defined symbols








# Enable all components in this package
if(CMSIS_ENTIRE_STMicroelectronics_st7315_part_drivers_2_0_0)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:OLED#Csub:ST7315 Core#Cversion:1.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:OLED#Csub:ST7315 IO#Cvariant:SPI Polling#Cversion:1.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:OLED#Csub:ST7315 IO#Cvariant:Custom#Cversion:1.0.0")
endif()

# All conditions used by this package

# condition: STMicroelectronics.st7315_part_drivers.2.0.0:ST7315 CFG Condition
# description: STMicroelectronics configuration for ST7315 part driver
set(STMicroelectronics.st7315_part_drivers.2.0.0_ST7315_CFG_Condition "1")
message(DEBUG "CMSIS condition STMicroelectronics.st7315_part_drivers.2.0.0_ST7315_CFG_Condition enabled")


# condition: STMicroelectronics.st7315_part_drivers.2.0.0:ST7315 Core Condition
# description: STMicroelectronics ST7315 Core Driver
set(STMicroelectronics.st7315_part_drivers.2.0.0_ST7315_Core_Condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:OLED#.*Csub:ST7315 IO(#.*|$)>,>>")
message(DEBUG "CMSIS condition STMicroelectronics.st7315_part_drivers.2.0.0_ST7315_Core_Condition enabled")


# condition: STMicroelectronics.st7315_part_drivers.2.0.0:ST7315 Custom IO Condition
# description: STMicroelectronics custom io configuration for ST7315 part driver
set(STMicroelectronics.st7315_part_drivers.2.0.0_ST7315_Custom_IO_Condition "1")
message(DEBUG "CMSIS condition STMicroelectronics.st7315_part_drivers.2.0.0_ST7315_Custom_IO_Condition enabled")


# condition: STMicroelectronics.st7315_part_drivers.2.0.0:ST7315 IO SPI Polling Condition
# description: STMicroelectronics ST7315 IO SPI Polling Driver
set(STMicroelectronics.st7315_part_drivers.2.0.0_ST7315_IO_SPI_Polling_Condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:SPI(#.*|$)>,>>")
message(DEBUG "CMSIS condition STMicroelectronics.st7315_part_drivers.2.0.0_ST7315_IO_SPI_Polling_Condition enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:OLED#Csub:ST7315 Core#Cversion:1.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # Driver for ST7315 OLED Controller
  message(DEBUG "Using component Board_Part_OLED_ST7315_Core_1_0_0")
  target_compile_definitions(STMicroelectronics_st7315_part_drivers_2_0_0 INTERFACE "$<${STMicroelectronics.st7315_part_drivers.2.0.0_ST7315_Core_Condition}:-DCMSIS_USE_Board_Part_OLED_ST7315_Core_1_0_0=1>")
  target_include_directories(STMicroelectronics_st7315_part_drivers_2_0_0 INTERFACE "$<${STMicroelectronics.st7315_part_drivers.2.0.0_ST7315_Core_Condition}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(STMicroelectronics_st7315_part_drivers_2_0_0 INTERFACE "$<${STMicroelectronics.st7315_part_drivers.2.0.0_ST7315_Core_Condition}:${CMAKE_CURRENT_LIST_DIR}/st7315.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:OLED#Csub:ST7315 IO#Cvariant:SPI Polling#Cversion:1.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # Driver for ST7315 IO SPI Polling
  message(DEBUG "Using component Board_Part_OLED_ST7315_IO_SPI_Polling_1_0_0")
  target_compile_definitions(STMicroelectronics_st7315_part_drivers_2_0_0 INTERFACE "$<${STMicroelectronics.st7315_part_drivers.2.0.0_ST7315_IO_SPI_Polling_Condition}:-DCMSIS_USE_Board_Part_OLED_ST7315_IO_SPI_Polling_1_0_0=1>")
  target_include_directories(STMicroelectronics_st7315_part_drivers_2_0_0 INTERFACE "$<${STMicroelectronics.st7315_part_drivers.2.0.0_ST7315_IO_SPI_Polling_Condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/spi>")
  target_sources(STMicroelectronics_st7315_part_drivers_2_0_0 INTERFACE "$<${STMicroelectronics.st7315_part_drivers.2.0.0_ST7315_IO_SPI_Polling_Condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/spi/st7315_io.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:OLED#Csub:ST7315 IO#Cvariant:Custom#Cversion:1.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # Custom I/O layer for the ST7315 OLED Controller
  message(DEBUG "Using component Board_Part_OLED_ST7315_IO_Custom_1_0_0")
  target_compile_definitions(STMicroelectronics_st7315_part_drivers_2_0_0 INTERFACE "$<${STMicroelectronics.st7315_part_drivers.2.0.0_ST7315_Custom_IO_Condition}:-DCMSIS_USE_Board_Part_OLED_ST7315_IO_Custom_1_0_0=1>")
  target_include_directories(STMicroelectronics_st7315_part_drivers_2_0_0 INTERFACE "$<${STMicroelectronics.st7315_part_drivers.2.0.0_ST7315_Custom_IO_Condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/custom>")
  if(EXISTS ${CMSIS_RTE_FOLDER}/Board Part/st7315_io.c)
    target_sources(STMicroelectronics_st7315_part_drivers_2_0_0 INTERFACE "$<${STMicroelectronics.st7315_part_drivers.2.0.0_ST7315_Custom_IO_Condition}:${CMSIS_RTE_FOLDER}/Board Part/st7315_io.c>")
  else()
    target_sources(STMicroelectronics_st7315_part_drivers_2_0_0 INTERFACE "$<${STMicroelectronics.st7315_part_drivers.2.0.0_ST7315_Custom_IO_Condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/custom/st7315_io.c>")
  endif()
endif()


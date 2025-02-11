add_executable(opendro-pico-firmware)

pico_generate_pio_header(opendro-pico-firmware ${CMAKE_CURRENT_SOURCE_DIR}/src/encoder.pio)

target_sources(opendro-pico-firmware PRIVATE src/main.c)

target_link_libraries(opendro-pico-firmware PRIVATE
    pico_stdlib
    pico_multicore
    hardware_i2c
    hardware_pio
    hardware_spi
    # segdisplay
)

pico_enable_stdio_usb(opendro-pico-firmware 1)
pico_enable_stdio_uart(opendro-pico-firmware 0)

pico_add_extra_outputs(opendro-pico-firmware)

option(OPENDRO_PICO_FIRMWARE_SEGDISPLAY "Whether to enable segdisplay/MAX7219 output on Pico firmware" ON)

add_executable(opendro-pico-firmware)

pico_generate_pio_header(opendro-pico-firmware ${CMAKE_CURRENT_SOURCE_DIR}/src/encoder.pio)

target_sources(opendro-pico-firmware PRIVATE src/main.c)

if (OPENDRO_PICO_FIRMWARE_SEGDISPLAY)
    target_compile_definitions(opendro-pico-firmware PRIVATE OPENDRO_PICO_FIRMWARE_SEGDISPLAY)
    target_link_libraries(opendro-pico-firmware PRIVATE
        segdisplay
    )
endif()

target_link_libraries(opendro-pico-firmware PRIVATE
    pico_stdlib
    pico_multicore
    hardware_i2c
    hardware_pio
    hardware_spi
)

pico_enable_stdio_usb(opendro-pico-firmware 1)
pico_enable_stdio_uart(opendro-pico-firmware 0)

pico_add_extra_outputs(opendro-pico-firmware)

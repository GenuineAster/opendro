add_library(segdisplay STATIC src/segdisplay.c)
target_include_directories(segdisplay PUBLIC include/)
target_link_libraries(segdisplay PRIVATE OpenDROPlatform)

if (OPENDRO_PLATFORM_PICO)
    target_link_libraries(segdisplay PRIVATE
        pico_stdlib
        hardware_spi
    )
endif()

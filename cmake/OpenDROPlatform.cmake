# create interface target so that we can cleanly pass platform info to
# targets that care about it
add_library(OpenDROPlatform INTERFACE)

if (OPENDRO_TARGET_PICO)
    set(OPENDRO_PLATFORM_PICO TRUE)
    set(OPENDRO_PLATFORM_HAS_GRAPHICAL_OS FALSE)
    target_compile_definitions(OpenDROPlatform INTERFACE
        OPENDRO_PLATFORM_PICO
    )
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(OPENDRO_PLATFORM_WINDOWS TRUE)
    set(OPENDRO_PLATFORM_HAS_GRAPHICAL_OS TRUE)
    target_compile_definitions(OpenDROPlatform INTERFACE
        OPENDRO_PLATFORM_WINDOWS
        OPENDRO_PLATFORM_HAS_GRAPHICAL_OS
    )
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux" AND "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64")
    # not perfect RPi detection but we are only targeting one arm64 Linux device
    set(OPENDRO_PLATFORM_RPI TRUE)
    set(OPENDRO_PLATFORM_LINUX TRUE)
    set(OPENDRO_PLATFORM_HAS_GRAPHICAL_OS TRUE)
    target_compile_definitions(OpenDROPlatform INTERFACE
        OPENDRO_PLATFORM_RPI
        OPENDRO_PLATFORM_LINUX
        OPENDRO_PLATFORM_HAS_GRAPHICAL_OS
    )
endif()

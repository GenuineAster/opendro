cmake_minimum_required(VERSION 3.25)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

option(OPENDRO_TARGET_PICO "Whether this build should build firmware for the Raspberry Pi Pico target" OFF)

if (OPENDRO_TARGET_PICO)
    set(PICO_BOARD pico2 CACHE STRING "Board type")
    include(pico_sdk_import)
endif()

project(opendro LANGUAGES C CXX)

if (OPENDRO_TARGET_PICO)
    pico_sdk_init()
endif()

include(OpenDROPlatform)

if (OPENDRO_PLATFORM_HAS_GRAPHICAL_OS)
    include(ConfigureSDL)
endif()

add_subdirectory(shared)

if (OPENDRO_PLATFORM_HAS_GRAPHICAL_OS)
    add_subdirectory(software)
endif()

if (OPENDRO_PLATFORM_PICO)
    add_subdirectory(firmware)
endif()

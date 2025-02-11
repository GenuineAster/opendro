set(SDL_INSTALL OFF)
set(SDL_ASSERTIONS "disabled")
set(SDL_RPI ON)
set(SDL_TEST_LIBRARY OFF)
set(SDL_TESTS OFF)
set(SDL_EXAMPLES OFF)
set(SDL_AUDIO OFF)
set(SDL_VIDEO ON)
set(SDL_CAMERA OFF)
set(SDL_HAPTIC OFF)
set(SDL_DIALOG OFF)
set(SDL_HIDAPI OFF)
set(SDL_SENSOR OFF)
set(SDL_JOYSTICK OFF)
set(SDL_OPENGL OFF)
set(SDL_VULKAN OFF)
set(SDL_OFFSCREEN OFF)
set(SDL_DUMMYVIDEO OFF)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux" AND "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64")
    set(SDL_CPU_ARM64 ON)
    set(UNIX_SYS ON)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
add_subdirectory(${CMAKE_SOURCE_DIR}/ext/sdl EXCLUDE_FROM_ALL)

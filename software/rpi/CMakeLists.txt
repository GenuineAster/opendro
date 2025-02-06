add_library(SDL3 STATIC IMPORTED)
target_include_directories(SDL3 INTERFACE $ENV{SDLDIR}/include)
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	set_target_properties(SDL3 PROPERTIES
		IMPORTED_LOCATION $ENV{SDLDIR}/lib/x64/SDL3.lib
	)
endif()

add_executable(opendro-rpi-gui src/main.c src/dro.c src/ui.c src/config.c)
target_link_libraries(opendro-rpi-gui SDL3)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	add_custom_command(TARGET opendro-rpi-gui
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $ENV{SDLDIR}/lib/x64/SDL3.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/SDL3.dll
		PRE_BUILD
	)
endif()

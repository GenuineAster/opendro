add_executable(opendro-rpi-gui src/main.c src/dro.c src/ui.c src/config.c src/keypad.c src/calculator.c)
target_link_libraries(opendro-rpi-gui SDL3::SDL3)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	add_custom_command(TARGET opendro-rpi-gui
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $ENV{SDLDIR}/lib/x64/SDL3.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/SDL3.dll
		PRE_BUILD
	)
endif()

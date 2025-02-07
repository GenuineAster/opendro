add_executable(opendro-rpi-gui src/main.c src/dro.c src/ui.c src/config.c src/keypad.c src/calculator.c src/memory_store.c)
target_link_libraries(opendro-rpi-gui SDL3::SDL3)

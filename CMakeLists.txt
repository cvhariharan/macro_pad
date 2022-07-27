cmake_minimum_required(VERSION 3.13)
include(pico-sdk/pico_sdk_init.cmake)

project(macro_pad)
pico_sdk_init()

add_library(matrix_keypad STATIC ../matrix_keypad.c)
target_link_libraries(matrix_keypad
    pico_stdlib
)

add_executable(macro_pad main.c)
target_link_libraries(macro_pad pico_stdlib matrix_keypad)

pico_enable_stdio_usb(macro_pad 1)
pico_enable_stdio_uart(macro_pad 0)

pico_add_extra_outputs(macro_pad)
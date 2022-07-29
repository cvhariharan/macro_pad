cmake_minimum_required(VERSION 3.13)
include(pico-sdk/pico_sdk_init.cmake)

project(macro_pad)
pico_sdk_init()


add_executable(macro_pad)

add_library(matrix_keypad STATIC matrix_keypad.c)
target_link_libraries(matrix_keypad
    pico_stdlib
)

target_sources(macro_pad PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
        )

target_include_directories(macro_pad PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(macro_pad PUBLIC pico_stdlib tinyusb_device tinyusb_board matrix_keypad)

pico_add_extra_outputs(macro_pad)
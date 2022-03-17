cmake_minimum_required(VERSION 3.13)

include(lib/pico-sdk/pico_sdk_init.cmake)

project(picotronix_firmware)

pico_sdk_init()

add_subdirectory(src)

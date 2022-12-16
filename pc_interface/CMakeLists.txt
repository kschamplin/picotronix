cmake_minimum_required(VERSION 3.13)

project(picotronix_gui)

find_package(SDL2 REQUIRED)

add_executable(picogui)

target_sources(picogui PRIVATE src/main.cpp)
target_link_libraries(picogui PUBLIC STM32)


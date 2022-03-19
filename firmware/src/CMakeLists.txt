add_executable(blinky blinky.c)

target_link_libraries(blinky pico_stdlib)
pico_add_extra_outputs(blinky)


add_subdirectory(pwm)
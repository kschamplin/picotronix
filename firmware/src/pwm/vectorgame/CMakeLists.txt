add_executable(vectorgame main.cpp collectible.cpp path_group.cpp path.cpp ship.cpp turret.cpp)

target_link_libraries(vectorgame pico_stdlib hardware_pwm hardware_irq)
pico_add_extra_outputs(vectorgame)

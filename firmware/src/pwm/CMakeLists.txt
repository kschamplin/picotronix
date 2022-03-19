add_executable(pwmtest pwmtest.c)

target_link_libraries(pwmtest pico_stdlib hardware_pwm hardware_irq)
pico_add_extra_outputs(pwmtest)

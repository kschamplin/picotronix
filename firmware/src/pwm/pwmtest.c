// short pwm test for use with scope.


#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"
#define OUTPIN 0
// IRQ handler for pwm wrap.
void pwm_increase() {
    static int value = 0;

    // pwm_clear_irq(pwm_gpio_to_slice_num(0));

    if (value++ > 15) {
        value = 0;
    }

    pwm_set_gpio_level(OUTPIN, value);
}

int main() {
    // setup pwm peripheral.

    gpio_set_function(OUTPIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(OUTPIN);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_wrap(&config, 15);
    // pwm_set_wrap(slice_num, 63); // 4 bits, so top = 16 - 1

    pwm_init(slice_num, &config, true);

    while (true) {
        sleep_ms(25);
        pwm_increase();
    }

}
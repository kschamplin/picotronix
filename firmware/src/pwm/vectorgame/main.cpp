/* 
 *  EE 1301
 *  vectorgame.ino
 *  Jack Sweeney
 *  sween428
 */

#include <cmath>
#include <algorithm>
#include <cstdint>
#include "path.h"
#include "path_group.h"
#include "turret.h"
#include "ship.h"
#include "collectible.h"


#include "pico/stdlib.h"
#include "hardware/pwm.h"

void init_pwm() {
	gpio_set_function(0, GPIO_FUNC_PWM);
    // gpio_set_function(1, GPIO_FUNC_PWM);
    gpio_set_function(2, GPIO_FUNC_PWM);
    // gpio_set_function(3, GPIO_FUNC_PWM);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_wrap(&config, 255);
    // pwm_set_wrap(slice_num, 15); // 4 bits, so top = 16 - 1

    pwm_init(0, &config, true);
    pwm_init(1, &config, true);
}
// Path and group for the "calibrate" home screen of crossed square and circle
// used for ensuring the oscilloscope settings are correct
path_t calibrate_path[10] = {
	{path_t::LINE,0,0,4095,4095},
	{path_t::LINE,4095,0,0,4095},
	{path_t::LINE,0,0,0,4095},
	{path_t::LINE,0,0,4095,0},
	{path_t::LINE,0,4095,4095,4095},
	{path_t::LINE,4095,0,4095,4095},
	{path_t::CUBIC_BEZIER,2048,3072,2612,3072,3072,2612,3072,2048},
	{path_t::CUBIC_BEZIER,3072,2048,3072,1483,2612,1024,2048,1024},
	{path_t::CUBIC_BEZIER,2048,1024,1483,1024,1024,1483,1024,2048},
	{path_t::CUBIC_BEZIER,1024,2048,1024,2612,1483,3072,2048,3072}
};
path_group_t calibrate(calibrate_path, 10, 2048, 2048);

ship_t ship; // Player controlled ship

turret_t turret; // Single "attacking" turret

collectible_t collectible; // Single objective collectible

int main() {
	// gfx::gfxinit(1024, 1024);
	init_pwm();
	gpio_init(16);
	gpio_set_dir(16, GPIO_IN);
	gpio_pull_up(16);
	
	srand(0);

	while (true) {
		static bool in_game = false; // Game state

		static int current_score = 0;

		//static unsigned long int next_logic_time = 0; // Next time logic computed
		//unsigned long int current_time = millis();

		static int previous_thrust = 0; // For edge detection
		int current_thrust = !gpio_get(16);

		// gfx::clearScreen(0,0,0);
		if (in_game) {
			//if (current_time >= next_logic_time) {
				// Update game logic
				ship.update();
				collectible.update();
				if (turret.update()) {
					// Reset position when finished with current volley
					turret.start(rand() % 4096, rand() % 4096, 10, 15);
				}

				// Poll controls
				/*if (input::getKeyState(SDLK_d)) {
					ship.rotate(M_PI/16);
				} else if (input::getKeyState(SDLK_a)) {
					ship.rotate(-M_PI/16);
				} */
				if (!gpio_get(16)) {
					ship.thrust(50);
				}

				// Check for collectible collection
				if (collectible.check_collision(ship.get_x(),
								ship.get_y(), 500)) {
					// Move collectible to random position
					collectible.move(rand() % (3800 - 300) + 300, rand() % (3800 - 300) + 300);
					current_score++; // Increase score
				}

				// If killed,
				if (false) {
					while (!turret.update()) {}; // Finish turret cycle

					// Reset ship and collectible
					ship.reset();

					collectible.move(rand() % (3800 - 300) + 300, rand() % (3800 - 300) + 300);

					in_game = false;
				}
				//next_logic_time += 50; // Game logic updates 20 times per second
			//}

			// Draw game as fast as possible
			turret.draw();
			ship.draw();
			collectible.draw();
		} else { // "Calibrate" home screen
			calibrate.draw();
			// On press start game
			if (previous_thrust == false && current_thrust == true) {
				in_game = true;
				sleep_ms(100);
			}
		}
		previous_thrust = current_thrust;
		sleep_ms(8);
	}
}

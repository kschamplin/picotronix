/* 
 *  EE 1301
 *  collectible.ino
 *  Jack Sweeney
 *  sween428
 */

#include <cmath>
#include <cstdint>
#include <stdlib.h>
#include "collectible.h"

collectible_t::collectible_t() {
	// Shrink because the collectible was drawn for the entire screen
	collectible.scale(0.1);
	// Move to initial random location (with margins)
	collectible.move(rand() % (3800 - 300) + 300, rand() % (3800 - 300) + 300);
}

void collectible_t::update() {
	collectible.rotate(M_PI/4);
}

void collectible_t::draw() {
	collectible.draw();
}

void collectible_t::move(int16_t x, int16_t y) {
	collectible.move(x, y);
}

bool collectible_t::check_collision(int16_t x, int16_t y, int16_t r) {
	// Distance formula
	return (sqrt(pow(collectible.get_x() - x, 2) +
	             pow(collectible.get_y() - y, 2)) <= r);
}

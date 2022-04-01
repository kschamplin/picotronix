/* 
 *  EE 1301
 *  collectible.h
 *  Jack Sweeney
 *  sween428
 */

#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include "path_group.h"
#include <cstdint>

// Class for the "collectible" which the ship collects for points
// Class holds basic data and animation/helper functions
class collectible_t {
private:
	// Path for a square
	path_t collectible_path[4] = {
		{path_t::LINE,0,0,0,4095},
		{path_t::LINE,0,4095,4095,4095},
		{path_t::LINE,4095,4095,4095,0},
		{path_t::LINE,4095,0,0,0}
	};
	path_group_t collectible = {collectible_path, 4, 2048, 2048};
public:
	// Performs rotation animation
	void update();
	// Draw to screen
	void draw();
	// Moves to absolute x, y coordinate
	void move(int16_t x, int16_t y);
	// Checks if x, y is radius r from center of collectible
	bool check_collision(int16_t x, int16_t y, int16_t r);

	// Constructor
	collectible_t();
};

#endif // end COLLECTIBLE_H header guard

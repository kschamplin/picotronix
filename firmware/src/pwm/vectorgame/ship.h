/* 
 *  EE 1301
 *  ship.h
 *  Jack Sweeney
 *  sween428
 */

#ifndef SHIP_H
#define SHIP_H

#include "path_group.h"
#include <cstdint>

// Class for the player controlled spaceship
class ship_t {
private:
	// Path array describing ship shape
	path_t ship_path[4] = {
		{path_t::QUAD_BEZIER,0,0,2048,4095*2,4095,0},
		{path_t::LINE,4095,0,2048,1024},
		{path_t::LINE,2048,1024,0,0},
		{path_t::QUAD_BEZIER,1024,512,2048,-2048,3072,512} // Thrust
	};
	path_group_t ship = {ship_path, 4, 2048, 2048}; // Start in center
	int16_t vx, vy; // Ship velocities
	float angle; // Angle of ship from 0 to 2*pi radians
public:
	// Updates velocity of ship
	void update();
	// Draws ship
	void draw();
	// Adds more thrust (velocity) in direction of angle. Must be run after
	// update() and rotate() for thrust to render correctly
	void thrust(float multiplier);
	// Rotates ship by additional theta radians
	void rotate(float theta);
	// Returns the path_group_t's center_X
	int16_t get_x();
	// Returns the path_group_t's cneter_y
	int16_t get_y();
	// Resets ship's position, velocity, and angle
	void reset();

	// Constructor
	ship_t();
};

#endif // end SHIP_H header guard

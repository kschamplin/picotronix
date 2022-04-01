/* 
 *  EE 1301
 *  ship.ino
 *  Jack Sweeney
 *  sween428
 */

#include <cmath>
#include <cstdint>
#include "ship.h"

#define MAX_VELOCITY 1024

ship_t::ship_t() {
	vx = 0;
	vy = 0;
	angle = M_PI/2;
	ship.scale(0.1); // Shrink ship because it's drawn for the entire screen
}

void ship_t::update() {
	// Turn on thrust before translation so it processes correctly
	ship_path[3].type = path_t::QUAD_BEZIER;
	ship.translate(vx, vy);
	ship_path[3].type = path_t::HIDDEN;
}

void ship_t::draw() {
	ship.draw();
}

void ship_t::thrust(float multiplier) {
	// Show thrust
	ship_path[3].type = path_t::QUAD_BEZIER;
	vx += (int16_t)(multiplier * cos(angle));
	vy += (int16_t)(multiplier * sin(angle));
	// Bounds check the velocity
	if (vx >=  MAX_VELOCITY) vx =  MAX_VELOCITY;
	if (vx <= -MAX_VELOCITY) vx = -MAX_VELOCITY;
	if (vy >=  MAX_VELOCITY) vy =  MAX_VELOCITY;
	if (vy <= -MAX_VELOCITY) vy = -MAX_VELOCITY;
}

void ship_t::rotate(float theta) {
	// Turn on thrust before rotation so it processes correctly
	ship_path[3].type = path_t::QUAD_BEZIER;
	ship.rotate(theta);
	ship_path[3].type = path_t::HIDDEN;
	angle += theta;
	if (angle >= 2*M_PI) angle -= 2*M_PI;
}

int16_t ship_t::get_x() {
	return ship.get_x();
}

int16_t ship_t::get_y() {
	return ship.get_y();
}

void ship_t::reset() {
	// Turn on thrust before so it processes correctly
	ship_path[3].type = path_t::QUAD_BEZIER;
	ship.move(2048, 2048); // Start in middle
	rotate(M_PI/2 - angle); // Start with angle of pi/2
	ship_path[3].type = path_t::HIDDEN;
	// Zero velocity
	vx = 0;
	vy = 0;
}

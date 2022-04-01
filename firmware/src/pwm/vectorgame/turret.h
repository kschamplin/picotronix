/* 
 *  EE 1301
 *  turret.h
 *  Jack Sweeney
 *  sween428
 */

#ifndef TURRET_H
#define TURRET_H

#include "path_group.h"
#include <cstdint>

// Class for the "turrets" which fire the bullets with a warning animation
// of a half circle rotating inwards
class turret_t {
private:
	// Path array describing shape of the half "warning" circle for bullets
	path_t spawn_circle_path[2] = {
		{path_t::CUBIC_BEZIER,2048,3072,2612,3072,3072,2612,3072,2048},
		{path_t::CUBIC_BEZIER,2048,1024,1483,1024,1024,1483,1024,2048}
	};
	path_group_t spawn_circle = {spawn_circle_path, 2, 2048, 2048};
	// Array of the eight bullets for each 45 degree direction
	path_t bullets[8] = {
		{path_t::POINT,2048,2048,1000},
		{path_t::POINT,2048,2048,1000},
		{path_t::POINT,2048,2048,1000},
		{path_t::POINT,2048,2048,1000},
		{path_t::POINT,2048,2048,1000},
		{path_t::POINT,2048,2048,1000},
		{path_t::POINT,2048,2048,1000},
		{path_t::POINT,2048,2048,1000}
	};

	// Stage of the turret state
	enum stage : uint8_t {
		COMPLETE, // Completed, do nothing
		APPROACH, // Warning circle stage
		BULLETS   // Bullets are spreading
	} stage = COMPLETE;
	uint16_t speed_bullets; // Speed of bullets per tick
	uint16_t approach_length; // Length of warning period
public:
	// Start turret in position x, y with bullet speed speed_bullets
	// and warning length approach_length
	void start(int x, int y, int speed_bullets, int approach_length);
	// Update animations
	bool update();
	// Draw warning or bullets
	void draw();
	// Check whether x, y is radius r from any of the bullets
	bool check_bullet_collision(int16_t x, int16_t y, int16_t r);
};

#endif // end TURRET_H header guard

/* 
 *  EE 1301
 *  turret.ino
 *  Jack Sweeney
 *  sween428
 */

#include <cmath>
#include <cstdint>
#include <new>
#include "turret.h"
#include "path.h"

void turret_t::start(int x, int y, int speed_bullets, int approach_length) {
	this->speed_bullets = speed_bullets;
	this->approach_length = approach_length;

	// Reset spawn_circle in place
	spawn_circle_path[0].~path_t();
	spawn_circle_path[1].~path_t();
	new (&spawn_circle_path[0]) path_t(path_t::CUBIC_BEZIER,
	                                   2048,3072,2612,3072,
	                                   3072,2612,3072,2048);
	new (&spawn_circle_path[1]) path_t(path_t::CUBIC_BEZIER,
	                                   2048,1024,1483,1024,
	                                   1024,1483,1024,2048);
	spawn_circle.~path_group_t();
	new (&spawn_circle) path_group_t(spawn_circle_path, 2, 2048, 2048);

	// Move the spawn_circle and all bullets to x, y
	spawn_circle.move(x, y);
	for (int i = 0; i < 8; i++) {
		bullets[i].type = path_t::POINT;
		point_t point = bullets[i].get_p1();
		bullets[i].translate(x - point.x, y - point.y);
	}
	stage = turret_t::APPROACH;
}

bool turret_t::update() {
	switch (stage) {
	case turret_t::BULLETS:{
		bool completed = true;
		// Base "circular" rates for the 8 bullets
		int direction[8][2] = {{ -7, 7},{0, 10},{ 7, 7},
		                       {-10, 0},        {10, 0},
		                       { -7,-7},{0,-10},{ 7,-7}};
		for (int i = 0; i < 8; i++) {
			if (bullets[i].type == path_t::POINT) {
				completed = false;
				bullets[i].translate(speed_bullets*direction[i][0],
				                     speed_bullets*direction[i][1]);

				// Hide when out of bounds
				point_t point = bullets[i].get_p1();
				if (point.x < 0 || point.x > 4095 ||
				    point.y < 0 || point.y > 4095) {
					bullets[i].type = path_t::HIDDEN;
				}
			}
		}
		if (completed) stage = turret_t::COMPLETE;
		return completed;}
	case turret_t::APPROACH:
		spawn_circle.rotate(M_PI/16);
		// Shrink linearly
		spawn_circle.scale((float)approach_length/(approach_length + 1));
		approach_length--;
		if (approach_length <= 1) {
			stage = turret_t::BULLETS;
		}
		return false;
	}
	return true;
}

void turret_t::draw() {
	switch (stage) {
	case turret_t::BULLETS:
		for (int i = 0; i < 8; i++) {
			bullets[i].draw();
		}
		break;
	case turret_t::APPROACH:
		spawn_circle.draw();
		break;
	}
}

bool turret_t::check_bullet_collision(int16_t x, int16_t y, int16_t r) {
	if (stage == turret_t::BULLETS) {
		for (int i = 0; i < 8; i++) {
			if (bullets[i].type != path_t::HIDDEN) {
				// Distance formula
				point_t point = bullets[i].get_p1();
				if (sqrt(pow(point.x - x, 2) +
					 pow(point.y - y, 2)) <= r) {
					return true;
				}
			}
		}
	}
	return false;
}

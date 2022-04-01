/* 
 *  EE 1301
 *  path_group.ino
 *  Jack Sweeney
 *  sween428
 */

#include "path_group.h"
#include <cmath>
#include <cstdint>

path_group_t::path_group_t(path_t path_array[], size_t size,
                           int16_t center_x, int16_t center_y) {
	this->path_array = path_array;
	this->size = size;
	this->center_x = center_x;
	this->center_y = center_y;
}

void path_group_t::draw() {
	for (int i = 0; i < size; i++) {
		path_array[i].draw();
	}
}

void path_group_t::linear_map(float m11, float m12, float m21, float m22) {
	for (int i = 0; i < size; i++) {
		path_array[i].linear_map(m11, m12, m21, m22,
		                         center_x, center_y);
	}
}

void path_group_t::rotate(float theta) {
	// Standard rotation matrix
	linear_map(cos(theta), -sin(theta),
	           sin(theta),  cos(theta));
}

void path_group_t::scale(float scaling_factor) {
	// Scaling matrix
	linear_map(scaling_factor, 0, 0, scaling_factor);
}

void path_group_t::translate(int x, int y) {
	center_x += x;
	center_y += y;
	int offset_x = 0, offset_y = 0;

	// Roll-over when out-of-bounds
	if (center_x < 0) {
		center_x += 4096;
		offset_x = 4096;
	} else if (center_x > 4095) {
		center_x -= 4096;
		offset_x = -4096;
	}
	if (center_y < 0) {
		center_y += 4096;
		offset_y = 4096;
	} else if (center_y > 4095) {
		center_y -= 4096;
		offset_y = -4096;
	}
	for (int i = 0; i < size; i++) {
		path_array[i].translate(x + offset_x, y + offset_y);
	}

}

void path_group_t::move(int x, int y) {
	translate(x - center_x, y - center_y);
}

int16_t path_group_t::get_x() {
	return center_x;
}

int16_t path_group_t::get_y() {
	return center_y;
}

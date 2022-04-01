/* 
 *  EE 1301
 *  path_group.h
 *  Jack Sweeney
 *  sween428
 */

#ifndef PATH_GROUP_H
#define PATH_GROUP_H

#include "path.h"
#include <cstdint>
#include <stddef.h>

// Wrapper class for path_t. Contains a pointer to an array of path_ts to make
// an object. Keeps track of a common center for transforms and collision.
// Includes convenience functions which wrap linear_map
class path_group_t {
private:
	path_t *path_array; // Pointer to array of path_ts for object
	size_t size;        // Number of elements in path_array
	int16_t center_x;   // Center X of object for transforms and reporting
	int16_t center_y;   // Center Y of object for transforms and reporting
public:
	// Draw group of paths
	void draw();
	// Perform linear map on group around center
	void linear_map(float m11, float m12, float m21, float m22);
	// Rotate group theta radians around center
	void rotate(float theta);
	// Scale group by scaling_factor around center
	void scale(float scaling_factor);
	// Translate group by x and y units
	void translate(int x, int y);
	// Move group to absolute x and y position
	void move(int x, int y);
	// Return center_x
	int16_t get_x();
	// Return center_y
	int16_t get_y();

	// Constructor
	path_group_t(path_t path_array[], size_t size,
	             int16_t center_x, int16_t center_y);
};

#endif // end PATH_GROUP_H header guard

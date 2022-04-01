/* 
 *  EE 1301
 *  path.h
 *  Jack Sweeney
 *  sween428
 */

#ifndef PATH_H
#define PATH_H

// Marcos to write to memory addresses directly
//#define DATA_8(addr)  (*(volatile uint8_t  *)(addr))
//#define DATA_16(addr) (*(volatile uint16_t *)(addr))
//#define DATA_32(addr) (*(volatile uint32_t *)(addr))
// Macro to write to both DACs simultaneously since analogWrite() is slow 
//#define DUAL_DACS(point) DATA_32(0x40007400 + 0x20) = (point)

#include <cstdint>

// Uncomment to swap the X and Y axis
//#define SWAP_XY

// 2D point. Includes "xy" for crude (read undefined per the standard)
// "type conversion." Necessary to write to the dual DACs efficiently
#ifndef SWAP_XY
typedef union {
	struct {
		int16_t x;
		int16_t y;
	};
	int32_t xy;
} point_t;
#else
typedef union {
	struct {
		int16_t y;
		int16_t x;
	};
	int32_t xy;
} point_t;
#endif

// Extra points required for QUAD_BEZIER
typedef struct {
	point_t p2;
	point_t p3;
} quad_bezier_extra_t;

// Extra points required for CUBIC_BEZIER
typedef struct {
	point_t p2;
	point_t p3;
	point_t p4;
} cubic_bezier_extra_t;

class path_t {
public:
	// Type of path/curve/error
	enum type : uint8_t {
		LINE,         // Line segment
		ARC,          // Arc of circle, not recommended (use beziers)
		POINT,        // Single point of brightness
		QUAD_BEZIER,  // Quadratic Bezier curve
		CUBIC_BEZIER, // Cubic Bezier curve
		HIDDEN,       // Purposely not shown
		WRONG_TYPE,   // Constructor/type mismatch
		MALLOC_FAIL,  // Failed to allocate memory
		ERROR         // Generic error
	} type;
private:
	// Starting point for LINE, QUAD_BEZIER, and CUBIC_BEZIER
	// Center of ARC, location of POINT
	point_t p1;
	union {
		// LINE's ending point
		point_t p2;
		// ARC's parameters
		struct {
			uint16_t r;          // Radius
			uint8_t start_angle; // Start angle in [0,255]
			uint8_t end_angle;   // End angle in [0,255]
		};
		// POINT's brightness in microseconds drawn on screen
		uint32_t brightness;
		// Pointer to extra points needed to define QUAD_BEZIER
		quad_bezier_extra_t *quad_bezier;
		// Pointer to extra points needed to define CUBIC_BEZIER
		cubic_bezier_extra_t *cubic_bezier;
	};

	// Applies a linear map to the point point using a matrix m
	// with entries m11, m12, m21, m22 around center_x/y
	void linear_map_point(point_t *point,
			      float m11, float m12, 
			      float m21, float m22,
			      int center_x, int center_y);
public:
	void draw(); // Draw path to screen

	// Applies a linear map to the path using a matrix m with
	// entries m11, m12, m21, m22 around center center_x, center_y
	void linear_map(float m11, float m12, float m21, float m22,
	                int center_x, int center_y);
	// Translates path by x and y units
	void translate(int x, int y);

	// Returns p1 of path
	point_t get_p1();

	// LINE constructor
	path_t(enum type type, int x1, int y1, int x2, int y2);
	// ARC constructor
	path_t(enum type type, int x, int y, int r,
	       int start_angle, int end_angle);
	// POINT constructor
	path_t(enum type type, int x, int y, uint32_t brightness);
	// QUAD_BEZIER constructor
	path_t(enum type type, int x1, int y1, int x2, int y2,
	       int x3, int y3);
	// CUBIC_BEZIER constructor
	path_t(enum type type, int x1, int y1, int x2, int y2,
	       int x3, int y3, int x4, int y4);

	~path_t();
};

#endif // end PATH_H header guard

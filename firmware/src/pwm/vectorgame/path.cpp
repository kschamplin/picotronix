/* 
 *  EE 1301
 *  path.ino
 *  Jack Sweeney
 *  sween428
 */

#include <cmath>
#include <cstdint>
#include <new>
#include "path.h"

#include "pico/stdlib.h"
#include "hardware/pwm.h"

void set_output(unsigned char x, unsigned char y) {
    pwm_set_chan_level(0, PWM_CHAN_A, x);
    // pwm_set_chan_level(0, PWM_CHAN_B, x >> 4);
    pwm_set_chan_level(1, PWM_CHAN_A, y);
    // pwm_set_chan_level(1, PWM_CHAN_B, y >> 4);
	sleep_us(1);
}

// Constructors/destructor
path_t::path_t(enum type type, int x1, int y1, int x2, int y2) {
	if (type == LINE) {
		this->type = type;
		this->p1.x = x1;
		this->p1.y = y1;
		this->p2.x = x2;
		this->p2.y = y2;
	} else {
		this->type = WRONG_TYPE;
	}
}

path_t::path_t(enum type type, int x, int y, int r, int start_angle,
               int end_angle) {
	if (type == ARC) {
		this->type = type;
		this->p1.x = x;
		this->p1.y = y;
		this->r = r;
		this->start_angle = start_angle;
		this->end_angle = end_angle;
	} else {
		this->type = WRONG_TYPE;
	}
}

path_t::path_t(enum type type, int x, int y, uint32_t brightness) {
	if (type == POINT) {
		this->type = type;
		this->p1.x = x;
		this->p1.y = y;
		this->brightness = brightness;
	} else {
		this->type = WRONG_TYPE;
	}
}

path_t::path_t(enum type type, int x1, int y1, int x2, int y2,
               int x3, int y3) {
	if (type == QUAD_BEZIER) {
		this->type = type;
		this->p1.x = x1;
		this->p1.y = y1;
		this->quad_bezier = new (std::nothrow) quad_bezier_extra_t;
		if (this->quad_bezier != nullptr) {
			this->quad_bezier->p2.x = x2;
			this->quad_bezier->p2.y = y2;
			this->quad_bezier->p3.x = x3;
			this->quad_bezier->p3.y = y3;
		} else {
			this->type = MALLOC_FAIL;
		}
	} else {
		this->type = WRONG_TYPE;
	}
}

path_t::path_t(enum type type, int x1, int y1, int x2, int y2, int x3, int y3,
               int x4, int y4) {
	if (type == CUBIC_BEZIER) {
		this->type = type;
		this->p1.x = x1;
		this->p1.y = y1;
		this->cubic_bezier = new (std::nothrow) cubic_bezier_extra_t;
		if (this->cubic_bezier != nullptr) {
			this->cubic_bezier->p2.x = x2;
			this->cubic_bezier->p2.y = y2;
			this->cubic_bezier->p3.x = x3;
			this->cubic_bezier->p3.y = y3;
			this->cubic_bezier->p4.x = x4;
			this->cubic_bezier->p4.y = y4;
		} else {
			this->type = MALLOC_FAIL;
		}
	} else {
		this->type = WRONG_TYPE;
	}
}

path_t::~path_t() {
	// Free based on type since the union means we can't rely on nullptr
	switch (type) {
	case path_t::QUAD_BEZIER:
		delete quad_bezier;
		break;
	case path_t::CUBIC_BEZIER:
		delete cubic_bezier;
		break;
	}
}

void path_t::draw() {
	point_t point;

	switch (type) {
	case path_t::LINE:{
		// Determine number of points needed
		int range_x = abs(p2.x - p1.x);
		int range_y = abs(p2.y - p1.y);
		int res = range_x > range_y ? range_x : range_y;

		for (int i = 0; i <= res; i +=16) {
			// Linear interpolation 
			point.x = ((res - i)*p1.x + i*p2.x)/res;
			point.y = ((res - i)*p1.y + i*p2.y)/res;

			//DUAL_DACS(point.xy);
			// gfx::drawPoint(1024-point.x/4, 1024-point.y/4, 255, 255, 255);
			set_output(point.x/16, point.y/16);
		}
		}break;
	case path_t::ARC:
		for (int angle = start_angle; angle <= end_angle; angle++) {
			// Standard parameterization of the circle
			point.x = (int16_t)(r * cos(angle * M_PI/128) + p1.x);
			point.y = (int16_t)(r * sin(angle * M_PI/128) + p1.y);

			//DUAL_DACS(point.xy);
			
			// gfx::drawPoint(1024-point.x/4, 1024-point.y/4, 255, 255, 255);
			set_output(point.x/16, point.y/16);
		}
		break;
	case path_t::POINT:
		//DUAL_DACS(p1.xy);
		// gfx::drawPoint(1024-p1.x/16, 1024-p1.y/4, 255, 255, 255);
			set_output(point.x/16, point.y/16);
		sleep_us(brightness);
		break;
	case path_t::QUAD_BEZIER:{
		point_t p2 = quad_bezier->p2;
		point_t p3 = quad_bezier->p3;

		// Determine number of points needed
		int range_x = abs(p3.x - p1.x);
		int range_y = abs(p3.y - p1.y);
		int res = range_x > range_y ? range_x : range_y;

		for (int i = 0; i <= res; i += 16) {
			// Definition of the quadratic Bezier curve
			// Operations ordered to avoid overflows
			point.x = (res - i)*(res - i)/res*p1.x/res
			        + 2*(res - i)*p2.x/res*i/res
			        + i*p3.x/res*i/res;
			point.y = (res - i)*(res - i)/res*p1.y/res
			        + 2*(res - i)*p2.y/res*i/res
			        + i*p3.y/res*i/res;

			//DUAL_DACS(point.xy);
			set_output(point.x/16, point.y/16);
			// gfx::drawPoint(1024-point.x/4, 1024-point.y/4, 255, 255, 255);
		}
		}break;
	case path_t::CUBIC_BEZIER:{
		point_t p2 = cubic_bezier->p2;
		point_t p3 = cubic_bezier->p3;
		point_t p4 = cubic_bezier->p4;

		// Determine number of points needed
		int range_x = abs(p4.x - p1.x);
		int range_y = abs(p4.y - p1.y);
		int res = range_x > range_y ? range_x : range_y;

		for (int i = 0; i <= res; i +=16) {
			// Definition of the cubic Bezier curve
			// Operations ordered to avoid overflows
			point.x = (res - i)*(res - i)/res*p1.x/res*(res - i)/res
			        + 3*(res - i)*p2.x/res*(res - i)/res*i/res
			        + 3*(res - i)*p3.x/res*i/res*i/res
			        + i*p4.x/res*i/res*i/res;

			point.y = (res - i)*(res - i)/res*p1.y/res*(res - i)/res
			        + 3*(res - i)*p2.y/res*(res - i)/res*i/res
			        + 3*(res - i)*p3.y/res*i/res*i/res
			        + i*p4.y/res*i/res*i/res;

			//DUAL_DACS(point.xy);
			// gfx::drawPoint(1024-point.x/4, 1024-point.y/4, 255, 255, 255);
			set_output(point.x/16, point.y/16);
		}
		}break;
	}
}

void path_t::linear_map_point(point_t *point,
                              float m11, float m12, float m21, float m22,
                              int center_x, int center_y) {
	point_t temp = *point;
	// Definition of linear transformation around a point
	point->x = (int16_t)round(m11 * (temp.x - center_x) +
	                          m12 * (temp.y - center_y) + center_x);
	point->y = (int16_t)round(m21 * (temp.x - center_x) +
	                          m22 * (temp.y - center_y) + center_y);
}

void path_t::linear_map(float m11, float m12, float m21, float m22,
                        int center_x, int center_y) {
	// Transform first point
	linear_map_point(&p1, m11, m12, m21, m22, center_x, center_y);
	
	// Transform additional points
	switch (type) {
	case path_t::LINE:
		linear_map_point(&p2, m11, m12, m21, m22, center_x, center_y);
		break;
	case path_t::QUAD_BEZIER:
		linear_map_point(&quad_bezier->p2, m11, m12, m21, m22,
		                 center_x, center_y);
		linear_map_point(&quad_bezier->p3, m11, m12, m21, m22,
		                 center_x, center_y);
		break;
	case path_t::CUBIC_BEZIER:
		linear_map_point(&cubic_bezier->p2, m11, m12, m21, m22,
		                 center_x, center_y);
		linear_map_point(&cubic_bezier->p3, m11, m12, m21, m22,
		                 center_x, center_y);
		linear_map_point(&cubic_bezier->p4, m11, m12, m21, m22,
		                 center_x, center_y);
		break;
	}
}

void path_t::translate(int x, int y) {
	// Translate first point
	p1.x += x;
	p1.y += y;

	// Translate additional points
	switch (type) {
	case path_t::LINE:
		p2.x += x;
		p2.y += y;
		break;
	case path_t::QUAD_BEZIER:
		quad_bezier->p2.x += x;
		quad_bezier->p2.y += y;
		quad_bezier->p3.x += x;
		quad_bezier->p3.y += y;
		break;
	case path_t::CUBIC_BEZIER:
		cubic_bezier->p2.x += x;
		cubic_bezier->p2.y += y;
		cubic_bezier->p3.x += x;
		cubic_bezier->p3.y += y;
		cubic_bezier->p4.x += x;
		cubic_bezier->p4.y += y;
		break;
	}
}

point_t path_t::get_p1() {
	return p1;
}

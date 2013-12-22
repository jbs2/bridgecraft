#ifndef NUMBER_H
#define NUMBER_H

#include <GL/gl.h>                                                  // Header File For The OpenGL32 Library
#include <GL/glu.h>                                                 // Header File For The GLu32 Library
#include <GL/glx.h>
#include <math.h>
#include <iostream>

class Number {
	private:
		static void nbeam(float wsize, float hsize, float x, float y, short dir, bool dirhv, bool enabled);	// segment einer Zahl
		static void ncol(float wsize, float hsize, float x, float y, bool a, bool b, bool c, bool d, bool e, bool f, bool g); // zeichne 7 Segment Anzeige
		static void drawDigit(float wsize, float hsize, float x, float y, short num); // draw digit
	public:
		static void drawNumber(float wsize, float hsize, float x, float y, int num, int length);
};

#endif	// NUMBER_H

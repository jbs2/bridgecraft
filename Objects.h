#ifndef OBJECTS_H
#define OBJECTS_H

#include <GL/gl.h>                                                  // Header File For The OpenGL32 Library
#include <GL/glu.h>                                                 // Header File For The GLu32 Library
#include <GL/glx.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>
#include <math.h>
#include "Textures.h"
#include "ArcBall.h"

#define PI2 6.28318531

class Objects {
	public:
		static void Torus(float MinorRadius, float MajorRadius);
		static void square(float size, float x, float y);
		static void timber4(float width, float fromX, float fromY, float toX, float toY);
		static void grid();
		static void gridS();
		static void grid2();
		static void upButton(float size, float x, float y, short dir);
};

#endif // OBJECTS_H

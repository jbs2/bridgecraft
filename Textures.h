#ifndef TEXTURES_H
#define TEXTURES_H

#include <iostream>
#include <GL/gl.h>                                                  // Header File For The OpenGL32 Library
#include <GL/glu.h>                                                 // Header File For The GLu32 Library
#include <GL/glx.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>
#include <cstdio>
#include <cstdlib>

/* Image type - contains height, width, and data */
struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};
typedef struct Image Image;

class Textures {
	private:
		static GLuint trashTexture;

		static int ImageLoad(char *filename, Image *image);
	public:
		static void LoadGLTextures();
		static void deleteTextures();
		static void bindTextures();
};

#endif	// TEXTURES_H

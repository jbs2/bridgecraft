
#ifndef BRIDGECRAFT_H
#define BRIDGECRAFT_H

#include<iostream>

#include <GL/gl.h>                                                  // Header File For The OpenGL32 Library
#include <GL/glu.h>                                                 // Header File For The GLu32 Library
#include <GL/glx.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>
#include <cstdio>
#include <cstdlib>
#include "NeHeGL.h"                                                 // Header File For NeHeGL

#include "math.h"                                                   // NEW: Needed For Sqrtf
#include "ArcBall.h"                                                // NEW: ArcBall Header
#include "Bridge.h"
#include "Level.h"
#include "Level1.h"
#include "Level2.h"

#ifndef CDS_FULLSCREEN                                              // CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4                                            // Compilers. By Defining It This Way,
#endif                                                              // We Can Avoid Errors

Level** levels;	// all levels (array)
static Level* level;	// current level
Bridge* bridge;	// current bridge

GL_Window*  g_window;
Keys*       g_keys;

GLuint buff[64] = {0};

short wwidth; short wheight;
short mpx; short mpy;
bool newClick=true;
void gridS();
void stopPicking();

void DrawS();
short state=0;

bool gotNewPosition=false;	// got new grid position coordinates
bool recycleClicked=false;	// got the 300, picking found recycle logo
bool recycleState=false;	// is clicking recycle button
bool recyclingMode=false;	// is in recycling mode

GLuint trashTexture;

// User Defined Variables
GLUquadricObj *quadratic;                                           // Used For Our Quadric

const float PI2 = 2.0*3.1415926535f;                                // PI Squared

Matrix4fT   Transform   = {  1.0f,  0.0f,  0.0f,  0.0f,             // NEW: Final Transform
                             0.0f,  1.0f,  0.0f,  0.0f,
                             0.0f,  0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  0.0f,  1.0f };

Matrix3fT   LastRot     = {  1.0f,  0.0f,  0.0f,                    // NEW: Last Rotation
                             0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  1.0f };

Matrix3fT   ThisRot     = {  1.0f,  0.0f,  0.0f,                    // NEW: This Rotation
                             0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  1.0f };

ArcBallT    ArcBall(640.0f, 480.0f);                                // NEW: ArcBall Instance
Point2fT    MousePt;                                                // NEW: Current Mouse Point
bool        isClicked  = false;                                     // NEW: Clicking The Mouse?
bool        isRClicked = false;                                     // NEW: Clicking The Right Mouse Button?
bool        isDragging = false;           

/* Image type - contains height, width, and data */
struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};
typedef struct Image Image;



#endif // BRIDGECRAFT_H


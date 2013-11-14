/*
 * This code was created by Jeff Molofee '99 
 * (ported to Linux/GLX by Mihael Vrbanec '00)
 *
 * If you've found this code useful, please let me know.
 * * Visit Jeff at http://nehe.gamedev.net/
 *
 * or for port-specific comments, questions, bugreports etc. 
 * email to Mihael.Vrbanec@stud.uni-karlsruhe.de
 */

#ifndef GL_FRAMEWORK
#define GL_FRAMEWORK

typedef struct {				// Structure For Keyboard Stuff
	bool keyDown [256];			// Holds TRUE / FALSE For Each Key
} Keys;							// Keys

typedef struct {				// Window Creation Info
	Display		*dpy;
	Window		win;
	GLXContext	ctx;
	XSetWindowAttributes attr;
	XF86VidModeModeInfo deskMode;
	bool doubleBuffered;
	int	screen;
	char *title;
	unsigned int width;
	unsigned int height;
	unsigned int depth;
	bool isFullScreen;
} GL_WindowInit;

typedef struct {					// Contains Information Vital To A Window
	Keys*	keys;					// Key Structure
	GL_WindowInit	init;			// Window Init
	struct timeval lastTickCount;	// Tick Counter
} GL_Window;						// GL_Window

void TerminateApplication (GL_Window* window);		// Terminate The Application
void ToggleFullscreen (GL_Window* window);			// Toggle Fullscreen / Windowed Mode

// These Are The Function You Must Provide
bool Initialize (GL_Window* window, Keys* keys);	// Performs All Your Initialization
void Deinitialize (void);							// Performs All Your DeInitialization
void Update (long milliseconds);					// Perform Motion Updates
void Draw (void);									// Perform All Your Scene Drawing

#endif												// GL_FRAMEWORK

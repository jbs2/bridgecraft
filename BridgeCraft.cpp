/**************************************
*                                     *
*            Bridge Craft             *
*                2012                 *
*                                     *
**************************************/

#include"BridgeCraft.h"

// quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.  
// See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info.
int ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size;                 // size of the image in bytes.
    unsigned long i;                    // standard counter.
    unsigned short int planes;          // number of planes in image (must be 1) 
    unsigned short int bpp;             // number of bits per pixel (must be 24)
    char temp;                          // temporary color storage for bgr-rgb conversion.
    if ((file = fopen(filename, "rb"))==NULL) {		// make sure the file is there.
		printf("File Not Found : %s\n",filename);
		return 0;
    }
    fseek(file, 18, SEEK_CUR);			// seek through the bmp header, up to the width/height:
	image->sizeX=0;
    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {		// read the width
    	printf("Error reading width from %s.\n", filename);
		return 0;
    }
    printf("Width of %s: %lu\n", filename, image->sizeX);
	image->sizeY=0;
    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {		// read the height
		printf("Error reading height from %s.\n", filename);
		return 0;
    }
    printf("Height of %s: %lu\n", filename, image->sizeY);
    size = image->sizeX * image->sizeY * 3;		// calculate the size (assuming 24 bits or 3 bytes per pixel).
    if ((fread(&planes, 2, 1, file)) != 1) {	// read the planes
		printf("Error reading planes from %s.\n", filename);
		return 0;
    }
    if (planes != 1) {
		printf("Planes from %s is not 1: %u\n", filename, planes);
		return 0;
    }
    if ((i = fread(&bpp, 2, 1, file)) != 1) { // read the bpp
		printf("Error reading bpp from %s.\n", filename);
		return 0;
    }
    if (bpp != 24) {
		printf("Bpp from %s is not 24: %u\n", filename, bpp);
		return 0;
    }
    fseek(file, 24, SEEK_CUR);		// seek past the rest of the bitmap header.
    image->data = (char *) malloc(size);	// read the data
    if (image->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 0;	
    }
    if ((i = fread(image->data, size, 1, file)) != 1) {
		printf("Error reading image data from %s.\n", filename);
		return 0;
    }
    for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
		temp = image->data[i];
		image->data[i] = image->data[i+2];
		image->data[i+2] = temp;
    }
    return 1;
}

// Load Bitmaps And Convert To Textures
void LoadGLTextures() {	
	// Load Texture
	Image *image1;
             
	// allocate space for texture
	image1 = (Image *) malloc(sizeof(Image));
	if (image1 == NULL) {
		printf("Error allocating space for image");
		exit(0);
	}
	if (!ImageLoad((char*)"trash.bmp", image1)) {
		exit(1);
	}        
	// Create Texture
	glGenTextures(1, &trashTexture);
	glBindTexture(GL_TEXTURE_2D, trashTexture);   // 2d texture (x and y size)
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture
	// 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
	// border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
}

bool Initialize (GL_Window* window, Keys* keys) {						// Any GL Init Code & User Initialiazation Goes Here
	g_window	= window;
	g_keys		= keys;

	// Start Of User Initialization
    isClicked   = false;								            // NEW: Clicking The Mouse?
    isDragging  = false;							                // NEW: Dragging The Mouse?

	glClearColor (0.0f, 0.0f, 0.0f, 0.5f);							// Black Background
	glClearDepth (1.0f);											// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);										// The Type Of Depth Testing (Less Or Equal)
	glEnable (GL_DEPTH_TEST);										// Enable Depth Testing
	glShadeModel (GL_FLAT);											// Select Flat Shading (Nice Definition Of Objects)
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);				// Set Perspective Calculations To Most Accurate

	quadratic=gluNewQuadric();										// Create A Pointer To The Quadric Object
	gluQuadricNormals(quadratic, GLU_SMOOTH);						// Create Smooth Normals
	gluQuadricTexture(quadratic, GL_TRUE);							// Create Texture Coords

	glEnable(GL_LIGHT0);											// Enable Default Light
	glEnable(GL_LIGHTING);											// Enable Lighting

	glEnable(GL_COLOR_MATERIAL);									// Enable Color Material

	levels=new Level*[10];
	levels[0]=new Level1();
	level=levels[0];
//	 bridge=level->myBridge;
	bridge = new Bridge();
	bridge->addEdge(0.1f,-24.0f,10.0f,0.0f,12.0f);
	bridge->addEdge(0.1f,0.0f,12.0f,24.0f,10.0f);
	bridge->addEdge(0.04f,0.0f,12.0f,8.0f,4.0f);
	bridge->addEdge(1.21f,8.0f,4.0f,9.0f,2.0f);
	bridge->addEdge(1.0f,10.0f,3.0f,10.0f,2.0f);
	bridge->addEdge(0.2f,-14.0f,0.0f,14.0f,-3.0f);

//	trashTexture = LoadTextureRAW("trash",0,20,20);
	LoadGLTextures();

	return true;													// Return true (Initialization Successful)
}

void Deinitialize (void) {											// Any User DeInitialization Goes Here
	gluDeleteQuadric(quadratic);
	glDeleteTextures( 1, &trashTexture );
	delete bridge;
}

void mySelect(GLdouble x, GLdouble y, int width, int height) { // mouse curser in window x,y; window width,hight
	// http://www.lighthouse3d.com/opengl/picking/index.php?openglway3
	GLint view[4];
	glSelectBuffer(64, buff);
	glRenderMode(GL_SELECT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glGetIntegerv(GL_VIEWPORT,view);
	gluPickMatrix(x,((GLdouble)height)-y,30.0f,30.0f,view);
	gluPerspective (45.0f, (GLfloat)(width)/(GLfloat)(height),          // Calculate The Aspect Ratio Of The Window
                    1.0f, 100.0f);
    glMatrixMode (GL_MODELVIEW);                                        // Select The Modelview Matrix
	DrawS();
	stopPicking();
    glLoadIdentity ();
	glRenderMode(GL_RENDER);
//	stopPicking();
}

void processHits(GLint hits, GLuint buffer[]) {
	unsigned int i, j;
	GLuint names, *ptr, minZ,*ptrNames, numberOfNames;

	ptr = (GLuint *) buffer;
	minZ = 0xffffffff;
	for (i = 0; i < hits; i++) {	
		names = *ptr;
		ptr++;
		if (*ptr < minZ) {
			numberOfNames = names;
			minZ = *ptr;
			ptrNames = ptr+2;
		}
		ptr += names+2;
	}
//	std::cout<< "The closest hit names are " ;
	ptr = ptrNames;
///	for (j = 0; j < numberOfNames; j++,ptr++) {
//		std::cout<< *ptr << "\t";
//	}
	if(numberOfNames > 1) {
//		std::cout << "process "<<*ptr << " " << *(ptr+1);
//		mpx = (float)(*ptr) - 24.0f;
//		mpy = (float)(*(ptr+1)) - 13.0f;
		if(*ptr ==  300) {
			recycleClicked=true;
		} else if(recyclingMode) {
			bridge->liveRemove((int)(*ptr)-24, (int)(*(ptr+1))-13,(int)(*(ptr+2))-24,(int)(*(ptr+3))-13);
		} else if(*ptr == 303) {	// up button pressed
//			if(100 > thicknessClickCounter > 0) {
			
//			}
			++thickness;
//			++thicknessClickCounter;
		} else if(*ptr == 301) {	// down button pressed
			if(thickness>1)
				--thickness;
		} else {
			gotNewPosition=true;
			mpx=*ptr;
			mpy=*(ptr+1);
		}
//		mpx-=24.0f;
//		mpy-=13.0f;
	} else
		gotNewPosition=false;
}

void stopPicking() {
	int hits;
	// restoring the original projection matrix
 	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();
	// returning to normal rendering mode
	hits = glRenderMode(GL_RENDER);
	// if there are hits process them
//	std::cout<<"\nhits = "<< hits;
	if (hits != 0)
		processHits(hits,buff);
}

void Update (long milliseconds) {									// Perform Motion Updates Here
	if(isClicked) {
		if(newClick) {
			if(!state) {
				mySelect(MousePt.s.X, MousePt.s.Y,(int)wwidth, (int)wheight);
				if(recycleClicked) {
					recycleState=true;
					recyclingMode=recyclingMode?false:true;
					newClick=false;
				} else if(recyclingMode) {
	//				mySelect(MousePt.s.X, MousePt.s.Y,(int)wwidth, (int)wheight);
					newClick=false;
				}
				else if(newClick && gotNewPosition) {
					bridge->click3(mpx,mpy,((float)thickness)/100.0f);
					newClick = false;
				}
			} else {
					Matrix3fSetIdentity(&LastRot);								// Reset Rotation
					Matrix3fSetIdentity(&ThisRot);								// Reset Rotation
					Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);		// Reset Rotation
					ArcBall.clickL(&MousePt);
			
			//		mySelect(MousePt.s.X, MousePt.s.Y, (int)wwidth, (int)wheight);
	
						state = 0;
						newClick = false;
						printf("was here asdf\n");
						for(int i=bridge->nnodes; i--;) {
							bridge->nodes[i].reset();
						}
			}
		}
	} else {
		newClick = true;
		gotNewPosition = false;
		recycleClicked = false;
		recycleState = false;
	}

	if(bridge->tfrom) {
		mySelect(MousePt.s.X, MousePt.s.Y, (int) wwidth, (int)wheight);
		if((bridge->tfrom->p.x != (mpx-24.0f) || bridge->tfrom->p.y != (mpy-13.0f))) {
			if(5>(sqrt(pow(bridge->tfrom->p.x-(mpx-24.0f),2)+pow(bridge->tfrom->p.y-(mpy-13.0f),2)))) {		// restricting to nodes which aren't itself ( edgelenth=0 ) and limiting the maximal size of edges
				if(bridge->tto == 0)	// tto = temporary to
					bridge->tto = new Node();
				bridge->tto->set((float)mpx-24.0f, (float)mpy-13.0f);
				if(bridge->tEdge == 0) {
					bridge->tEdge = new Edge();	// tEdge = temporary Edge
					bridge->tEdge->set(((float)thickness)/100.0f,bridge->tfrom,bridge->tto);
				}
			} else {
				if(bridge->tto == 0)	// tto = temporary to
					bridge->tto = new Node();
//				std::cout<<"tfrom x="<<bridge->tfrom->p.x<<"  tfrom y="<<bridge->tfrom->p.y<<"  mpx-24.0f="<<(float)mpx-24.0f<<"   mpy-13.0f="<<mpy-13.0f<<std::endl;
				float vlength = 5/sqrt(pow(((float)mpx-24.0f)-bridge->tfrom->p.x,2.0)+pow(((float)mpy-13.0f)-bridge->tfrom->p.y,2.0));
				bridge->tto->set(bridge->tfrom->p.x+round((((float)mpx-24.0f)-bridge->tfrom->p.x)*vlength), bridge->tfrom->p.y+round((((float)mpy-13.0f)-bridge->tfrom->p.y)*vlength));
				if(bridge->tEdge == 0) {
					bridge->tEdge = new Edge();	// tEdge = temporary Edge
					bridge->tEdge->set(((float)thickness)/100.0f,bridge->tfrom,bridge->tto);
				}
			}
		}
//		else
//			std::cout<<"\n was here"<<std::endl;
	}

    if (!isDragging) {												// Not Dragging
        if (isRClicked) {												// First Click
			recyclingMode = false;
			bridge->stopClicking();
			state=1;
			isDragging = true;										// Prepare For Dragging
			LastRot = ThisRot;										// Set Last Static Rotation To Last Dynamic One
			ArcBall.click(&MousePt);								// Update Start Vector And Prepare For Dragging
        }
    }
    else {
        if (isRClicked) {												// Still Clicked, So Still Dragging
            Quat4fT     ThisQuat;

            ArcBall.drag(&MousePt, &ThisQuat);						// Update End Vector And Get Rotation As Quaternion
            Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);		// Convert Quaternion Into Matrix3fT
            Matrix3fMulMatrix3f(&ThisRot, &LastRot);				// Accumulate Last Rotation Into This One
            Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);	// Set Our Final Transform's Rotation From This One
        }
        else														// No Longer Dragging
            isDragging = false;
    }
}

void Torus(float MinorRadius, float MajorRadius)					// Draw A Torus With Normals
{
	int i, j;
	glBegin( GL_TRIANGLE_STRIP );									// Start A Triangle Strip
		for (i=0; i<20; i++ )										// Stacks
		{
			for (j=-1; j<20; j++)									// Slices
			{
				float wrapFrac = (j%20)/(float)20;
				float phi = PI2*wrapFrac;
				float sinphi = float(sin(phi));
				float cosphi = float(cos(phi));

				float r = MajorRadius + MinorRadius*cosphi;

				glNormal3f(float(sin(PI2*(i%20+wrapFrac)/(float)20))*cosphi, sinphi, float(cos(PI2*(i%20+wrapFrac)/(float)20))*cosphi);
				glVertex3f(float(sin(PI2*(i%20+wrapFrac)/(float)20))*r,MinorRadius*sinphi,float(cos(PI2*(i%20+wrapFrac)/(float)20))*r);

				glNormal3f(float(sin(PI2*(i+1%20+wrapFrac)/(float)20))*cosphi, sinphi, float(cos(PI2*(i+1%20+wrapFrac)/(float)20))*cosphi);
				glVertex3f(float(sin(PI2*(i+1%20+wrapFrac)/(float)20))*r,MinorRadius*sinphi,float(cos(PI2*(i+1%20+wrapFrac)/(float)20))*r);
			}
		}
	glEnd();														// Done Torus
}

void upButton(float size, float x, float y, short dir) {
	glPushName((unsigned int) 302+dir); glPushName((unsigned int) 302+dir);
	size/=2.0f;
	glBegin(GL_TRIANGLES);
	glNormal3f(0,0,1);
	glVertex3f(x-size,y-dir*size,0.1);
	glNormal3f(0,0,1);
	glVertex3f(x+size,y-dir*size,0.1);
	glNormal3f(0,0,1);
	glVertex3f(x,y+dir*size,0.1);
	glEnd();
	glPopName(); glPopName();
}

void nbeam(float wsize, float hsize, float x, float y, short dir, bool dirhv) {
	glPushName((unsigned int) 308+dir); glPushName((unsigned int) 308+dir);
	wsize/=2.0f;
	hsize/=2.0f;
	glBegin(GL_TRIANGLES);
	glNormal3f(0,0,1);
	glVertex3f(dirhv ? x-wsize : x-hsize-dir*wsize,dirhv ? y-hsize-dir*wsize : y-wsize,0.1);
	glNormal3f(0,0,1);
	glVertex3f(dirhv ? x-wsize : x+hsize+dir*wsize,dirhv ? y+hsize+dir*wsize : y-wsize,0.1);
	glNormal3f(0,0,1);
	glVertex3f(dirhv ? x+wsize: x+hsize-dir*wsize,dirhv ? y+hsize-dir*wsize : y+wsize,0.1);

	glNormal3f(0,0,1);
	glVertex3f(dirhv ? x-wsize : x-hsize-dir*wsize,dirhv ? y-hsize-dir*wsize : y-wsize,0.1);
	glNormal3f(0,0,1);
	glVertex3f(dirhv ? x+wsize : x-hsize+dir*wsize ,dirhv ? y-hsize+dir*wsize : y+wsize,0.1);
	glNormal3f(0,0,1);
	glVertex3f(dirhv ? x+wsize : x+hsize-dir*wsize,dirhv ? y+hsize-dir*wsize : y+wsize,0.1);
	glEnd();
	glPopName(); glPopName();
}

/*void nv(float wsize, float hsize, float x, float y, short dir) {
	glPushName((unsigned int) 305+dir); glPushName((unsigned int) 305+dir);
	wsize/=2.0f;
	hsize/=2.0f;
	glBegin(GL_TRIANGLES);
	glNormal3f(0,0,1);
	glVertex3f(x-wsize,y-hsize-dir*wsize,0.1);
	glNormal3f(0,0,1);
	glVertex3f(x-wsize,y+hsize+dir*wsize,0.1);
	glNormal3f(0,0,1);
	glVertex3f(x+wsize,y+hsize-dir*wsize,0.1);

	glNormal3f(0,0,1);
	glVertex3f(x-wsize,y-hsize-dir*wsize,0.1);
	glNormal3f(0,0,1);
	glVertex3f(x+wsize,y-hsize+dir*wsize,0.1);
	glNormal3f(0,0,1);
	glVertex3f(x+wsize,y+hsize-dir*wsize,0.1);
	glEnd();
	glPopName(); glPopName();
}*/

void square(float size, float x, float y) {
//	glPopName(); glPopName();
    glPushName((unsigned int) 300); glPushName((unsigned int) 300);
	size/=2.0f;
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, trashTexture );
	glBegin(GL_TRIANGLES);
	glNormal3f(0.0,0,1);
	glTexCoord2d(0.0,1.0);
	glVertex3f(x-size,y+size,0.1);
	glNormal3f(0.0,0.0,1);
	glTexCoord2d(1.0,1.0);
	glVertex3f(x+size,y+size,0.1);
	glNormal3f(0.0,0.0,1);
	glTexCoord2d(1.0,0.0);
	glVertex3f(x+size,y-size,0.1);
	glNormal3f(0.0,0.0,1);
	glTexCoord2d(1.0,0.0);
	glVertex3f(x+size,y-size,0.1);
	glNormal3f(0.0,0.0,1);
	glTexCoord2d(0.0,0.0);
	glVertex3f(x-size,y-size,0.1);
	glNormal3f(0.0,0.0,1);
	glTexCoord2d(0.0,1.0);
	glVertex3f(x-size,y+size,0.1);
	glEnd();
	glDisable( GL_TEXTURE_2D);
	glPopName(); glPopName();
}

void Timber4(float width, float fromX, float fromY, float toX, float toY) {
	float ir=width/(float)2;
	Tuple3fT a; a.s.X=fromX-toX; a.s.Y=fromY-toY; a.s.Z=0;
	Tuple3fT b; b.s.X=0; b.s.Y=0; b.s.Z=-1;
	Tuple3fT normal, dir;
	Vector3fCross(&normal, &a, &b);
	GLfloat l=Vector3fLength(&normal);
	normal.s.X=normal.s.X/l; normal.s.Y=normal.s.Y/l; normal.s.Z=normal.s.Z/l;
	dir.s.X=normal.s.X*ir; dir.s.Y=normal.s.Y*ir; dir.s.Z=normal.s.Z*ir;
	l=Vector3fLength(&a);
	a.s.X=a.s.X/l; a.s.Y=a.s.Y/l; a.s.Z=a.s.Z/l;

	glBegin(GL_TRIANGLES);
		// If it's too slow, leave these 4 triangles away
		// 4 triangles to model the two caps
		glNormal3f(a.s.X,a.s.Y,a.s.Z);
		glVertex3f(fromX+dir.s.X,fromY+dir.s.Y,-ir);
		glNormal3f(a.s.X,a.s.Y,a.s.Z);
		glVertex3f(fromX+dir.s.X,fromY+dir.s.Y,ir);
		glNormal3f(a.s.X,a.s.Y,a.s.Z);
		glVertex3f(fromX-dir.s.X,fromY-dir.s.Y,ir);

		glNormal3f(a.s.X,a.s.Y,a.s.Z);
		glVertex3f(fromX+dir.s.X,fromY+dir.s.Y,-ir);
		glNormal3f(a.s.X,a.s.Y,a.s.Z);
		glVertex3f(fromX-dir.s.X,fromY-dir.s.Y,-ir);
		glNormal3f(a.s.X,a.s.Y,a.s.Z);
		glVertex3f(fromX-dir.s.X,fromY-dir.s.Y,ir);

		glNormal3f(-a.s.X,-a.s.Y,-a.s.Z);
		glVertex3f(toX+dir.s.X,toY+dir.s.Y,-ir);
		glNormal3f(-a.s.X,-a.s.Y,-a.s.Z);
		glVertex3f(toX+dir.s.X,toY+dir.s.Y,ir);
		glNormal3f(-a.s.X,-a.s.Y,-a.s.Z);
		glVertex3f(toX-dir.s.X,toY-dir.s.Y,ir);

		glNormal3f(-a.s.X,-a.s.Y,-a.s.Z);
		glVertex3f(toX-dir.s.X,toY-dir.s.Y,ir);
		glNormal3f(-a.s.X,-a.s.Y,-a.s.Z);
		glVertex3f(toX-dir.s.X,toY-dir.s.Y,-ir);
		glNormal3f(-a.s.X,-a.s.Y,-a.s.Z);
		glVertex3f(toX+dir.s.X,toY+dir.s.Y,-ir);

		// 4 triangles for 2 sides
		glNormal3f(0,0,1);
		glVertex3f(fromX+dir.s.X,fromY+dir.s.Y,ir);
		glNormal3f(0,0,1);
		glVertex3f(fromX-dir.s.X,fromY-dir.s.Y,ir);
		glNormal3f(0,0,1);
		glVertex3f(toX+dir.s.X,toY+dir.s.Y,ir);

		glNormal3f(0,0,1);
		glVertex3f(fromX-dir.s.X,fromY-dir.s.Y,ir);
		glNormal3f(0,0,1);
		glVertex3f(toX+dir.s.X,toY+dir.s.Y,ir);
		glNormal3f(0,0,1);
		glVertex3f(toX-dir.s.X,toY-dir.s.Y,ir);

		glNormal3f(0,0,-1);
		glVertex3f(fromX+dir.s.X,fromY+dir.s.Y,-ir);
		glNormal3f(0,0,-1);
		glVertex3f(fromX-dir.s.X,fromY-dir.s.Y,-ir);
		glNormal3f(0,0,-1);
		glVertex3f(toX+dir.s.X,toY+dir.s.Y,-ir);

		glNormal3f(0,0,-1);
		glVertex3f(fromX-dir.s.X,fromY-dir.s.Y,-ir);
		glNormal3f(0,0,-1);
		glVertex3f(toX+dir.s.X,toY+dir.s.Y,-ir);
		glNormal3f(0,0,-1);
		glVertex3f(toX-dir.s.X,toY-dir.s.Y,-ir);

		// Schiefe Ebene
		glNormal3f(-normal.s.X,-normal.s.Y,-normal.s.Z);
		glVertex3f(fromX-dir.s.X,fromY-dir.s.Y,-ir);
		glNormal3f(-normal.s.X,-normal.s.Y,-normal.s.Z);
		glVertex3f(fromX-dir.s.X,fromY-dir.s.Y,ir);
		glNormal3f(-normal.s.X,-normal.s.Y,-normal.s.Z);
		glVertex3f(toX-dir.s.X,toY-dir.s.Y,ir);

		glNormal3f(-normal.s.X,-normal.s.Y,-normal.s.Z);
		glVertex3f(fromX-dir.s.X,fromY-dir.s.Y,-ir);
		glNormal3f(-normal.s.X,-normal.s.Y,-normal.s.Z);
		glVertex3f(toX-dir.s.X,toY-dir.s.Y,ir);
		glNormal3f(-normal.s.X,-normal.s.Y,-normal.s.Z);
		glVertex3f(toX-dir.s.X,toY-dir.s.Y,-ir);

		glNormal3f(normal.s.X,normal.s.Y,normal.s.Z);
		glVertex3f(fromX+dir.s.X,fromY+dir.s.Y,-ir);
		glNormal3f(normal.s.X,normal.s.Y,normal.s.Z);
		glVertex3f(fromX+dir.s.X,fromY+dir.s.Y,ir);
		glNormal3f(normal.s.X,normal.s.Y,normal.s.Z);
		glVertex3f(toX+dir.s.X,toY+dir.s.Y,ir);

		glNormal3f(normal.s.X,normal.s.Y,normal.s.Z);
		glVertex3f(fromX+dir.s.X,fromY+dir.s.Y,-ir);
		glNormal3f(normal.s.X,normal.s.Y,normal.s.Z);
		glVertex3f(toX+dir.s.X,toY+dir.s.Y,ir);
		glNormal3f(normal.s.X,normal.s.Y,normal.s.Z);
		glVertex3f(toX+dir.s.X,toY+dir.s.Y,-ir);
	glEnd();
}

void grid() {
	glBegin(GL_POINTS);
		for(int i=-24; i<=24; i+=1)
			for(int j=-13; j<=13; j+=1) {
				glNormal3f(0.0,0.0,1.0);
				glVertex3f((float)i,(float)j,0.0f);
			}
	glEnd();
}

void gridS() {
	glPushName(0); glPushName(0);
	for(int i=-24; i<=24; i+=1)
		for(int j=-13; j<=13; j+=1) {
			glPopName(); glPopName();
			glPushName((unsigned int) (i+24)); glPushName((unsigned int) (j+13));
			glBegin(GL_POINTS);
			glNormal3f(0.0,0.0,1.0);
			glVertex3f(i,j,0.0f);
			glEnd();
		}
	glPopName(); glPopName();
}

void grid2() {
	void glInitNames(void);
	glPushName(0);
	const float gridSize = 0.06/2.0;
	glBegin(GL_TRIANGLES);
	for(float i=-24; i<=24; i+=1.0f) {
		for(float j=-12.5; j<=12.5; j+=1.0f) {
			glLoadName(i+j*49);
			glNormal3f(0.0,0.0,1.0);
			glVertex3f(i-gridSize,j+gridSize,0.0f);
			glNormal3f(0.0,0.0,1.0);
			glVertex3f(i-gridSize,j-gridSize,0.0f);
			glNormal3f(0.0,0.0,1.0);
			glVertex3f(i+gridSize,j-gridSize,0.0f);
			
			glNormal3f(0.0,0.0,1.0);
			glVertex3f(i+gridSize,j-gridSize,0.0f);
			glNormal3f(0.0,0.0,1.0);
			glVertex3f(i+gridSize,j+gridSize,0.0f);
			glNormal3f(0.0,0.0,1.0);
			glVertex3f(i-gridSize,j+gridSize,0.0f);
		}
	}
	glEnd();
}

void Draw (void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Clear Screen And Depth Buffer
	glLoadIdentity();												// Reset The Current Modelview Matrix
//	glTranslatef(-1.5f,0.0f,-6.0f);									// Move Left 1.5 Units And Into The Screen 6.0
	glTranslatef(0.0f,0.0f,-35.0f);

    glPushMatrix();													// NEW: Prepare Dynamic Transform
    glMultMatrixf(Transform.M);										// NEW: Apply Dynamic Transform
	glColor3f(0.20f,1.00f,0.3f);
//	Torus(0.30f,1.00f);
//	Timber(2.0f,0.1f);

//	float stepSize=0.08;
//	float k=0.5;
//	for(float i=-4; i<0; i=i+stepSize) {
//		Timber4(0.10f,i,-k,i+stepSize,k);
//		k=k==-0.5?0.5:-0.5;
//	}

//	Timber4(0.2f,-3.0f,-1.0f,-2.0f,1.0f);
//	std::cout<<wwidth<<"  "<<wheight<<"\n";
	if(!state)
		grid();
//	grid2();

//	bridge.addNewNode(-3.0f,0.0f);
//	bridge.addNewNode(-1.0f,1.0f);
	//	bridge.addEdge(0.1f,-3.0f,0.0f,-1.0f,1.0f);
//	bridge.addNewNode(1.0f,0.0f);
	//	bridge.addEdge(0.1f,-1.0f,1.0f,1.0f,0.0f);
	if(state) {
		for(int i=bridge->nedges; i--;) {
			Edge& e=bridge->edges[i];
	//		std::cout<<"\n"<<e.stress;
			glColor3f(e.stress,1.0f-e.stress,0.1f);
			Timber4(e.width, e.from->p.x, e.from->p.y, e.to->p.x, e.to->p.y);
		}
	}
	else {
		for(int i=bridge->nedges; i--;) {
			Edge& e=bridge->edges[i];
			glColor3f(0.6f,0.6f,0.6f);
			Timber4(e.width, e.from->b.x, e.from->b.y, e.to->b.x, e.to->b.y);
		}
		if(bridge->tEdge) {
			glColor3f(0.4f,0.4f,0.4f);
			Timber4(bridge->tEdge->width,bridge->tEdge->from->b.x, bridge->tEdge->from->b.y,bridge->tEdge->to->b.x,bridge->tEdge->to->b.y);
		}
		if(recycleState)
			glColor3f(0.2f,0.2f,0.2f);
		else {
			if(recyclingMode)
				glColor3f(0.3f,1.0f,0.3f);
			else
				glColor3f(1.0f,1.0f,1.0f);
		}
		square(3.2,0,-13);
		upButton(1.2,22,-11,1);
		upButton(1.2,22,-13,-1);
		nbeam(0.3,0.8,19,-11,1,true);
		nbeam(0.3,0.8,18,-11,-1,false);
		nbeam(0.3,0.8,20,-11,-1,true);
		nbeam(0.3,0.8,21,-11,1,false);
		for(int i=level->amountOfFix; i--;) {
			glColor3f(0.1f,0.1f,1.0f);
			Timber4(0.2f,level->fixPositions[i].x-0.10f,level->fixPositions[i].y,level->fixPositions[i].x+0.10f,level->fixPositions[i].y);
//			std::cout<< level->fixPositions[i].x-0.05f << "  "<<level->fixPositions[i].y <<"  "<< level->fixPositions[i].x+0.05f << "  "<< level->fixPositions[i].y <<"\n";
		}
	}

//	Timber3(0.2f,-4.0f,-1.0f,-3.0f,0.0f);
//	Timber3(0.2f,-3.0f,0.0f,-2.0f,-1.0f);
//	Timber3(0.2f,-2.0f,-1.0f,-1.0f,0.0f);
    glPopMatrix();													// NEW: Unapply Dynamic Transform

	glLoadIdentity();												// Reset The Current Modelview Matrix
	glTranslatef(1.5f,0.0f,-6.0f);									// Move Right 1.5 Units And Into The Screen 7.0

//    glPushMatrix();													// NEW: Prepare Dynamic Transform
//    glMultMatrixf(Transform.M);										// NEW: Apply Dynamic Transform
//	glColor3f(1.0f,0.75f,0.75f);
//	gluSphere(quadratic,1.3f,10,10);
//    glPopMatrix();													// NEW: Unapply Dynamic Transform

	glFlush();														// Flush The GL Rendering Pipeline
	if(state) {
		int detail = 100;
		for(int i=detail; i--;)
			bridge->oneTimeStep(1.0f/(60.0f*(float)detail), level);
	}
}

void DrawS() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Clear Screen And Depth Buffer
	glLoadIdentity();												// Reset The Current Modelview Matrix
	glTranslatef(0.0f,0.0f,-35.0f);

	glPushMatrix();													// NEW: Prepare Dynamic Transform
	glMultMatrixf(Transform.M);										// NEW: Apply Dynamic Transform
	glColor3f(0.20f,1.00f,0.3f);

	glInitNames();

	if(recyclingMode) {
		for(int i=bridge->nedges; i--;) {
			Edge& e=bridge->edges[i];
//			glPopName(); glPopName(); glPopName(); glPopName();
		    glPushName((unsigned int) e.from->b.x + 24); glPushName((unsigned int) e.from->b.y + 13); glPushName((unsigned int) e.to->b.x + 24); glPushName((unsigned int) e.to->b.y + 13);
			glColor3f(0.6f,0.6f,0.6f);
			Timber4(e.width, e.from->b.x, e.from->b.y, e.to->b.x, e.to->b.y);
			glPopName(); glPopName(); glPopName(); glPopName();
		}
//		glPopName(); glPopName(); glPopName(); glPopName();
	} else {
		gridS();
	}
	square(2.7,0,-13);
	upButton(0.8,22,-11,1);
    upButton(0.8,22,-13,-1);

	glPopMatrix();													// NEW: Unapply Dynamic Transform

	glLoadIdentity();												// Reset The Current Modelview Matrix
	glTranslatef(1.5f,0.0f,-6.0f);									// Move Right 1.5 Units And Into The Screen 7.0

	glFlush();
}


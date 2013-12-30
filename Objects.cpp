#include "Objects.h"

void Objects::Torus(float MinorRadius, float MajorRadius) {                  // Draw A Torus With Normals
    int i, j;
    glBegin( GL_TRIANGLE_STRIP );                                   // Start A Triangle Strip
        for (i=0; i<20; i++ )                                       // Stacks
        {
            for (j=-1; j<20; j++)                                   // Slices
            {
                float wrapFrac = (j%20)/(float)20;
                float phi = PI2*wrapFrac;
                float sinphi = (float)(sin(phi));
                float cosphi = (float)(cos(phi));

                float r = MajorRadius + MinorRadius*cosphi;

                glNormal3f((float)(sin(PI2*(i%20+wrapFrac)/(float)20))*cosphi, sinphi, (float)(cos(PI2*(i%20+wrapFrac)/(float)20))*cosphi);
                glVertex3f((float)(sin(PI2*(i%20+wrapFrac)/(float)20))*r,MinorRadius*sinphi,(float)(cos(PI2*(i%20+wrapFrac)/(float)20))*r);

                glNormal3f((float)(sin(PI2*(i+1%20+wrapFrac)/(float)20))*cosphi, sinphi, (float)(cos(PI2*(i+1%20+wrapFrac)/(float)20))*cosphi);
                glVertex3f((float)(sin(PI2*(i+1%20+wrapFrac)/(float)20))*r,MinorRadius*sinphi,(float)(cos(PI2*(i+1%20+wrapFrac)/(float)20))*r);
            }
        }
    glEnd();                                                        // Done Torus
}

void Objects::square(float size, float x, float y) {
//  glPopName(); glPopName();
    glPushName((unsigned int) 300); glPushName((unsigned int) 300);
    size/=2.0f;
    glEnable( GL_TEXTURE_2D );
    Textures::bindTextures();
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

void Objects::timber4(float width, float fromX, float fromY, float toX, float toY) {
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

void Objects::grid() {
	glBegin(GL_POINTS);
		for(int i=-24; i<=24; i+=1)
			for(int j=-13; j<=13; j+=1) {
				glNormal3f(0.0,0.0,1.0);
				glVertex3f((float)i,(float)j,0.0f);
			}
	glEnd();
}

void Objects::gridS() {
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

void Objects::grid2() {
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

void Objects::upButton(float size, float x, float y, short dir) {
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


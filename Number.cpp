#include "Number.h"

void Number::nbeam(float wsize, float hsize, float x, float y, short dir, bool dirhv, bool enabled) {
    if(enabled)
		//	glColor3f(0.090196f,0.57255f,0.88627f); from image
		glColor3f(0.190196f,0.67255f,0.98627f);
	else
		//  glColor3f(0.015686f,0.078431f,0.12941f); from image
		glColor3f(0.005686f,0.068431f,0.11941f);
	glPushName((unsigned int) 308+dir); glPushName((unsigned int) 308+dir);
	wsize=((double)wsize)/2.0f;
	hsize=((double)hsize)/2.0f;
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

void Number::ncol(float wsize, float hsize, float x, float y, bool a, bool b, bool c, bool d, bool e, bool f, bool g) {
	float hhsize=((double)hsize)/2.0f;//+((double)wsize)/1.0f;
	float hwsize=((double)wsize)/2.0f;
	float hhwsize=((double)hwsize)/2.0f;
	float hhh=((double)hsize)/7.0f;
	Number::nbeam(wsize, hsize-hhh,x-hhsize,y-hhsize,1,true, e);
	Number::nbeam(wsize, hsize-hhh,x-hhsize,y+hhsize,1,true, f);
	Number::nbeam(wsize, hsize-hhh,x+hhsize,y-hhsize,-1,true, c);
	Number::nbeam(wsize, hsize-hhh,x+hhsize,y+hhsize,-1,true, b);

	Number::nbeam(wsize, hsize-hhh/2.0f,x,y+hsize-hwsize,-1,false, a);
	Number::nbeam(hwsize, hsize,x,y-hhwsize,-1,false, g);
	Number::nbeam(hwsize, hsize,x,y+hhwsize,1,false, g);
	Number::nbeam(wsize, hsize-hhh/2.0f,x,y-hsize+hwsize,1,false, d);
}

void Number::drawDigit(float wsize, float hsize, float x, float y, short num) {
	switch (num) {
		case 0:  ncol(wsize, hsize, x, y, true,  true,  true,  true,  true,  true,  false); break; 
		case 1:  ncol(wsize, hsize, x, y, false, true,  true,  false, false, false, false); break; 
		case 2:  ncol(wsize, hsize, x, y, true,  true,  false, true,  true,  false, true ); break; 
		case 3:  ncol(wsize, hsize, x, y, true,  true,  true,  true,  false, false, true ); break; 
		case 4:  ncol(wsize, hsize, x, y, false, true,  true,  false, false, true,  true ); break; 
		case 5:  ncol(wsize, hsize, x, y, true,  false, true,  true,  false, true,  true ); break; 
		case 6:  ncol(wsize, hsize, x, y, true,  false, true,  true,  true,  true,  true ); break; 
		case 7:  ncol(wsize, hsize, x, y, true,  true,  true,  false, false, false, false); break; 
		case 8:  ncol(wsize, hsize, x, y, true,  true,  true,  true,  true,  true,  true ); break; 
		case 9:  ncol(wsize, hsize, x, y, true,  true,  true,  true,  false, true,  true ); break;
		case 10: ncol(wsize, hsize, x, y, false, false, false, false, false, false, false); break;
		default: std::cout<<("Error in drawDigit(...)\n");
	}
}

void Number::drawNumber(float wsize, float hsize, float x, float y, int num, int length) {
	bool firstNotNull=false;
	short digit=0;
	for(int i=length; i--;) {
		if(i)
			digit=(num/(int)pow(10,i))%10;
		else
			digit=num%10;
		if(!digit && !firstNotNull) {
			drawDigit(wsize,hsize,x+10-i,y,10);
		} else {
			drawDigit(wsize,hsize,x+10-i,y,digit);
			firstNotNull=true;
		}
	}
}


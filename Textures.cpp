#include "Textures.h"

GLuint Textures::trashTexture = 0;

// Load Bitmaps And Convert To Textures
void Textures::LoadGLTextures() {	
	// Load Texture
	Image *image1;
             
	// allocate space for texture
	image1 = (Image *) malloc(sizeof(Image));
	if (image1 == NULL) {
		std::cout<<("Error allocating space for image");
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

// quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.  
// See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info.
int Textures::ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size;                 // size of the image in bytes.
    unsigned long i;                    // standard counter.
    unsigned short int planes;          // number of planes in image (must be 1) 
    unsigned short int bpp;             // number of bits per pixel (must be 24)
    char temp;                          // temporary color storage for bgr-rgb conversion.
    if ((file = fopen(filename, "rb"))==NULL) {		// make sure the file is there.
		std::cout<<("File Not Found : %s\n",filename);
		return 0;
    }
    fseek(file, 18, SEEK_CUR);			// seek through the bmp header, up to the width/height:
	image->sizeX=0;
    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {		// read the width
    	std::cout<<("Error reading width from %s.\n", filename);
		return 0;
    }
//    std::cout<<("Width of %s: %lu\n", filename, image->sizeX);
	image->sizeY=0;
    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {		// read the height
		std::cout<<("Error reading height from %s.\n", filename);
		return 0;
    }
//    std::cout<<("Height of %s: %lu\n", filename, image->sizeY);
    size = image->sizeX * image->sizeY * 3;		// calculate the size (assuming 24 bits or 3 bytes per pixel).
    if ((fread(&planes, 2, 1, file)) != 1) {	// read the planes
		std::cout<<("Error reading planes from %s.\n", filename);
		return 0;
    }
    if (planes != 1) {
		std::cout<<("Planes from %s is not 1: %u\n", filename, planes);
		return 0;
    }
    if ((i = fread(&bpp, 2, 1, file)) != 1) { // read the bpp
		std::cout<<("Error reading bpp from %s.\n", filename);
		return 0;
    }
    if (bpp != 24) {
		std::cout<<("Bpp from %s is not 24: %u\n", filename, bpp);
		return 0;
    }
    fseek(file, 24, SEEK_CUR);		// seek past the rest of the bitmap header.
    image->data = (char *) malloc(size);	// read the data
    if (image->data == NULL) {
		std::cout<<("Error allocating memory for color-corrected image data");
		return 0;	
    }
    if ((i = fread(image->data, size, 1, file)) != 1) {
		std::cout<<("Error reading image data from %s.\n", filename);
		return 0;
    }
    for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
		temp = image->data[i];
		image->data[i] = image->data[i+2];
		image->data[i+2] = temp;
    }
    return 1;
}

void Textures::deleteTextures() {
	glDeleteTextures( 1, &trashTexture );
}

void Textures::bindTextures() {
	glBindTexture( GL_TEXTURE_2D, trashTexture );
}


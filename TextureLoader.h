#ifndef BMPLOADER_H_INCLUDED
#define BMPLOADER_H_INCLUDED

#include <string>
#include <GL/gl.h>

typedef unsigned int uint32;

class TextureLoader {
public:
	TextureLoader(){}
	GLuint loadBMP(const char * imgPath);
private:
	unsigned char header[54];
	uint32 dataPos;
	uint32 width, height;
	uint32 imageSize;
	unsigned char *data;
	
	GLuint createTexture();
};

#endif

#include <iostream>
#include <cstdio>
#include "TextureLoader.h"

using namespace std;

GLuint TextureLoader::loadBMP(const char * imgPath){
	FILE * file = fopen(imgPath,"rb");
	
	if (!file){
		cout << "Image could not be opened\n";
		return 0;
	}
	
	if (fread(header, 1, 54, file) != 54){ // If not 54 bytes read : problem
    	cout << "Not a correct BMP format." << endl;
    	return false;
	}
	
	if(header[0] != 'B' || header[1] != 'M'){
		cout << "Not a correct BMP format." << endl;
		return 0;
	}	
	
	// Read ints from the byte array
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);
	
	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize = width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos = 54; // The BMP header is done that way
	
	data = new unsigned char[imageSize];
	
	fread(data, 1, imageSize, file);
	
	fclose(file);
	
	return createTexture();
}

GLuint TextureLoader::createTexture(){
	GLuint textureID;
	
	// Create one OpenGL texture
	glGenTextures(1, &textureID);	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	
	// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Generate mipmaps, by the way.
	glGenerateMipmap(GL_TEXTURE_2D);
	
	return textureID;
}

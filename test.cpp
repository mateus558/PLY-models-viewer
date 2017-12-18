#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "TextureLoader.h"

using namespace std;

in vec2 UV;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

int main(){
	TextureLoader loader;
	
	GLuint Texture = loader.loadBMP("Textures/Granite1.bmp");
	
	color = texture( myTextureSampler, UV ).rgb;

}

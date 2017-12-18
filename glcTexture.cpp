#include <string>
#include <stdio.h>
#include <malloc.h>

#include "GL/glut.h"
#include "png.h"
#include "glcTexture.h"

//-----------------------------------------------------------
glcTexture::glcTexture()
{
    this->numberOfTextures = 1;
    this->textureID        = new GLuint[1];
    this->aspectRatio      = new float[1];
    this->wrapMode = GL_REPEAT;
    this->magMode = GL_LINEAR;
    this->minMode = GL_LINEAR;
    this->colorMode = GL_MODULATE;

    env_color[0] = 0.7f;
    env_color[1] = 0.7f;
    env_color[2] = 0.0f;
    env_color[3] = 1.0f;
}

//-----------------------------------------------------------
glcTexture::~glcTexture()
{
   if(this->textureID)     delete []this->textureID;
   if(this->aspectRatio)   delete []this->aspectRatio;
}

//-----------------------------------------------------------
void glcTexture::Bind(int id)
{
   glEnable ( GL_TEXTURE_2D );
   this->Update();
   glBindTexture ( GL_TEXTURE_2D, (GLuint) this->textureID[ id ] );
}

//-----------------------------------------------------------
void glcTexture::Disable()
{
   glDisable(GL_TEXTURE_2D);
}

//-----------------------------------------------------------
void glcTexture::SetWrappingMode(int mode)
{
	switch(mode)
	{
		case GL_CLAMP:
			this->wrapMode = GL_CLAMP;
		break;
		case GL_REPEAT:
			this->wrapMode = GL_REPEAT;
		break;
		default:
			printf("Error - Wrong Wrapping Mode!\n\n");
	}
}

//-----------------------------------------------------------
void glcTexture::SetMinFilterMode(int mode)
{
	switch(mode)
	{
		case GL_LINEAR:
			this->minMode = GL_LINEAR;
		break;
		case  GL_NEAREST:
			this->minMode =  GL_NEAREST;
		break;
		case GL_NEAREST_MIPMAP_NEAREST:
			this->minMode =  GL_NEAREST_MIPMAP_NEAREST;
		break;
		case  GL_NEAREST_MIPMAP_LINEAR:
			this->minMode =  GL_NEAREST_MIPMAP_LINEAR;
		break;
		case  GL_LINEAR_MIPMAP_NEAREST:
			this->minMode =  GL_LINEAR_MIPMAP_NEAREST;
		break;
		case  GL_LINEAR_MIPMAP_LINEAR:
			this->minMode =  GL_LINEAR_MIPMAP_LINEAR;
		break;
		default:
			printf("Error - Wrong Min Filter Mode!\n\n");
	}
}

//-----------------------------------------------------------
void glcTexture::SetColorMode(int mode)
{
	switch(mode)
	{
		case GL_REPLACE:
			this->colorMode = GL_REPLACE;
		break;
		case  GL_MODULATE:
			this->colorMode =  GL_MODULATE;
		break;
		case GL_BLEND:
			this->colorMode =  GL_BLEND;
		break;
		case  GL_DECAL:
			this->colorMode =  GL_DECAL;
		break;
		default:
			printf("Error - Wrong Color Mode!\n\n");
	}
}

//-----------------------------------------------------------
void glcTexture::SetMagFilterMode(int mode)
{
	switch(mode)
	{
		case GL_LINEAR:
			this->magMode = GL_LINEAR;
		break;
		case  GL_NEAREST:
			this->magMode =  GL_NEAREST;
		break;
		default:
			printf("Error - Wrong Mag Filter Mode!\n\n");
	}
}

//-----------------------------------------------------------
void glcTexture::SetNumberOfTextures(int n)
{
   this->numberOfTextures = n;

   // Removing default loads
   if(this->textureID)     delete []this->textureID;
   if(this->aspectRatio)   delete []this->aspectRatio;

   this->textureID        = new GLuint[n];
   this->aspectRatio      = new float[n];

   glEnable ( GL_TEXTURE_2D );
   glGenTextures ( this->numberOfTextures, textureID );
}

//-----------------------------------------------------------
float glcTexture::GetAspectRatio(int tex)
{
   if(tex >= this->numberOfTextures)
   {
      printf("Error in glcTexture::GetAspectRatio(int) : Out of Range access!\"\n\n");
      exit(1);
   }
   return this->aspectRatio[tex];
}

//-----------------------------------------------------------
int glcTexture::GetNumberOfTextures()
{
   return this->numberOfTextures;
}

//-----------------------------------------------------------
void glcTexture::CreateTexture(std::string nome, int id)
{
   CreateTextureFromPNG(nome, id);
}

//-----------------------------------------------------------
void glcTexture::CreateTextureFromPNG(std::string nome, int id)
{
	int width;
	int height;

	//const char *filename = nome.c_str();
	glBindTexture ( GL_TEXTURE_2D, this->textureID[id] );

	//header for testing if it is a png
	png_byte header[8];

	//open file as binary
	FILE *fp = fopen(nome.c_str(), "rb");
	if (!fp) return;

	//read the header
	fread(header, 1, 8, fp);

	//test if png
	int is_png = !png_sig_cmp(header, 0, 8);
	if (!is_png)
	{
		fclose(fp);
		return ;
	}

	//create png struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		fclose(fp);
		return ;
	}

	//create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		fclose(fp);
		return ;
	}

	//create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		return ;
	}

	//png error stuff, not sure libpng man suggests this.
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return;
	}
	//init png reading
	png_init_io(png_ptr, fp);

	//let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	//variables to pass to get info
	int bit_depth, color_type;

	// get info about png
	png_get_IHDR(png_ptr, info_ptr, (png_uint_32*) &width, (png_uint_32*) &height, &bit_depth, &color_type, NULL, NULL, NULL);

	this->aspectRatio[id] = (float) width / (float) height;

	color_type = png_get_color_type(png_ptr, info_ptr);

	// These color_type don't have an alpha channel then fill it with 0xff.
	if(color_type == PNG_COLOR_TYPE_RGB ||
		color_type == PNG_COLOR_TYPE_GRAY ||
	   color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// Allocate the image_data as a big block, to be given to opengl
	png_byte *image_data = new png_byte[rowbytes * height];
	if (!image_data)
	{
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return ;
	}

	//row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep *row_pointers = new png_bytep[height];
	if (!row_pointers)
	{
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		delete[] image_data;
		fclose(fp);
		return ;
	}
	// set the individual row_pointers to point at the correct offsets of image_data
	for (int i = 0; i < height; ++i)
		row_pointers[height - i - 1] = image_data + i * rowbytes;

	//read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	// Update texture object
	this->SetTexture(width, height, image_data);

	//clean up memory and close stuff
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] image_data;
	delete[] row_pointers;
	fclose(fp);
}


void glcTexture::Update()
{
   //Aplicação de filtros para tratamento da imagem pelo OpenGL
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,this->magMode);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,this->minMode);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,this->wrapMode);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,this->wrapMode);
   glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, this->env_color);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, this->colorMode);
}

void glcTexture::SetTexture(int width, int height, GLubyte *image)
{
    // Update texture parameters
    this->Update();

    // Opengl stuff
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) image);
}

/*
  Name:        glcTexture.h
  Author:      Rodrigo Luis de Souza da Silva
  Date:        03/11/2008
  Description: Load and store PNG image as textures

  Updates:
      07/11/2008 - Aspect Ratio option added
      09/02/2013 - New default is to load at least one texture
      19/02/2016 - Fix PNG reader when loading files with transparency
*/

#ifndef _GLC_TEXTURE_H
#define _GLC_TEXTURE_H

#include <string>

class glcTexture
{
	public:
      glcTexture();
      ~glcTexture();
      void  Bind(int id = 0);
      void  Disable();
      void  SetNumberOfTextures(int );
      int   GetNumberOfTextures();
      void  CreateTexture(std::string nome, int id = 0);
      void  Update();
      float GetAspectRatio(int);
      void  SetWrappingMode(int);
      void  SetMinFilterMode(int);
      void  SetMagFilterMode(int);
      void  SetColorMode(int);

	protected:
      GLuint *textureID;
      float  *aspectRatio;
      int    numberOfTextures;
      int    wrapMode;
      int    minMode;
      int    magMode;
      int    colorMode;
      GLfloat env_color[4];
      void CreateTextureFromPNG(std::string nome, int id);

	private:
		void SetTexture(int width, int height, GLubyte *image);
};

#endif

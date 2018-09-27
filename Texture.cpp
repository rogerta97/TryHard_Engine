#include "Texture.h"
#include "OpenGL.h"


Texture::Texture()
{
}


Texture::~Texture()
{
}

void Texture::SetWidth()
{
}

unsigned int Texture::GetWidth() const
{
	return 0;
}

void Texture::SetHeight()
{
}

unsigned int Texture::GetHeight() const
{
	return 0;
}

void Texture::SetTextureSettings()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Texture::SetCheckTexture()
{
	buffer = new GLubyte[10 * 10 * 4]; 
	GLubyte checkImage[10][10][4];
	for (int i = 0; i < 10; i++) { // height
		for (int j = 0; j < 10; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	buffer = (GLubyte*)checkImage;
}

void Texture::LoadToMemory()
{

}

void Texture::UnloadFromMemory()
{
}

void Texture::Bind()
{
}

void Texture::UnBind()
{
}

void Texture::Clear()
{
}

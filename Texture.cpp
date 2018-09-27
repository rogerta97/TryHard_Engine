#include "Texture.h"
#include "OpenGL.h"


Texture::Texture()
{
}


Texture::~Texture()
{
}

void Texture::SetWidth(float width)
{
	this->width = width; 
}

unsigned int Texture::GetWidth() const
{
	return width;
}

void Texture::SetHeight(float height)
{
	this->height = height;
}

unsigned int Texture::GetHeight() const
{
	return height;
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
	//buffer = new GLubyte[10 * 10 * 4]; 
	//GLubyte checkImage[10][10][4];
	//for (int i = 0; i < 10; i++) { // height
	//	for (int j = 0; j < 10; j++) {
	//		int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
	//		checkImage[i][j][0] = (GLubyte)c;
	//		checkImage[i][j][1] = (GLubyte)c;
	//		checkImage[i][j][2] = (GLubyte)c;
	//		checkImage[i][j][3] = (GLubyte)255;
	//	}
	//}

	//memcpy(buffer, checkImage, 400 * sizeof(GLubyte));
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 10, 10, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

uint Texture::CreateBuffer()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texture_id);

	return texture_id;
}

void Texture::Clear()
{
}

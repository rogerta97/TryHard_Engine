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

Texture* Texture::GetCheckTexture()
{
	GLubyte* buffer = new GLubyte[64 * 64 * 4];
	GLubyte checkImage[64][64][4];
	for (int i = 0; i < 64; i++) { // height
		for (int j = 0; j < 64; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	memcpy(buffer, checkImage, 400 * sizeof(GLubyte));

	//Create the texture to return 
	Texture* check_tex = new Texture(); 
	check_tex->name = "Checker Texture"; 

	check_tex->CreateBuffer(); 
	check_tex->Bind(); 
	SetTextureSettings(); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	check_tex->UnBind(); 

	check_tex->width = 256; 
	check_tex->height = 256;

	return check_tex; 
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

const char * Texture::GetPath() const
{
	return path.c_str();
}

void Texture::SetPath(const char * path)
{
	this->path = path; 
}

const char * Texture::GetName() const
{
	return name.c_str();
}

void Texture::SetName(const char * new_name)
{
	name = new_name; 
}

uint Texture::GetTextureID() const
{
	return texture_id;
}

void Texture::SetTextureID(uint id)
{
	texture_id = id; 
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

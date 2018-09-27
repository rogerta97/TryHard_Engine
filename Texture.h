#pragma once

#include "Globals.h"
#include "OpenGL.h"

class Texture
{
public:
	Texture();
	~Texture();

	void SetWidth(float width);
	unsigned int GetWidth() const;

	void SetHeight(float height);
	unsigned int GetHeight() const;

	void SetTextureSettings(); 
	void SetCheckTexture();

	//Memory management
	void Bind();
	void UnBind();

	uint CreateBuffer(); 

	void Clear(); 

	GLubyte* buffer; 

private:

	uint texture_id;
	uint width;
	uint height;
};


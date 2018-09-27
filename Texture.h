#pragma once

#include "Globals.h"
#include "OpenGL.h"

class Texture
{
public:
	Texture();
	~Texture();

	void SetWidth();
	unsigned int GetWidth() const;

	void SetHeight();
	unsigned int GetHeight() const;

	void SetTextureSettings(); 
	void SetCheckTexture();

	//Memory management
	void Bind();
	void UnBind();

	uint CreateBuffer(); 

	void Clear(); 

private:

	uint texture_id;
	uint width;
	uint height;

	GLubyte* buffer; 


};


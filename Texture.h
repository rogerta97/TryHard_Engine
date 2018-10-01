#pragma once

#include "Globals.h"
#include "OpenGL.h"
#include <string>

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

	uint GetTextureID(); 
	void SetTextureID(uint id); 

	uint CreateBuffer(); 

	void Clear(); 

private:

	uint texture_id;
	uint width;
	uint height;

	std::string path; 
};


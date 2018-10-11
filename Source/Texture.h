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
	Texture* GetCheckTexture();

	//Memory management
	void Bind();
	void UnBind();

	const char* GetPath() const; 
	void SetPath(const char* path); 

	const char* GetName() const; 
	void SetName(const char* new_name); 

	uint GetTextureID() const; 
	void SetTextureID(uint id); 

	uint CreateBuffer(); 

	void Clear(); 

private:

	uint texture_id;
	uint width;
	uint height;

	std::string path;
	std::string name; 
};


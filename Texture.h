#pragma once

#include "Globals.h"
#include "OpenGL.h"
#include <string>
#include "Resource.h"

class Texture
{
public:
	Texture();
	~Texture();

	//Utility
	void SetTextureSettings(); 
	void FillCheckerTextureData();

	//Memory management
	void Bind();
	void UnBind();

	//Setters & Getters 
	const char* GetPath() const; 
	void SetPath(const char* path); 

	uint GetTextureID() const; 
	void SetTextureID(uint id); 

	void SetWidth(float width);
	unsigned int GetWidth() const;

	void SetHeight(float height);
	unsigned int GetHeight() const;

	//Buffers
	uint CreateBuffer(); 
	void Clear(); 

	std::string name = "";
	uint texture_id;

private:

	
	uint width;
	uint height;

	std::string path = ""; 
	
};


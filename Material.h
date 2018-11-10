#pragma once
#include "Color.h"
#include "Resource.h"

class Texture; 

class Material : public Resource
{
public:
	Material();
	~Material();

	void CleanUp(); 

	Texture* GetDiffuseTexture(); 
	void SetDiffuseTexture(Texture* new_tex); 

	bool IsLoadedToMemory(); 

	Color color;

	Texture * diffuse;

private:

	

};


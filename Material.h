#pragma once
#include "Color.h"

class Texture; 

class Material
{
public:
	Material();
	~Material();

	void CleanUp(); 

	Texture* GetDiffuseTexture(); 
	void SetDiffuseTexture(Texture* new_tex); 


	Color color;

private:

	Texture * diffuse; 

};


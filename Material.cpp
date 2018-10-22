#include "Material.h"



Material::Material()
{
	diffuse = nullptr;
}


Material::~Material()
{
}

Texture * Material::GetDiffuseTexture()
{
	return diffuse;
}

void Material::SetDiffuseTexture(Texture * new_tex)
{
	if(new_tex != nullptr)
		diffuse = new_tex; 
}

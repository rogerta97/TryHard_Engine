#include "Material.h"
#include "Texture.h"


Material::Material()
{
	diffuse = nullptr;
}


Material::~Material()
{
}

void Material::CleanUp()
{
	diffuse->Clear();
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

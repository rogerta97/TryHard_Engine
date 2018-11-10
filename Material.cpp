#include "Material.h"
#include "Texture.h"
#include "OpenGL.h"


Material::Material()
{
	diffuse = nullptr;
	color = { 1.0f,1.0f,1.0f };
	reference_counting = 0; 
}


Material::~Material()
{
}

void Material::CleanUp()
{
	//diffuse->Clear();
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

bool Material::IsLoadedToMemory()
{
	if (diffuse->GetTextureID() != 0)
		return true; 

	return false;
}

void Material::LoadToMemory()
{
	diffuse->Bind();
}



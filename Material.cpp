#include "Material.h"
#include "Texture.h"
#include "OpenGL.h"

#include "Application.h"
#include "MaterialImporter.h"


#include "mmgr\mmgr.h"

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

bool Material::IsLoadedToMemory()
{
	if (diffuse->GetTextureID() != 0)
		return true; 

	return false;
}

void Material::LoadToMemory()
{
	diffuse = App->resources->material_importer->LoadTexture(path.c_str(), true);
}

void Material::UnloadFromMemory()
{
	if (diffuse != nullptr)
	{
		glDeleteTextures(1, (GLuint*)&diffuse->texture_id);
		delete diffuse;
	}
}

void Material::CreateMeta()
{
}

void Material::SetColor(Color color)
{
	this->color = color; 
}



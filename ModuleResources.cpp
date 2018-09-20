#include "ModuleResources.h"


ModuleResources::ModuleResources()
{
	name = "Resources";
}


ModuleResources::~ModuleResources()
{
}

void ModuleResources::AddTextureToList(Texture* new_texture)
{
	
}

Texture * ModuleResources::GetTextureFromID()
{
	return nullptr;
}

update_status ModuleResources::Update()
{
	return update_status::UPDATE_CONTINUE;
}

#include "ModuleResources.h"

#include "MeshImporter.h"


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

bool ModuleResources::Start()
{
	//Initiallize importers
	bool ret = true; 

	LoadImporters();

	return ret;
}

void ModuleResources::LoadImporters()
{
	//Initiallize importers
	mesh_importer = new MeshImporter();
	mesh_importer->Start(); 

	texture_importer = new TextureImporter();
	texture_importer->Start();
}

update_status ModuleResources::Update()
{
	return update_status::UPDATE_CONTINUE;
}

Texture * ModuleResources::GetCheckedTexture()
{
	Texture* new_text; 

	new_text->SetCheckTexture(); 

	return new_text;
}


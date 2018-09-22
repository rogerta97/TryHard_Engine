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

	//Call Start
	for (std::list<Importer*>::iterator it = importer_list.begin(); it != importer_list.end(); it++)
	{
		if ((*it)->Start())
			continue; 

		ret = false; 
	}

	return ret;
}

void ModuleResources::LoadImporters()
{
	//Initiallize importers
	MeshImporter* mesh_importer = new MeshImporter();
	importer_list.push_back((Importer*)mesh_importer);
}

update_status ModuleResources::Update()
{
	return update_status::UPDATE_CONTINUE;
}

Importer * ModuleResources::GetImporterFromType(ImporterType type)
{
	for (auto it = importer_list.begin(); it != importer_list.end(); it++)
	{
		if ((*it)->imp_type == type)
			return (*it); 
	}

	return nullptr; 
}

#include "ModuleResources.h"
#include "ModuleFileSystem.h"
#include "Application.h"

#include "Resource.h"

#include "Functions.h"

#include "MeshImporter.h"
#include "MaterialImporter.h"
#include "Material.h"


ModuleResources::ModuleResources()
{
	name = "Resources";
}

ModuleResources::~ModuleResources()
{
}

Resource * ModuleResources::Get(UID uid)
{
	for (auto it = resources.begin(); it != resources.end(); it++)
	{
		if ((*it).first == uid)
			return (*it).second;
	}

	return nullptr; 
}

Resource * ModuleResources::Get(resource_type type, const char * resource_name)
{
	for (auto it = resources.begin(); it != resources.end(); it++)
	{
		if ((*it).second->GetType() == type && App->file_system->DeleteFileExtension((*it).second->name.c_str()) == resource_name)
			return (*it).second;
	}
}

Resource* ModuleResources::CreateNewResource(resource_type type, UID force_id)
{
	Resource* to_ret = nullptr; 

	switch (type)
	{
	case RES_MESH:
		to_ret = new Mesh();
		break;

	case RES_TEXTURE:
		to_ret = new Texture();		
		break; 

	case RES_MATERIAL:
		to_ret = new Material();
		break;
	}

	if (to_ret)
	{
		to_ret->SetType(type);
		to_ret->SetUID(GenerateUID());
		resources[to_ret->GetUID()] = to_ret;
	}
		
	return to_ret; 
}

void ModuleResources::AddTextureToList(Texture* new_texture)
{
	
}

UID ModuleResources::GenerateUID()
{
	UID	ret_id = GetPCGRandomNumberInt(0, 9999999);
	return ret_id;
}

bool ModuleResources::Init(JSON_Object* config)
{
	return true; 
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
	material_importer = new MaterialImporter();
	material_importer->Start();

	mesh_importer = new MeshImporter();
	mesh_importer->Start();
}

update_status ModuleResources::Update()
{
	return update_status::UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{
//	material_importer->SaveTexturesAsDDS();

	return false;
}



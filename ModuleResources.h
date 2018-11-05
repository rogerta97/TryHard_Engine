#pragma once

#include "Module.h"
#include <map>
#include "Resource.h"

class MeshImporter; 
class MaterialImporter; 
class Texture; 

class ModuleResources : public Module
{
public:
	ModuleResources();
	~ModuleResources();

	bool Init(JSON_Object* config);
	bool Start();
	update_status Update();
	bool CleanUp();
	
	Resource* Get(UID uid);
	Resource* CreateNewResource(resource_type type, UID force_id = 0);
	void AddTextureToList(Texture* new_texture); 
	UID GenerateUID();

	void LoadImporters();

	MeshImporter* mesh_importer; 
	MaterialImporter* material_importer; 

private: 
	
	std::map<UID, Resource*> resources;


};


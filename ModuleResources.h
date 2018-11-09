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
	update_status PostUpdate(float dt); 
	bool CleanUp();

	void AddResourceToDelete(UID to_del_id); 
	void DeleteResourcesNow();
	bool WantToDelete();
	void DeleteFromList(UID to_del_id);
	
	Resource* Get(UID uid);
	Resource* Get(resource_type type, const char* resource_name);
	Resource* CreateNewResource(resource_type type, UID force_id = 0);
	void AddTextureToList(Texture* new_texture); 
	UID GenerateUID();

	void LoadImporters();

	void PrintConfigData();

	MeshImporter* mesh_importer; 
	MaterialImporter* material_importer; 

private: 
	
	std::map<UID, Resource*> resources;
	std::vector<UID> to_del_list; 


};


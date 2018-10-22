#pragma once

#include "Module.h"
#include "MaterialImporter.h"
#include <map>
#include "Importer.h"

#include "MeshImporter.h"

class ModuleResources : public Module
{
public:
	ModuleResources();
	~ModuleResources();

	void AddTextureToList(Texture* new_texture); 

	bool Start(); 	
	update_status Update();
	bool CleanUp(); 

	void LoadImporters();

	MeshImporter* mesh_importer; 
	MaterialImporter* material_importer; 

private: 
	
	std::list<Resource*> resource_list;


};


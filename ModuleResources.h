#pragma once

#include "Module.h"
#include "TextureImporter.h"
#include <map>
#include "Importer.h"

#include "Resource.h"

#include "MeshImporter.h"
#include "TextureImporter.h"

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
	TextureImporter* texture_importer; 

private: 
	
	std::list<Resource*> resource_list;


};


#pragma once

#include "Module.h"
#include "TextureImporter.h"
#include <map>
#include "Importer.h"

#include "MeshImporter.h"
#include "TextureImporter.h"

class ModuleResources : public Module
{
public:
	ModuleResources();
	~ModuleResources();

	void AddTextureToList(Texture* new_texture); 
	Texture* GetTextureFromID(); 

	bool Start(); 
	void LoadImporters(); 
	update_status Update();

	MeshImporter* mesh_importer; 
	TextureImporter* texture_importer; 

private: 
	
	std::map<unsigned int, Texture*> textures_list;


};


#pragma once

#include "Module.h"
#include "TextureImporter.h"
#include <map>
#include "Importer.h"


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

	Importer* GetImporterFromType(ImporterType type);

private: 
	
	std::map<unsigned int, Texture*> textures_list;

	std::list<Importer*> importer_list; 

};


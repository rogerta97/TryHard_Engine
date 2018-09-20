#pragma once

#include "Module.h"
#include "TextureImporter.h"

#include <map>

class ModuleResources : public Module
{
public:
	ModuleResources();
	~ModuleResources();

	void AddTextureToList(Texture* new_texture); 
	Texture* GetTextureFromID(); 

	update_status Update();

private: 
	
	std::map<unsigned int, Texture*> textures_list; 

};


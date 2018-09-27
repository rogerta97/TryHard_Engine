#pragma once

#include "Texture.h"
#include <list>

#include "Importer.h"


class TextureImporter : public Importer
{
public:
	TextureImporter();

	bool Start();
	bool Update();
	bool CleanUp();

	~TextureImporter();

private:

	std::list<Texture*> textures_list; 

};


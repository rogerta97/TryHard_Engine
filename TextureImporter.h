#pragma once

#include "Texture.h"
#include "Importer.h"

#include <filesystem>
#include <list>

#include "DevIL Windows SDK\include\IL\il.h"
#include "DevIL Windows SDK\include\IL\ilu.h"
#include "DevIL Windows SDK\include\IL\ilut.h"

#pragma comment(lib, "DevIL Windows SDK/libx86/x86/Release/DevIL.lib")
#pragma comment(lib, "DevIL Windows SDK/libx86/x86/Release/ILU.lib")
#pragma comment(lib, "DevIL Windows SDK/libx86/x86/Release/ILUT.lib")


class TextureImporter : public Importer
{
public:
	TextureImporter();

	bool Start();
	bool Update();
	bool CleanUp();

	Texture* LoadTexture(const char* path, bool not_flip = false); 

	bool SaveTexturesAsDDS();

	bool SaveTexture(Texture* tex_to_save, ILenum format_type); 

	Texture* DrawTextureList(); 

	Texture* GetTexture(const char* name); 

	~TextureImporter();

private:

	std::list<Texture*> textures_list; 

};


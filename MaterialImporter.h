#pragma once

#include "Texture.h"
#include "Importer.h"

#include <filesystem>
#include <list>

#include "DevIL Windows SDK\include\IL\il.h"
#include "DevIL Windows SDK\include\IL\ilu.h"
#include "DevIL Windows SDK\include\IL\ilut.h"

#pragma comment(lib, "DevIL Windows SDK/libx86/DevIL.lib")
#pragma comment(lib, "DevIL Windows SDK/libx86/ILU.lib")
#pragma comment(lib, "DevIL Windows SDK/libx86/ILUT.lib")

class Material; 

class MaterialImporter
{
public:
	MaterialImporter();

	bool Start();
	bool Update();
	bool CleanUp();

	Texture* LoadTexture(const char* path, bool not_flip = false); 

	bool SaveTexturesAsDDS();

	bool SaveTexture(Texture* tex_to_save, ILenum format_type); 

	void ImportAllFilesInAssets();

	void DeleteTextureFromList(Texture* to_del);

	bool DrawTextureList(); 

	Texture* GetTexture(const char* name); 

	void GenerateCheckerTexture();
	Texture* GetCheckerTexture() const; 

	bool Import(Material* mat_to_save, const char* tex_name); 
	Material* LoadFromBinary(const char* tex_name);

	~MaterialImporter();

private:

	Texture* checker_texture = nullptr; 

};


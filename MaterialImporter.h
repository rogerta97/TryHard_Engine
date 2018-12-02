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
	void FlipTexture(Texture* tex); 

	void ImportAllFilesFromAssets();

	void ManageNewTexture(std::string path);

	bool DrawTextureList(bool ui = false); 

	void GenerateCheckerTexture();
	Texture* GetCheckerTexture() const; 

	bool Import(Material* mat_to_save, const char* tex_name, UID uid); 
	void LoadFromBinary(const char* tex_name, Material* mat_to_fill);

	~MaterialImporter();

private:

	Texture* checker_texture = nullptr; 

};


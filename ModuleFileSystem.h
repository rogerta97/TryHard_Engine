#pragma once
#include "Module.h"

using namespace std; 

enum file_extension
{
	FX_PNG,
	FX_FBX,
	FX_JPG,
	FX_DDS,
	FX_ERR
};

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(bool start_enabled = true);
	~ModuleFileSystem();

	bool Init(JSON_Object* config);
	update_status Update(float dt);
	bool CleanUp();

	file_extension GetFileExtension(string full_path);
	string GetWorkingDirectory() const;
	string GetLastPathItem(const char* path, bool termination = false);
	string DeleteLastPathItem(const char* path);
	bool IsFolder(const char* directory); 

	string GetModelsPath() const; 
	string GetTexturesPath() const;
	string GetLibraryPath() const;
	string GetSkyBoxPath() const;
	string GetAssetsPath() const; 

	std::vector<string> GetFilesInDirectory(const char* directory); 
	bool IsFileInDirectory(const char* directory, const char* filename);

private:

	string library_path; 
	string mesh_library_path; 
	string game_path; 
	string assets_path; 
	string models_path; 
	string skybox_path;
	string textures_path;
};


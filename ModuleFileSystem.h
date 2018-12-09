#pragma once
#include "Module.h"

#include "MonitorDirectory.h"

using namespace std; 

enum file_extension
{
	FX_PNG,
	FX_FBX,
	FX_JPG,
	FX_DDS,
	FX_TTF,
	FX_OTF,
	FX_META,
	FX_TGA, 
	FX_JSON, 
	FX_JPREFAB,  
	FX_ERR
};

enum file_type
{
	FT_3DMODEL,
	FT_IMAGE,
	FT_FONT,
	FT_SCENE, 
	FT_META,
	FT_PREFAB, 
	FT_UNDEFINED,
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
	file_type GetFileType(string full_path);

	string GetFileExtensionStr(string full_path);
	string GetWorkingDirectory() const;
	string GetLastPathItem(string path, bool termination = false);
	string GetItemsAmountFromEnd(string directory, int ammount);
	string DeleteLastFileExtension(string path);
	string DeleteLastPathItem(string path);
	string DeleteFileExtension(string path);
	bool IsFolder(string directory);

	string GetModelsPath() const; 
	string GetTexturesPath() const;
	string GetPrefabPath() const;
	string GetScriptsPath() const;
	string GetLibraryPath() const;
	string GetSkyBoxPath() const;
	string GetAssetsPath() const; 
	string GetScenesPath() const; 
	string GetFontsPath() const; 

	UID GenerateUID(); 

	vector<string> GetNewFiles() const; 

	void GetFilesInDirectory(const char* directory, std::vector<string>& list, bool include_path, bool get_meta);
	void GetFilesInThisDirectory(const char* directory, std::vector<string>& list, bool include_path);
	std::vector<string> GetAllFilesInDirectory(const char* directory, bool include_path);
	std::vector<string> GetAllFoldersInDirectory(const char* directory, bool include_path);
	string GetAllFilesExtension(string path);
	std::vector<string> GetAllItemsInDirectory(const char* directory, bool include_path);
	string GetFileInAllDirectory(const char* directory);
	bool IsFileInDirectory(string directory, const char* filename);

	int files_ammount;

private:

	MonitorDirectory* monitor_directory; 

	string library_path; 
	string game_path; 
	string assets_path; 
	string scripts_path; 
	string scenes_path; 
	string prefabs_path; 
	string models_path; 
	string skybox_path;
	string fonts_path; 
	string textures_path;
};


#include "ModuleFileSystem.h"
#include "Application.h"
#include "Shlwapi.h"
#include "Functions.h"
#include <sys\stat.h>
#include <cstdio>

#include <fstream>
#include <string>
#include <Windows.h>
#include <filesystem>


#include "mmgr\mmgr.h"

ModuleFileSystem::ModuleFileSystem(bool start_enabled)
{
	name = "FileSystem"; 
}

ModuleFileSystem::~ModuleFileSystem()
{
}

bool ModuleFileSystem::Init(JSON_Object* config)
{
	char* buf = new char[256];
	GetCurrentDirectoryA(256, buf);

	game_path = buf;

	library_path = game_path + string("\\") + std::string("Library");
	CreateDirectory(library_path.c_str(), NULL);

	string curr_lib_folder_path = library_path.c_str() + string("\\") + std::string("Meshes");
	CreateDirectory(curr_lib_folder_path.c_str(), NULL);

	curr_lib_folder_path = library_path.c_str() + string("\\") + std::string("Prefabs");
	CreateDirectory(curr_lib_folder_path.c_str(), NULL);

	curr_lib_folder_path = library_path.c_str() + string("\\") + std::string("Scenes");
	CreateDirectory(curr_lib_folder_path.c_str(), NULL);

	curr_lib_folder_path = library_path.c_str() + string("\\") + std::string("Materials");
	CreateDirectory(curr_lib_folder_path.c_str(), NULL);
		 
	assets_path = game_path + string("\\") + string("Assets");
	models_path = assets_path + string("\\") + string("3DModels");
	scenes_path = assets_path + string("\\") + string("Scenes");
	textures_path = assets_path + string("\\") + string("Textures");
	prefabs_path = assets_path + string("\\") + string("Prefabs");
	scripts_path = assets_path + string("\\") + string("Scripts");
	skybox_path = assets_path + string("\\") + string("Textures\\SkyBox");
	fonts_path = assets_path + string("\\") + string("Fonts");

	//Start monitor directory
	monitor_directory = new MonitorDirectory(); 
	monitor_directory->StartMonitoring(assets_path);

	return true;
}

update_status ModuleFileSystem::Update(float dt)
{
	monitor_directory->Update(); 

	return UPDATE_CONTINUE;
}

bool ModuleFileSystem::CleanUp()
{
	delete monitor_directory;
	return true;
}

file_extension ModuleFileSystem::GetFileExtension(std::string full_path) 
{
		int pos = full_path.find_last_of(".");

		if (pos == -1)
			return FX_ERR; 

		string term = full_path.substr(pos, full_path.length() - pos);

		if (term == ".FBX" || term == ".fbx")
			return FX_FBX;

		else if (term == ".PNG" || term == ".png")
			return FX_PNG;

		else if (term == ".DDS" || term == ".dds")
			return FX_DDS;

		else if (term == ".jpg" || term == ".JPG")
			return FX_JPG;

		else if (term == ".ttf" || term == ".TTF")
			return FX_TTF;

		else if (term == ".otf" || term == ".OTF")
			return FX_OTF;

		else if (term == ".tga" || term == ".TGA")
			return FX_TGA;

		else if (term == ".json" || term == ".JSON")
			return FX_JSON;

		else if (term == ".jprefab")
			return FX_JPREFAB;

		else if (term == ".meta")
			return FX_META;
		
			

		return FX_ERR;
	
}

string ModuleFileSystem::GetFileExtensionStr(std::string full_path)
{
	int pos = full_path.find_last_of(".");
	string term = full_path.substr(pos, full_path.length() - pos);

	return term;

}

file_type ModuleFileSystem::GetFileType(string full_path)
{
	file_extension curr_extension = GetFileExtension(full_path);

	if (curr_extension == FX_FBX)
		return file_type::FT_3DMODEL;

	else if (curr_extension == FX_PNG || curr_extension == FX_JPG || curr_extension == FX_JPG || curr_extension == FX_DDS || curr_extension == FX_TGA)
		return file_type::FT_IMAGE; 

	else if (curr_extension == FX_TTF || curr_extension == FX_OTF)
		return file_type::FT_FONT;

	else if (curr_extension == FX_JSON)
		return file_type::FT_SCENE;

	else if (curr_extension == FX_JPREFAB)
		return file_type::FT_PREFAB;

	else if (curr_extension == FX_META)
		return file_type::FT_META;

	return file_type::FT_UNDEFINED; 
}

std::string ModuleFileSystem::GetWorkingDirectory() const
{
	return game_path;
}

string ModuleFileSystem::GetLastPathItem(string path, bool termination)
{
	string result_string(path);

	int pos = result_string.find_last_of('\\');
	int to_copy = result_string.length() - pos; 
	result_string = result_string.substr(pos + 1, to_copy);

	if (termination == false)
		result_string = DeleteFileExtension(result_string.c_str());
			
	return result_string;
}

string ModuleFileSystem::GetItemsAmountFromEnd(string directory, int ammount)
{
	string ret_str = directory;

	for (int i = 0; i < ammount; i++)
		ret_str = DeleteLastPathItem(ret_str.c_str());

	return ret_str; 
}

string ModuleFileSystem::DeleteLastPathItem(string path)
{
	string result_string(path);
	int pos = result_string.find_last_of('\\');
	result_string = result_string.substr(0, pos); 
	return result_string;
}

string ModuleFileSystem::DeleteFileExtension(string path_char)
{
	string path(path_char);

	int pos = path.find_first_of(".");
	int to_del = path.size() - pos; 
	path = path.substr(0, path.size() - to_del);

	return path;
}
void ModuleFileSystem::DeleteAllFileExtensions(string& path)
{
	while (path.find_last_of('.') <= path.size())
	{
		path = DeleteLastFileExtension(path.c_str());
	}
}
string ModuleFileSystem::DeleteLastFileExtension(string path_char)
{
	string path(path_char);

	int pos = path.find_last_of(".");
	int to_del = path.size() - pos;
	path = path.substr(0, path.size() - to_del);

	return path;
}

bool ModuleFileSystem::IsFolder(string directory)
{
	vector<string> files;
	App->file_system->GetFilesInDirectory(directory.c_str(), files, false, false);

	for (auto it = files.begin(); it != files.end(); it++)
	{
		if ((*it) != "." && (*it) != "..")
		{
			return true; 
		}
	}

	return false; 
}

std::string ModuleFileSystem::GetModelsPath() const
{
	return models_path;
}

std::string ModuleFileSystem::GetTexturesPath() const
{
	return textures_path;
}

string ModuleFileSystem::GetPrefabPath() const
{
	return prefabs_path;
}

string ModuleFileSystem::GetScriptsPath() const
{
	return scripts_path;
}

std::string ModuleFileSystem::GetLibraryPath() const
{
	return library_path;
}

string ModuleFileSystem::GetSkyBoxPath() const
{
	return skybox_path;
}

string ModuleFileSystem::GetAssetsPath() const
{
	return assets_path;
}

string ModuleFileSystem::GetScenesPath() const
{
	return scenes_path;
}

string ModuleFileSystem::GetFontsPath() const
{
	return fonts_path;
}

UID ModuleFileSystem::GenerateUID()
{
	UID	ret_id = GetPCGRandomNumberInt(0, 9999999);	
	return ret_id;
}

vector<string> ModuleFileSystem::GetNewFiles() const
{
	return vector<string>();
}

void ModuleFileSystem::GetFilesInThisDirectory(const char * directory, std::vector<string>& list, bool include_path = false)
{
	std::string path(directory);
	path.append("\\*");

	WIN32_FIND_DATA data;
	HANDLE hFind;

	if ((hFind = FindFirstFile(path.c_str(), &data)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (std::string(data.cFileName) != std::string(".") && std::string(data.cFileName) != std::string(".."))
			{
	
				string new_str = directory + string("\\") + string(data.cFileName);

				if(!IsFolder(new_str.c_str()))
					list.push_back(new_str);
				
			}


		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}

	return;
}

void ModuleFileSystem::GetFilesInDirectory(const char * directory, std::vector<string>& list, bool include_path = false, bool get_meta = false)
{
	std::string path(directory);
	path.append("\\*");

	WIN32_FIND_DATA data;
	HANDLE hFind;

	if ((hFind = FindFirstFile(path.c_str(), &data)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (GetFileExtension(data.cFileName) == FX_META && get_meta == false)
				continue;

			if (std::string(data.cFileName) != std::string(".") && std::string(data.cFileName) != std::string(".."))
			{
				if (include_path)
				{
					string new_str = directory + string("\\") + string(data.cFileName);
					list.push_back(new_str);
				}					
				else
					list.push_back(data.cFileName);
			}
				

		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}

	return;
}

std::vector<string> ModuleFileSystem::GetAllFilesInDirectory(const char * directory, bool include_path = false)
{
	vector<string> file_names;
	GetFilesInDirectory(directory, file_names, include_path);

	vector<string> files_to_add; 

	for (auto it = file_names.begin(); it != file_names.end();)
	{
		string new_dir = directory + string("\\") + GetLastPathItem((*it).c_str(), true);

		if (GetFileExtension(new_dir.c_str()) == FX_META)
		{
			it = file_names.erase(it);
			continue; 
		}

		if (App->file_system->IsFolder(new_dir.c_str()))
		{			
			it = file_names.erase(it);
			GetFilesInDirectory(new_dir.c_str(), files_to_add, include_path);				
		}
		else
			it++;
	}

	for (auto it = files_to_add.begin(); it != files_to_add.end(); it++)
		file_names.push_back((*it));
	
	return file_names;
}

std::vector<string> ModuleFileSystem::GetAllFoldersInDirectory(const char * directory, bool include_path)
{
	vector<string> file_names;
	GetFilesInDirectory(directory, file_names, include_path);

	vector<string> files_to_add;
	vector<string> folder_files; 

	for (auto it = file_names.begin(); it != file_names.end();it++)
	{
		string new_dir = directory + string("\\") + GetLastPathItem((*it).c_str(), true);

		if (App->file_system->IsFolder(new_dir.c_str()))
		{
			folder_files.push_back(new_dir.c_str());
			GetFilesInDirectory(new_dir.c_str(), files_to_add, include_path);		
		}	
	}

	for (auto it = files_to_add.begin(); it != files_to_add.end(); it++)
	{
		if (App->file_system->IsFolder((*it).c_str()))
			folder_files.push_back((*it).c_str());		
	}

	return folder_files;
}

std::vector<string> ModuleFileSystem::GetAllItemsInDirectory(const char * directory, bool include_path)
{
	std::vector<string> files = GetAllFilesInDirectory(directory, include_path);
	std::vector<string> folders = GetAllFoldersInDirectory(directory, include_path);;
	std::vector<string> res_str; 

	for (auto it = files.begin(); it != files.end(); it++)
	{
		res_str.push_back((*it)); 
	}

	for (auto it = folders.begin(); it != folders.end(); it++)
	{
		res_str.push_back((*it));
	}

	return res_str; 
}

string ModuleFileSystem::GetFileInAllDirectory(const char * directory)
{
	vector<string> files = GetAllFilesInDirectory(directory); 

	for (auto it = files.begin(); it != files.end(); it++)
	{

	}

	return nullptr;

}

bool ModuleFileSystem::IsFileInDirectory(string directory, const char * filename)
{
	std::vector<string> files_to_ret;

	std::string path(directory);
	path.append("\\*");

	WIN32_FIND_DATA data;
	HANDLE hFind;

	if ((hFind = FindFirstFile(path.c_str(), &data)) != INVALID_HANDLE_VALUE)
	{
		do {
			if (std::string(data.cFileName) == std::string(filename))
				return true; 

		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}

	return false;
}

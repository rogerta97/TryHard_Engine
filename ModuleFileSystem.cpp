#include "ModuleFileSystem.h"
#include "Application.h"
#include "Shlwapi.h"
#include <sys\stat.h>
#include <cstdio>

#include <fstream>
#include <string>
#include <Windows.h>
#include <filesystem>


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

	mesh_library_path = library_path.c_str() + string("\\") + std::string("Meshes");
	CreateDirectory(mesh_library_path.c_str(), NULL);

	mesh_library_path = library_path.c_str() + string("\\") + std::string("Materials");
	CreateDirectory(mesh_library_path.c_str(), NULL);
		 
	assets_path = game_path + string("\\") + string("Assets");
	models_path = assets_path + string("\\") + string("3DModels");
	textures_path = assets_path + string("\\") + string("Textures");
	skybox_path = assets_path + string("\\") + string("Textures\\SkyBox");

	return true;
}

update_status ModuleFileSystem::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleFileSystem::CleanUp()
{
	return true;
}

file_extension ModuleFileSystem::GetFileExtension(std::string full_path) 
{
		std::string term = full_path.substr(full_path.length() - 4);

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

		return FX_ERR;
	
}

std::string ModuleFileSystem::GetWorkingDirectory() const
{
	return game_path;
}

string ModuleFileSystem::GetLastPathItem(const char* path, bool termination)
{
	string result_string(path);

	int pos = result_string.find_last_of('\\');
	int to_copy = result_string.length() - pos; 
	result_string = result_string.substr(pos + 1, to_copy);

	if (termination == false)
		result_string = result_string.substr(0, result_string.size() - 4); 

	return result_string;
}

string ModuleFileSystem::DeleteLastPathItem(const char * path)
{
	string result_string(path);
	int pos = result_string.find_last_of('\\');
	result_string = result_string.substr(0, pos); 
	return result_string;
}

bool ModuleFileSystem::IsFolder(const char * directory)
{
	vector<string> files = App->file_system->GetFilesInDirectory(directory);

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


std::vector<string> ModuleFileSystem::GetFilesInDirectory(const char * directory)
{
	std::vector<string> files_to_ret; 

	std::string path(directory);
	path.append("\\*");

	WIN32_FIND_DATA data;
	HANDLE hFind;

	if ((hFind = FindFirstFile(path.c_str(), &data)) != INVALID_HANDLE_VALUE)
	{
		do 
		{
			//if(std::string(data.cFileName) != std::string(".")  && std::string(data.cFileName) != std::string(".."))
			files_to_ret.push_back(data.cFileName);

		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}

	return files_to_ret; 
}

bool ModuleFileSystem::IsFileInDirectory(const char * directory, const char * filename)
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

#include "ModuleFileSystem.h"
#include "Application.h"

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

bool ModuleFileSystem::Start()
{
	char* buf = new char[256];
	GetCurrentDirectoryA(256, buf);

	game_path = buf + std::string("\\"); 

	library_path = game_path + std::string("Library\\"); 
	models_path = game_path + string("Assets\\3DModels\\");
	textures_path = game_path + string("Assets\\Textures\\");
	skybox_path = game_path + string("Assets\\Textures\\SkyBox\\");

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

std::vector<string> ModuleFileSystem::GetFilesInDirectory(const char * directory)
{
	std::vector<string> files_to_ret; 

	std::string path(directory);
	path.append("*");

	WIN32_FIND_DATA data;
	HANDLE hFind;

	if ((hFind = FindFirstFile(path.c_str(), &data)) != INVALID_HANDLE_VALUE)
	{
		do {
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
			if (data.cFileName == filename)
				return true; 

		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}

	return false;
}

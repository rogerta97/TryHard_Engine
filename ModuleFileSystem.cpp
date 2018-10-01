#include "ModuleFileSystem.h"
#include <fstream>
#include <Windows.h>


ModuleFileSystem::ModuleFileSystem(bool start_enabled)
{
}

ModuleFileSystem::~ModuleFileSystem()
{
}

bool ModuleFileSystem::Start()
{
	char* buf = new char[256];
	GetCurrentDirectoryA(256, buf);

	game_path = buf + std::string("\\"); 
	models_path = game_path + string("Assets\\3DModels\\");
	textures_path = game_path + string("Assets\\Textures\\");

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

	return result_string;
}

std::string ModuleFileSystem::GetModelsPath()
{
	return models_path;
}

std::string ModuleFileSystem::GetTexturesPath()
{
	return textures_path;
}

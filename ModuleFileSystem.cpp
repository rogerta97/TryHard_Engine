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
	textures_path = game_path + string("Textures\\");

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

std::string ModuleFileSystem::GetModelsPath()
{
	return models_path;
}

std::string ModuleFileSystem::GetTexturesPath()
{
	return models_path;
}

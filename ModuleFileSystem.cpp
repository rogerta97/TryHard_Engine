#include "ModuleFileSystem.h"
#include <fstream>


ModuleFileSystem::ModuleFileSystem(bool start_enabled)
{
}

ModuleFileSystem::~ModuleFileSystem()
{
}

bool ModuleFileSystem::Start()
{
	models_path = "C:\\Users\\FULLMAC\\Documents\\GitHub\\TryHard_Engine\\Game\\Assets\\3DModels\\"; 

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

std::string ModuleFileSystem::GetModelsPath()
{
	return models_path;
}

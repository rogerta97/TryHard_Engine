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
	return false;
}

update_status ModuleFileSystem::Update(float dt)
{
	return update_status();
}

bool ModuleFileSystem::CleanUp()
{
	return false;
}

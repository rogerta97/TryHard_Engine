#include "ModuleUserInterface.h"


ModuleUserInterface::ModuleUserInterface()
{
}


ModuleUserInterface::~ModuleUserInterface()
{
}

bool ModuleUserInterface::Init(JSON_Object * config)
{
	return true;
}

bool ModuleUserInterface::Start()
{
	return true;
}

update_status ModuleUserInterface::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleUserInterface::CleanUp()
{
	return true;
}

#include "ModuleTime.h"



ModuleTime::ModuleTime()
{
	name = "Time manager";
}


ModuleTime::~ModuleTime()
{
}

update_status ModuleTime::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

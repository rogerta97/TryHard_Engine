#pragma once
#include "Module.h"

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(bool start_enabled = true);
	~ModuleFileSystem();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

private:


};


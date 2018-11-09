#pragma once
#include "Module.h"
class ModuleTime :
	public Module
{
public:
	ModuleTime();
	~ModuleTime();

	Timer real_timer;
	Timer game_timer;

	update_status Update(float dt) override;
};


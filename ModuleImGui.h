#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleImGui : public Module
{
public:
	ModuleImGui(bool start_enabled = true);
	~ModuleImGui();

	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp();

	// Utility

	void DrawTopBar();
	void SetDefaultStyle(); 

};

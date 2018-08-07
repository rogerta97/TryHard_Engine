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

	update_status DrawTopBar();
	void SetDefaultStyle(); 

private: 

	bool show_demo_window = false; 
	bool show_style_editor = false; 

};

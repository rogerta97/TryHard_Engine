#pragma once
#include "Module.h"
#include "Globals.h"

class UI_ConfigurationPanel; 
class UI_ScenePanel; 
class UI_Panel;

enum Panel_Types { CONFIGURATION_PANEL, SCENE_PANEL };

class ModuleImGui : public Module
{
public:
	ModuleImGui(bool start_enabled = true);
	~ModuleImGui();

	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp();

	UI_Panel* AddPanel(Panel_Types type);

	// Utility

	update_status DrawTopBar();
	update_status DrawDocking();
	void SetDefaultStyle(); 

private: 

	bool show_demo_window = false; 
	bool show_style_editor = false; 

	std::list<UI_Panel*> panels_list;

	UI_ConfigurationPanel* config_panel; 
	UI_ScenePanel* scene_panel; 

};

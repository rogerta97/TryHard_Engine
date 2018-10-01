#pragma once
#include "Module.h"
#include "Globals.h"
#include "Console.h"
#include "SDL\include\SDL.h"

class UI_PerformancePanel;
class UI_ConfigurationPanel; 
class UI_ScenePanel; 
class UI_ConsolePanel;
class UI_RandomNumberPanel; 
class UI_InspectorPanel; 
class UI_Panel;
class UI_HierarchyPanel; 

enum Panel_Types { NULL_PANEL_TYPE, CONFIGURATION_PANEL, SCENE_PANEL, CONSOLE_PANEL, RANDOM_PANEL, INSPECTOR_PANEL, PERFORMANCE_PANEL, HIERARCHY_PANEL };

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

	void SendInput(SDL_Event* e) const;

	void ShowHelpMarker(const char* desc);

	Console console;

	bool is_initial_position = true;
	UI_InspectorPanel* inspector_panel;

private: 

	int x_motion, y_motion, last_x, last_y;
	bool dragging = false;

	bool show_demo_window = false; 
	bool show_style_editor = false; 

	std::list<UI_Panel*> panels_list;

	UI_ConfigurationPanel* config_panel; 
	UI_ScenePanel* scene_panel; 
	UI_ConsolePanel* console_panel; 
	UI_RandomNumberPanel* random_panel; 
	UI_PerformancePanel* performance_panel;
	UI_HierarchyPanel* hierarchy_panel; 

};

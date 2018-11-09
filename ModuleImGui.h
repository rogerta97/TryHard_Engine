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
class UI_ExplorerPanel; 
class UI_Panel;
class UI_HierarchyPanel; 
class UI_OctreePanel; 
class UI_TagPanel; 
class UI_GamePanel;
class UI_ClocksPanel;

enum Panel_Types 
{ 
	NULL_PANEL_TYPE, 
	CONFIGURATION_PANEL,
	SCENE_PANEL, 
	CONSOLE_PANEL, 
	RANDOM_PANEL, 
	INSPECTOR_PANEL, 
	PERFORMANCE_PANEL, 
	HIERARCHY_PANEL, 
	EXPLORER_PANEL, 
	GAME_PANEL,
	OCTREE_PANEL,
	TAG_PANEL,
	CLOCKS_PANEL,
};

class ModuleImGui : public Module
{
public:
	ModuleImGui(bool start_enabled = true);
	~ModuleImGui();

	bool Init(JSON_Object* config);
	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp();

	UI_Panel* AddPanel(Panel_Types type);

	// Utility
	update_status ShowSavePopup();

	update_status DrawTopBar();
	update_status DrawDocking();
	void SetDefaultStyle(); 

	void SendInput(SDL_Event* e) const;

	void ShowHelpMarker(const char* desc);

public:
	Console console;

	bool show_save_popup = false;

	bool is_initial_position = true;

	bool is_mouse_in_scene = false;

	UI_InspectorPanel* inspector_panel = nullptr;
	UI_HierarchyPanel* hierarchy_panel = nullptr;
	UI_ScenePanel* scene_panel = nullptr;
	UI_GamePanel* game_panel = nullptr;
	UI_TagPanel* tag_panel;

	std::list<UI_Panel*> panels_list;

private: 

	int x_motion, y_motion, last_x, last_y;
	bool dragging = false;

	bool show_demo_window = false; 
	bool show_style_editor = false; 

	bool close_on_next_update = false;



	UI_ConfigurationPanel* config_panel = nullptr; 
	UI_ExplorerPanel* explorer_panel = nullptr; 
	UI_OctreePanel* octree_panel = nullptr; 
	UI_ConsolePanel* console_panel = nullptr;
	UI_RandomNumberPanel* random_panel = nullptr;
	UI_PerformancePanel* performance_panel = nullptr;
	UI_ClocksPanel* clocks_panel = nullptr;
};

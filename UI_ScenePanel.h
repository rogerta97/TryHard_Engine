#pragma once

#include "UI_Panel.h"

class Material; 

class UI_ScenePanel : public UI_Panel
{
public:

	UI_ScenePanel();
	~UI_ScenePanel();

	bool Start();
	bool Update();

	ImVec2 GetMousePosInDock();
	ImVec2 GetMousePosInDockNormalized();
	ImVec2 GetPos() const;
	ImVec2 GetSize() const;

	bool is_mouse_in;
	bool is_dock_focused;

	bool show = true;

private:
	ImVec2 pos;
	ImVec2 region_size;

	Material* play_icon; 
	Material* stop_icon;
	Material* pause_icon;

	Material* play_icon_pressed;
	Material* stop_icon_pressed;
	Material* pause_icon_pressed;

	//To highlight the button if the bool is true
	bool play;
	bool pause;
};


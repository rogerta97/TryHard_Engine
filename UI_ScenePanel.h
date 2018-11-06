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

	bool is_mouse_in;

	bool show = true;

private:
	ImVec2 pos;
	ImVec2 region_size;

	Material* play_icon; 
	Material* stop_icon;
	Material* pause_icon;
};


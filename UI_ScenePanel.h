#pragma once

#include "UI_Panel.h"

class UI_ScenePanel : public UI_Panel
{
public:

	UI_ScenePanel();
	~UI_ScenePanel();

	bool Start();
	bool Update();

	bool is_mouse_in;
	bool show = true;
};


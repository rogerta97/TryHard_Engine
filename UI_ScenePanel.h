#pragma once

#include "UI_Panel.h"

class UI_ScenePanel : public UI_Panel
{
public:

	UI_ScenePanel();
	~UI_ScenePanel();

	bool Start();
	bool Update();

	bool show = true;
};


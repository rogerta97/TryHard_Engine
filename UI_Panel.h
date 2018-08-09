#pragma once

#include "imgui.h"
#include "Globals.h"
#include "ModuleImGui.h"

class UI_Panel
{
public:
	UI_Panel() {};
	~UI_Panel() {};

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update()
	{
		return true;
	}

	Panel_Types type;

	bool show;
};


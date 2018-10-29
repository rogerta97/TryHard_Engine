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

	const char* name = "";

	Panel_Types type;

	Timer performance_timer;
	std::vector<float> update_ms_buffer;
	bool show;
};


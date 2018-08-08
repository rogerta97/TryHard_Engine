#pragma once

#include "imgui.h"
#include "Globals.h"

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

	bool show; 
};


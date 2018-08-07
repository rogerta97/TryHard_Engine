#pragma once

#include "imgui.h"
#include "Globals.h"

class UI_ConfigurationPanel
{

public:
	UI_ConfigurationPanel();
	~UI_ConfigurationPanel();

	bool Start(); 
	bool Update();

	bool show = false; 


};



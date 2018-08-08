#pragma once

#include "UI_Panel.h"


class UI_ConfigurationPanel : public UI_Panel
{

public:
	UI_ConfigurationPanel();
	~UI_ConfigurationPanel();

	bool Start(); 
	bool Update();

	bool show = false; 


};



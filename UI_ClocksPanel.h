#pragma once
#include "UI_Panel.h"
class UI_Clocks :
	public UI_Panel
{
public:
	UI_Clocks();
	~UI_Clocks();

	bool Start();
	bool Update();
};


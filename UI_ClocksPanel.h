#pragma once
#include "UI_Panel.h"
class UI_ClocksPanel :
	public UI_Panel
{
public:
	UI_ClocksPanel();
	~UI_ClocksPanel();

	bool Start();
	bool Update();
};


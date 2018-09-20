#pragma once
#include "UI_Panel.h"
class UI_PerformancePanel :
	public UI_Panel
{
public:
	UI_PerformancePanel();
	~UI_PerformancePanel();

	bool Start();
	bool Update();

	bool show = false;
};


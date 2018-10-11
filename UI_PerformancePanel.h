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

	void PrintPreUpdatePlots();

	void PrintUpdatePlots();

	void PrintPostUpdatePlots();

	bool show = false;

private:
	ImVec2 size;
	std::list<Module*> list_modules;
	std::list<Module*>::iterator module_iterator;
};


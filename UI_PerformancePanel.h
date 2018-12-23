#ifndef __UI_PERFORMANCE_PANEL_H__
#define __UI_PERFORMANCE_PANEL_H_

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

private:
	ImVec2 size;
	std::list<Module*> list_modules;
	std::list<Module*>::iterator module_iterator;
};

#endif
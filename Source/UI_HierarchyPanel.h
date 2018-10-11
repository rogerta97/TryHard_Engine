#pragma once
#include "UI_Panel.h"
class UI_HierarchyPanel :
	public UI_Panel
{
public:
	UI_HierarchyPanel();
	~UI_HierarchyPanel();

	bool Start(); 
	bool Update(); 
	bool CleanUp(); 

	bool show_click_menu; 
};


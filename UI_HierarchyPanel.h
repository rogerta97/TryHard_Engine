#pragma once
#include "UI_Panel.h"

class GameObject; 

class UI_HierarchyPanel :
	public UI_Panel
{
public:
	UI_HierarchyPanel();
	~UI_HierarchyPanel();

	bool Start(); 
	bool Update(); 
	bool CleanUp(); 

	void ManageDragAndDrop(GameObject* current); 

	bool show_click_menu; 
	bool show_create_menu; 
	bool want_to_drag; 

	GameObject* source_in_drag; 
	GameObject* dst_in_drag; 
};


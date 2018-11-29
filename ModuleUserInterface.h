#pragma once

#include "Module.h"
#include <list>
#include "UI_Element.h"

class GameObject; 

class ModuleUserInterface : public Module
{
public:
	ModuleUserInterface();
	~ModuleUserInterface();

	bool Init(JSON_Object* config);
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	// Utility ---------

	UI_Element* CreateUIElement(UI_Widgget_Type type = UI_Widgget_Type::UI_NULL);	//Create the UI element

	void AddCanvas(GameObject* canvas_go);

	// -----------------

private:
	std::list<GameObject*> go_with_canvas; 
};


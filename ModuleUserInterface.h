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
	void AddaptCanvasToScreen();					// Will addapt all the canvas of the scene (unless they are in world space) to game screen size

	// -----------------

private:
	std::list<GameObject*> go_with_canvas; 
};


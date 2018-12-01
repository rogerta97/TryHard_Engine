#pragma once

#include "Module.h"
#include <list>
#include "UI_Element.h"

class GameObject; 
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

	void DrawSceneUI(GameObject* camera); 
	void AddCanvas(GameObject* canvas_go);
	void AddaptCanvasToScreen();					// Will addapt all the canvas of the scene (unless they are in world space) to game screen size
	GameObject* GetLastCanvas() const;						// Will return the UI_Canvas of the last GameObject in the list

	// -----------------

private:
	std::list<GameObject*> go_with_canvas; 
};


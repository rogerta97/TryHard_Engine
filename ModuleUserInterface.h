#pragma once

#include "Module.h"
#include <list>
#include "UI_Element.h"
#include "MathGeoLib\MathGeoLib.h"

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

	void AddCanvas(GameObject* canvas_go);
	void DeleteCanvas(GameObject* go);


	void DrawSceneUI(GameObject* camera); 
	void AddaptCanvasToScreen();					// Will addapt all the canvas of the scene (unless they are in world space) to game screen size
	GameObject* GetLastCanvas() const;						// Will return the UI_Canvas of the last GameObject in the list

	// -----------------

private:
	std::list<GameObject*> go_with_canvas; 
	AABB ui_render_box;
};


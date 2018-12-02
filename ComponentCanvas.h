#pragma once
#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"
#include "SDL\include\SDL_rect.h"

#include <list>
#include <string>

class UI_Element; 
class UI_Canvas; 

class ComponentCanvas : public Component
{
public:

	ComponentCanvas(GameObject* parent);
	~ComponentCanvas();

	bool Start();
	bool Update();
	bool CleanUp();
	void Draw(bool is_editor); 

	//Utility functions -----

	void AddElement(GameObject* new_element); 
	UI_Element* GetElement(std::string name) const;
	UI_Element* GetElement(uint element_id) const;
	
	// Creates GameObject with the UI element specified, by default it will add a child in the last canvas of the scene. 

	// Warning: Forcing a parent which is not UI will set the default behaviour. 

	// ---------------------- 

private:

	UI_Canvas * canvas; 
};

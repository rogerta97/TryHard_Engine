#pragma once
#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"
#include "SDL\include\SDL_rect.h"

#include <list>
#include <string>

class UI_Element; 
class GameObject; 
class UI_Canvas; 

class ComponentCanvas : public Component
{
public:

	ComponentCanvas(GameObject* parent);
	~ComponentCanvas();

	bool Start();
	bool Update();
	bool CleanUp();
	void Draw(); 

	//Utility functions -----

	void AddElement(UI_Element* new_element); 
	UI_Element* GetElement(std::string name) const;
	UI_Element* GetElement(uint element_id) const;

	// ----------------------

	void DrawFrame();
	void DrawElements(); 

private:

	UI_Canvas * canvas; 
};


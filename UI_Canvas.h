#pragma once
#include "GameObject.h"
#include "SDL\include\SDL_rect.h"
#include <list>

class ComponentCanvas; 

class UI_Canvas
{
public:
	UI_Canvas(ComponentCanvas* container);
	~UI_Canvas();

	ComponentCanvas* cmp_container; 

	void Update(); 
	void DeleteElementByUID(UID uid); 

	std::list<GameObject*> elements_in_canvas;
};


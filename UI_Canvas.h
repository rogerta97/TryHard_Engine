#pragma once
#include "GameObject.h"
#include "SDL\include\SDL_rect.h"
#include <list>

class UI_Canvas
{
public:
	UI_Canvas();
	~UI_Canvas();

	void Update(); 

	void DeleteElementByUID(UID uid); 

	std::list<GameObject*> elements_in_canvas;
};


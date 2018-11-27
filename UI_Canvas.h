#pragma once
#include "UI_Element.h"
#include "SDL\include\SDL_rect.h"
#include <list>

class UI_Canvas
{
public:
	UI_Canvas();
	~UI_Canvas();

	std::list<UI_Element*> elements_in_canvas;
	SDL_Rect frame_rect;
};


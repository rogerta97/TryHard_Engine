#include "UI_Canvas.h"


UI_Canvas::UI_Canvas()
{
}


UI_Canvas::~UI_Canvas()
{
}

void UI_Canvas::Update()
{
	for (auto it = elements_in_canvas.begin(); it != elements_in_canvas.end(); it++)
	{
		(*it)->Update(); 
	}
}

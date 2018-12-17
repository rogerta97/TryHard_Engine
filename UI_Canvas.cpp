#include "UI_Canvas.h"
#include "ComponentCanvas.h"

UI_Canvas::UI_Canvas(ComponentCanvas* canvas)
{
	cmp_container = canvas; 
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

void UI_Canvas::DeleteElementByUID(UID uid)
{
	for (auto it = elements_in_canvas.begin(); it != elements_in_canvas.end(); it++)
	{
		if ((*it)->unique_id == uid)
		{
			elements_in_canvas.erase(it);
			break; 
		}			
	}
}

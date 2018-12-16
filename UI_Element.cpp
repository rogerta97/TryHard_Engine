#include "UI_Element.h"
#include "GameObject.h"

UI_Element::UI_Element()
{
	
}

UI_Element::UI_Element(UI_Widgget_Type type)
{
	wid_type = type; 
}

UI_Element::~UI_Element()
{
}

void UI_Element::Start()
{
}

void UI_Element::Update()
{
	
}

void UI_Element::CleanUp()
{
}

void UI_Element::Draw()
{
}

UI_Widgget_Type UI_Element::GetType() const
{
	return wid_type;
}

void UI_Element::SetType(UI_Widgget_Type new_type)
{
	wid_type = new_type;
}

float UI_Element::GetPercentage() const
{
	return canvas_percentage;
}

void UI_Element::SetPercentage(float new_num)
{
	canvas_percentage = new_num;
}

bool UI_Element::GetRaycast() const
{
	return raycast;
}

void UI_Element::SetRaycast(bool new_type)
{
	raycast = new_type; 
}

UI_Canvas * UI_Element::GetCanvas() const
{
	return canvas;
}

void UI_Element::SetCanvas(UI_Canvas * new_canvas)
{
	canvas = new_canvas;
}

void UI_Element::SetState(const UI_ElementState new_state)
{
	state = new_state; 
}

UI_ElementState UI_Element::GetState() const
{
	return state;
}

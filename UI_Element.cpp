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

bool UI_Element::GetRaycast() const
{
	return raycast;
}

void UI_Element::SetRaycast(bool new_type)
{
	raycast = new_type; 
}

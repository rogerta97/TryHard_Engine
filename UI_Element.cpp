#include "UI_Element.h"

UI_Element::UI_Element()
{
	// Add the rect transform component to it

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

UI_Widgget_Type UI_Element::GetType() const
{
	return wid_type;
}

void UI_Element::SetType(UI_Widgget_Type new_type)
{
	wid_type = new_type;
}

#include "ComponentCanvas.h"
#include "ComponentRectTransform.h"
#include "Application.h"
#include "DebugDraw.h"
#include "UI_Canvas.h"

ComponentCanvas::ComponentCanvas(GameObject* parent)
{
	component_type = CMP_CANVAS;
	gameobject = parent; 
}


ComponentCanvas::~ComponentCanvas()
{
	delete canvas; 
}

bool ComponentCanvas::Start()
{

	canvas = new UI_Canvas();
	App->user_interface->AddCanvas(gameobject); 

	return false;
}

bool ComponentCanvas::Update()
{
	Draw();
	return true;
}

bool ComponentCanvas::CleanUp()
{
	return true;
}

void ComponentCanvas::Draw()
{
	DrawFrame();
	DrawElements();
}

void ComponentCanvas::AddElement(UI_Element * new_element)
{
	if (new_element != nullptr)
		canvas->elements_in_canvas.push_back(new_element); 
	else
	{
		CONSOLE_ERROR("Element Trying to be added is NULLPTR"); 
	}
}

UI_Element * ComponentCanvas::GetElement(std::string name) const
{
	return nullptr;
}

UI_Element * ComponentCanvas::GetElement(uint element_id) const
{
	return nullptr;
}

void ComponentCanvas::DrawFrame()
{
	
}

void ComponentCanvas::DrawElements()
{
}

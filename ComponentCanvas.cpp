#include "ComponentCanvas.h"
#include "ComponentRectTransform.h"
#include "Application.h"
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
	canvas->Update();
	return true;
}

bool ComponentCanvas::CleanUp()
{
	return true;
}

void ComponentCanvas::Draw(bool is_editor)
{
	for (auto it = canvas->elements_in_canvas.begin(); it != canvas->elements_in_canvas.end(); it++)
	{
		(*it)->Draw(is_editor);
	}
}

void ComponentCanvas::AddElement(GameObject * new_element)
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




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
	// TODO1_UI
	// Here we should get the size of the rendering texture and create the canvas from it, NOT FROM THE CAMERA FRUSTUM SIZE (imagine there's no cam, ui should have to render. We also wouldn't have to take in account FOV).
	// Thus, we need to keep the AR for the game camera always the same when resizing. 
	// For now we will force a 500x500 canvas

	canvas = new UI_Canvas();

	//ComponentRectTransform* r_transform = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM);
	//if (r_transform != nullptr)
	//{
	//	r_transform->AddaptRectToScreenSize();
	//}

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

#include "ComponentButton.h"
#include "ComponentRectTransform.h"

#include "UI_Button.h"
#include "UI_Plane.h"
#include "UI_Canvas.h"
#include "UI_Image.h"

ComponentButton::ComponentButton(GameObject * parent)
{
	gameobject = parent; 
	component_type = CMP_BUTTON; 
	button = new UI_Button(this); 
	callback_system = new UICallbackSystem(this); 
}

ComponentButton::~ComponentButton()
{
}

bool ComponentButton::Start()
{
	return false;
}

bool ComponentButton::Update()
{
	button->Update(); 

	return true;
}

bool ComponentButton::CleanUp()
{
	// Delete object from the canvas 
	UI_Canvas* canvas_container = button->GetCanvas();
	canvas_container->DeleteElementByUID(gameobject->unique_id);

	// Callback System 
	OnMousePressed.clear();
	callback_system->CleanSystem();
	delete callback_system; 

	button->CleanUp();
	delete button; 

	return true;
}

void ComponentButton::Draw(bool is_editor)
{
	if(button)
		button->Draw(is_editor);
}

void ComponentButton::FitToRect()
{
	ComponentRectTransform* rtransform = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM);
	button->GetArea()->Resize(rtransform->width, rtransform->height);
}

UI_Button * ComponentButton::GetButton() const
{
	return button;
}

void ComponentButton::BindCallbackFunctions()
{
	for (auto it = callback_system->GetCallbacks().begin(); it != callback_system->GetCallbacks().end(); it++)
	{
		if ((*it)->action_char != nullptr)
		{
			std::function<void()> binded_func = std::bind((*it)->action_char, (*it)->value_char);
			OnMousePressed.push_back(binded_func);
		}
	}
}

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
	return false;
}

bool ComponentButton::CleanUp()
{
	// Delete object from the canvas 
	UI_Canvas* canvas_container = button->GetCanvas();
	canvas_container->DeleteElementByUID(gameobject->unique_id);

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

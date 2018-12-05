#include "ComponentButton.h"
#include "UI_Button.h"

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
	button->CleanUp();
	delete button; 

	return true;
}

void ComponentButton::Draw(bool is_editor)
{
	if(button)
		button->Draw(is_editor);
}

UI_Button * ComponentButton::GetButton() const
{
	return button;
}

#include "ComponentButton.h"




ComponentButton::ComponentButton(GameObject * parent)
{
	gameobject = parent; 
	component_type = CMP_BUTTON; 
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
	return false;
}

void ComponentButton::Draw(bool is_editor)
{
}

UI_button * ComponentButton::GetButton() const
{
	return button;
}
